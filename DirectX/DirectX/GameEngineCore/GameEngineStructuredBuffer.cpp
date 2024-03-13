#include "PrecompileHeader.h"
#include "GameEngineStructuredBuffer.h"

std::map<std::string, std::map<int, std::shared_ptr < GameEngineStructuredBuffer>>> GameEngineStructuredBuffer::StructuredBufferRes;


GameEngineStructuredBuffer::GameEngineStructuredBuffer() 
{
}

GameEngineStructuredBuffer::~GameEngineStructuredBuffer() 
{
	Release();
}

void GameEngineStructuredBuffer::Release()
{
	if (nullptr != WriteBuffer)
	{
		WriteBuffer->Release();
		WriteBuffer = nullptr;
	}

	if (nullptr != ReadBuffer)
	{
		ReadBuffer->Release();
		ReadBuffer = nullptr;
	}

	if (nullptr != UnorderedAccessView)
	{
		UnorderedAccessView->Release();
		UnorderedAccessView = nullptr;
	}


	if (nullptr != ShaderResourceView)
	{
		ShaderResourceView->Release();
		ShaderResourceView = nullptr;
	}
}

void GameEngineStructuredBuffer::CreateResize(const D3D11_SHADER_BUFFER_DESC& _Desc, int Count, StructuredBufferType _Type, void* _StartData, bool _CPUAccess)
{
	if (false == IsInit)
	{
		ShaderDesc = _Desc;
		IsInit = true;
	}

	CreateResize(ShaderDesc.Size, Count, _Type, _StartData, _CPUAccess);
}

void GameEngineStructuredBuffer::CreateResize(size_t Count, StructuredBufferType _Type, void* _StartData /*= nullptr*/, bool _CPUAccess)
{
	CreateResize(DataSize, Count, _Type, _StartData, _CPUAccess);
}

void GameEngineStructuredBuffer::CreateResize(size_t _DataSize, size_t Count, StructuredBufferType _Type, void* _StartData/* = nullptr*/, bool _CPUAccess)
{
	if (0 == _DataSize)
	{
		MsgAssert("������ ����� 0�� ����ȭ ���۸� ������� �����ϴ�.");
	}

	DataSize = static_cast<int>(_DataSize);

	if (0 == Count)
	{
		return;
	}

	if (DataCount >= Count)
	{
		return;
	}

	Release();

	D3D11_BUFFER_DESC Desc;

	Desc.ByteWidth = DataSize * DataCount;
	DataCount = static_cast<int>(Count);

	BufferInfo.ByteWidth = DataSize * DataCount; // GPU �� ������ ����ȭ ���� �޸� ũ��(�ּҴ��� ??)
	BufferInfo.StructureByteStride = DataSize; // 1�� ũ�⵵ �־���� �Ѵ�.

	CreateResize(BufferInfo, _Type, _StartData, _CPUAccess);
}

void GameEngineStructuredBuffer::CreateResize(const D3D11_BUFFER_DESC& _Data, StructuredBufferType _Type, void* _StartData, bool _CPUAccess)
{
	BufferInfo = _Data;

	BufferInfo.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	BufferInfo.Usage = D3D11_USAGE_DEFAULT;

	DataType = _Type;

	switch (_Type)
	{
	case StructuredBufferType::SRV_ONLY:
		BufferInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		BufferInfo.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		BufferInfo.Usage = D3D11_USAGE_DYNAMIC;
		break;
	case StructuredBufferType::UAV_INC:
		// ��ǻƮ ���̴��� ���� �뵵�� �Ǹ� CPU ���� �ɼ��� �Ұ���������.
		BufferInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		BufferInfo.CPUAccessFlags = 0;
		BufferInfo.Usage = D3D11_USAGE_DEFAULT;
		break;
	default:
		break;
	}

	D3D11_SUBRESOURCE_DATA* StartDataPtr = nullptr;
	D3D11_SUBRESOURCE_DATA StartData = { 0 };
	if (nullptr != _StartData)
	{
		StartData.SysMemPitch = 0;
		StartData.SysMemSlicePitch = 0;
		StartData.pSysMem = _StartData;
		StartDataPtr = &StartData;
	}

	// ������� �ϰ� 
	if (S_OK != GameEngineDevice::GetDevice()->CreateBuffer(&BufferInfo, StartDataPtr, &Buffer))
	{
		MsgAssert("��Ʈ��ó�� ���� ������ �����߽��ϴ�.");
	}

	// Shader Resource View
	// ���̴� ���ҽ��� ����µ� ���۱�� 
	// D3D11_SRV_DIMENSION_BUFFEREX ���۱�� ���̴� ���ҽ� ���°��� �˷��ֱ� ���� �뵵
	D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};
	tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	tSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	tSRVDesc.BufferEx.FirstElement = 0;
	tSRVDesc.BufferEx.Flags = 0;
	tSRVDesc.BufferEx.NumElements = DataCount; // 10���� �츮���� m_pSRV�� �־�޶�.

	if (S_OK != GameEngineDevice::GetDevice()->CreateShaderResourceView(Buffer, &tSRVDesc, &ShaderResourceView))
	{
		MsgAssert("FAIL (S_OK != GameEngineDevice::GetDevice()->CreateShaderResourceView(GetBuffer(), &tSRVDesc, &m_pSRV))");
	}

	if (_Type == StructuredBufferType::UAV_INC)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};
		UAVDesc.Buffer.NumElements = DataCount;
		UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;

		if (S_OK != GameEngineDevice::GetDevice()->CreateUnorderedAccessView(Buffer, &UAVDesc, &UnorderedAccessView))
		{
			// ��ǻƮ ���̴��� view
			MsgAssert("if (S_OK != GameEngineDevice::GetDevice()->CreateUnorderedAccessView(Buffer, &UAVDesc, &UnorderedAccessView))");
		}
	}

	if (_CPUAccess)
	{
		BufferInfo.ByteWidth = DataSize * DataCount;	// ���� ��ü ũ��
		BufferInfo.StructureByteStride = DataSize;		// ���� ��� ũ��			
		BufferInfo.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED; // ����ȭ ���� �߰� �÷��� ����
		BufferInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;	// Texture Register Binding	

		BufferInfo.Usage = D3D11_USAGE_DYNAMIC;
		BufferInfo.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;


		if (GameEngineDevice::GetDevice()->CreateBuffer(&BufferInfo, nullptr, &WriteBuffer))
		{
			MsgAssert("if (GameEngineDevice::GetDevice()->CreateBuffer(&BufferInfo, nullptr, &WriteBuffer))");
		}


		BufferInfo.Usage = D3D11_USAGE_DEFAULT;
		BufferInfo.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

		if (GameEngineDevice::GetDevice()->CreateBuffer(&BufferInfo, nullptr, &ReadBuffer))
		{
			MsgAssert("if (GameEngineDevice::GetDevice()->CreateBuffer(&BufferInfo, nullptr, &ReadBuffer))");
		}
	}
}


void GameEngineStructuredBuffer::ChangeData(const void* _Data, size_t _Size)
{
	// 512 ����Ʈ �����͸� �������ټ� �ִ� ���۸� ������ٰ� �ϴ���
// ��¥ 512���� ����Ʈ�� �����ϴ°��� �ƴҼ��� �����Ƿ�
// ������ ���� ���õ��� ���̵� ����Ʈ�� �������� �ʰ�����
// �����ϴϱ� ������ �ص־� �Ұ̴ϴ�.

// ���⿡�� ���� �����͸� �������ְ� �Ǵµ�.
#ifdef _DEBUG
	//if (m_BufferInfo.ByteWidth != _Size)
	//{
	//	GameEngineDebug::AssertMsg("if (m_BufferInfo.ByteWidth != _Size)");
	//}
	SettingResources.pData = nullptr;
#endif

	GameEngineDevice::GetContext()->Map(Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &SettingResources);

#ifdef _DEBUG
	if (nullptr == SettingResources.pData)
	{
		MsgAssert("if (nullptr == m_Res.pData)");
	}
#endif 

	const float4x4* Ptr = reinterpret_cast<const float4x4*>(_Data);

	float4x4 Test = *Ptr;

	memcpy_s(SettingResources.pData, BufferInfo.ByteWidth, _Data, _Size);

	GameEngineDevice::GetContext()->Unmap(Buffer, 0);
}


void GameEngineStructuredBuffer::VSReset(int _BindPoint)
{
	ID3D11ShaderResourceView* Nullptr = nullptr;

	GameEngineDevice::GetContext()->VSSetShaderResources(_BindPoint, 1, &Nullptr);
}
void GameEngineStructuredBuffer::PSReset(int _BindPoint)
{
	ID3D11ShaderResourceView* Nullptr = nullptr;

	GameEngineDevice::GetContext()->PSSetShaderResources(_BindPoint, 1, &Nullptr);
}

void GameEngineStructuredBuffer::GSReset(int _BindPoint)
{
	ID3D11ShaderResourceView* Nullptr = nullptr;
	GameEngineDevice::GetContext()->GSSetShaderResources(_BindPoint, 1, &Nullptr);
}

void GameEngineStructuredBuffer::CSReset(int _BindPoint)
{
	UINT i = -1;
	ID3D11UnorderedAccessView* Nullptr = nullptr;
	GameEngineDevice::GetContext()->CSSetUnorderedAccessViews(_BindPoint, 1, &Nullptr, &i);
}

void GameEngineStructuredBuffer::VSSetting(int _BindPoint)
{
	if (nullptr == ShaderResourceView)
	{
		MsgAssert("�������� �ʴ� ��Ʈ��ó�� ���۸� ����� �� �����ϴ�.");
	}

	GameEngineDevice::GetContext()->VSSetShaderResources(_BindPoint, 1, &ShaderResourceView);
}

void GameEngineStructuredBuffer::CSRWSetting(int _BindPoint)
{
	if (nullptr == UnorderedAccessView)
	{
		MsgAssert("�������� �ʴ� �� ������ �׼��� �並 �� ����� �� �����ϴ�.");
	}

	UINT i = -1;
	GameEngineDevice::GetContext()->CSSetUnorderedAccessViews(_BindPoint, 1, &UnorderedAccessView, &i);
}

void GameEngineStructuredBuffer::PSSetting(int _BindPoint)
{
	if (nullptr == ShaderResourceView)
	{
		MsgAssert("�������� �ʴ� ����ȭ ���۸� ����� �� �����ϴ�.");
	}

	GameEngineDevice::GetContext()->PSSetShaderResources(_BindPoint, 1, &ShaderResourceView);
}

void GameEngineStructuredBuffer::CSSetting(int _BindPoint)
{
	if (nullptr == ShaderResourceView)
	{
		MsgAssert("�������� �ʴ� ����ȭ ���۸� ����� �� �����ϴ�.");
	}

	GameEngineDevice::GetContext()->CSSetShaderResources(_BindPoint, 1, &ShaderResourceView);
}

void GameEngineStructuredBuffer::GSSetting(int _BindPoint)
{
	if (nullptr == ShaderResourceView)
	{
		MsgAssert("�������� �ʴ� ����ȭ ���۸� ����� �� �����ϴ�.");
	}

	GameEngineDevice::GetContext()->GSSetShaderResources(_BindPoint, 1, &ShaderResourceView);
}


void GameEngineStructuredBuffer::SetData(void* _pSrc, UINT _DataCount)
{
	// ������ ���ڶ�� �߰��Ҵ��ϸ鼭 �ʱ�ȭ�Ѵ�.
	if (DataCount < _DataCount)
	{
		CreateResize(DataSize, _DataCount, DataType, _pSrc);
	}

	// ������ ����ϴٸ�, ������ ����
	else
	{
		D3D11_MAPPED_SUBRESOURCE tMapSub = {};

		GameEngineDevice::GetContext()->Map(WriteBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tMapSub);
		memcpy(tMapSub.pData, _pSrc, DataSize * DataCount);
		GameEngineDevice::GetContext()->Unmap(WriteBuffer, 0);

		// ������� -> ���ι���
		GameEngineDevice::GetContext()->CopyResource(Buffer, WriteBuffer);
	}
}
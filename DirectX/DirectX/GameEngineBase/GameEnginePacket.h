#pragma once
#include "GameEngineSerializer.h"
#include "GameEngineDebug.h"
#include <memory>

// 설명 :
class GameEnginePacket : public std::enable_shared_from_this<GameEnginePacket>
{
public:
	// constrcuter destructer
	GameEnginePacket();
	virtual ~GameEnginePacket();

	// delete Function
	GameEnginePacket(const GameEnginePacket& _Other) = delete;
	GameEnginePacket(GameEnginePacket&& _Other) noexcept = delete;
	GameEnginePacket& operator=(const GameEnginePacket& _Other) = delete;
	GameEnginePacket& operator=(GameEnginePacket&& _Other) noexcept = delete;

	template<typename EnumType>
	void SetPacketID(EnumType _PacketID)
	{
		PacketID = static_cast<int>(_PacketID);
	}

	void SetPacketID(unsigned int _PacketID)
	{
		PacketID = _PacketID;
	}

	void SerializePacket(GameEngineSerializer& _Ser)
	{
		Serialize(_Ser);
		SerializeEnd(_Ser);
	}

	void DeSerializePacket(GameEngineSerializer& _Ser)
	{
		DeSeralize(_Ser);
	}

	unsigned int GetPacketID()
	{
		return PacketID;
	}

	unsigned int GetObjectID()
	{
		return ObjectID;
	}

	void SetObjectID(unsigned int _ObjectID)
	{
		ObjectID = _ObjectID;
	}

protected:
	virtual void Serialize(GameEngineSerializer& _Ser)
	{
		_Ser << PacketID;
		_Ser << Size;
		_Ser << ObjectID;
	}

	virtual void DeSeralize(GameEngineSerializer& _Ser)
	{
		_Ser >> PacketID;
		_Ser >> Size;
		_Ser >> ObjectID;
	}

private:
	unsigned int PacketID = -1; // <= 종류인데 int로 처리한다.
	unsigned int Size = -1; // <= 패킷의 크기
	unsigned int ObjectID = -1; // <= 어떤 오브젝트가 보냈다.

	void SerializeEnd(GameEngineSerializer& _Ser)
	{
		Size = _Ser.GetWriteOffSet();

		if (Size <= 0)
		{
			MsgAssert("0바이트 패킷이 감지되었습니다.");
		}

		// 이녀석의 4바이트 번째의 데이터를 size로 교체한다.
		unsigned char* Ptr = _Ser.GetDataPtr();
		memcpy_s(&Ptr[4], sizeof(int), &Size, sizeof(int));
	}
};

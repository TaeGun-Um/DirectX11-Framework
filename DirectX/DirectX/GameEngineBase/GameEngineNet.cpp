#include "PrecompileHeader.h"
#include "GameEngineNet.h"
#include "GameEngineSerializer.h"
#include "GameEnginePacket.h"

GameEngineNet::GameEngineNet() 
{
}

GameEngineNet::~GameEngineNet() 
{
	IsNetValue = false;
}

void GameEngineNet::RecvThreadFunction(SOCKET _Socket, GameEngineNet* _Net)
{
	// 1500 - 1024
	char Data[1024] = { 0 };

	GameEngineSerializer Serializer;
	unsigned int PacketType = -1;
	unsigned int PacketSize = -1;

	while (true == _Net->IsNet())
	{
		// 상대가 보내온 패킷의 양.
		int Result = recv(_Socket, Data, sizeof(Data), 0);

		// 접속 끊김
		if (-1 == Result)
		{
			return;
		}

		// 접속 끊김
		if (SOCKET_ERROR == _Socket || INVALID_SOCKET == _Socket)
		{
			return;
		}

		// TCP 에서 문제가 될수 있는부분.
		// TCP가 12바이트를 쏜다면
		// 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 
		// 7바이트

		// 우리의 프로토콜은 
		// 무조건 적으로
		// 맨앞 4바이트 패킷의 타입
		// 그 뒤 4바이트 사이즈 
		Serializer.Write(Data, Result);

		if (8 > Serializer.GetWriteOffSet())
		{
			continue;
		}

		// 8바이트 이상 받았고
		// 아직 패킷타입이 뭔지 알아내지 못했다면
		if (-1 == PacketType)
		{
			// 패킷타입 알아낸다.
			{
				unsigned char* TypePivotPtr = &Serializer.GetDataPtr()[0];
				int* ConvertPtr = reinterpret_cast<int*>(TypePivotPtr);
				PacketType = *ConvertPtr;
			}

			// 사이즈 알아내고
			{
				unsigned char* SizePivotPtr = &Serializer.GetDataPtr()[4];
				int* ConvertPtr = reinterpret_cast<int*>(SizePivotPtr);
				PacketSize = *ConvertPtr;
			}
		}

		// 패킷사이즈로도 검증할게 최소 최대 패킷이라던가
		if (-1 == PacketSize)
		{
			return;
		}

		// 8바이트 이상 받았지만
		// 그걸 통해서 알아낸 패킷의 크기보다는 덜았어. 
		if (PacketSize > Serializer.GetWriteOffSet())
		{
			continue;
		}

		while (true)
		{
			
			// ConnectIDPacket
			std::shared_ptr<GameEnginePacket> Packet = _Net->Dispatcher.ConvertPacket(PacketType, Serializer);

			if (nullptr != Packet)
			{
				_Net->RecvPacketLock.lock();
				_Net->RecvPacket.push_back(Packet);
				_Net->RecvPacketLock.unlock();
			} else 
			{
				int a = 0;
			}

			// Write가 증가할 것이다.
			// _Net->Dispatcher.ProcessPacket(Packet);
			
			// 12 바이트가 날아왔는데 12바이트가 처리됐다.
			if (Serializer.GetWriteOffSet() == Serializer.GetReadOffSet())
			{
				Serializer.Reset();
				PacketType = -1;
				PacketSize = -1;
				break;
			}
			else
			{
				unsigned int RemainSize = Serializer.GetWriteOffSet() - Serializer.GetReadOffSet();

				if (8 > RemainSize)
				{
					break;
				}

				{
					unsigned char* TypePivotPtr = &Serializer.GetDataPtr()[0];
					int* ConvertPtr = reinterpret_cast<int*>(TypePivotPtr);
					PacketType = *ConvertPtr;
				}

				{
					unsigned char* SizePivotPtr = &Serializer.GetDataPtr()[4];
					int* ConvertPtr = reinterpret_cast<int*>(SizePivotPtr);
					PacketSize = *ConvertPtr;
				}

				if (RemainSize < PacketSize)
				{
					// 남은 찌거기를 다 앞으로 밀어버린다.
					Serializer.ClearReadData();
					break;
				}
			}
		}
	}
}



void GameEngineNet::SendPacket(std::shared_ptr<GameEnginePacket> _Packet, int _IgnoreID)
{
	GameEngineSerializer Ser;
	_Packet->SerializePacket(Ser);
	Send(reinterpret_cast<const char*>(Ser.GetDataPtr()), Ser.GetWriteOffSet(), _IgnoreID);

}

void GameEngineNet::Send(SOCKET _Socket, const char* Data, unsigned int _Size)
{
	send(_Socket, Data, _Size, 0);
}

void GameEngineNet::UpdatePacket()
{
	// 이게 쓰레드에서 또 이뤄지면 위험할수 있다.
	RecvPacketLock.lock();

	if (0 >= RecvPacket.size())
	{
		RecvPacketLock.unlock();
		return;
	}

	ProcessPackets = RecvPacket;
	RecvPacket.clear();
	RecvPacketLock.unlock();

	for (std::shared_ptr<GameEnginePacket> Packet : ProcessPackets)
	{
		Dispatcher.ProcessPacket(Packet);
	}

	ProcessPackets.clear();
};
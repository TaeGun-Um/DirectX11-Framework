#pragma once
#include <functional>
#include "GameEnginePacket.h"

// 설명 :
class GameEngineDispatcher
{
public:
	// constrcuter destructer
	GameEngineDispatcher();
	~GameEngineDispatcher();

	// delete Function
	GameEngineDispatcher(const GameEngineDispatcher& _Other) = delete;
	GameEngineDispatcher(GameEngineDispatcher&& _Other) noexcept = delete;
	GameEngineDispatcher& operator=(const GameEngineDispatcher& _Other) = delete;
	GameEngineDispatcher& operator=(GameEngineDispatcher&& _Other) noexcept = delete;

	template<typename PacketType, typename EnumType>
	void AddHandler(std::function<void(std::shared_ptr<PacketType>)> _CallBack)
	{
		AddHandler(_CallBack);
	}

	template<typename PacketType>
	void AddHandler(std::function<void(std::shared_ptr<PacketType>)> _CallBack)
	{
		int Type = static_cast<int>(PacketType::Type);

		if (true == PacketHandlers.contains(Type))
		{
			MsgAssert("이미 존재하는 핸드러를 또 등록하려고 했습니다.");
		}

		ConvertPacketHandlers[Type] = [=](GameEngineSerializer& _Ser)
		{
			std::shared_ptr<PacketType> NewPacket = std::make_shared<PacketType>();
			NewPacket->DeSerializePacket(_Ser);
			return NewPacket;
		};

		PacketHandlers[Type] = [=](std::shared_ptr<GameEnginePacket> _Packet)
		{
			std::shared_ptr<PacketType> ConvertPacket = std::dynamic_pointer_cast<PacketType>(_Packet);

			if (nullptr == ConvertPacket)
			{
				MsgAssert("패킷 타입 변환에 실패했습니다.");
			}

			_CallBack(ConvertPacket);
		};
	}

	std::shared_ptr<GameEnginePacket> ConvertPacket(int Type, GameEngineSerializer& _Ser)
	{
		if (false == ConvertPacketHandlers.contains(Type))
		{
			MsgAssert("AddHandler를 등록하지 않은 패킷입니다");
			return nullptr;
		}

		return ConvertPacketHandlers[Type](_Ser);
	}

	void ProcessPacket(std::shared_ptr<GameEnginePacket> _Packet)
	{
		if (false == PacketHandlers.contains(_Packet->GetPacketID()))
		{
			MsgAssert("AddHandler를 등록하지 않은 패킷입니다");
			return;
		}

		PacketHandlers[_Packet->GetPacketID()](_Packet);
	}


protected:

private:
	// 0번이 들어오면 ConnectIDPacket 바꿔서 리턴
	// 1번이 들어오면 ObjectUpdatePacket 바꿔서 리턴
	std::map<int, std::function<std::shared_ptr<GameEnginePacket>(GameEngineSerializer&)>> ConvertPacketHandlers;

	// 어떤 패킷이 들어오면 어떻게 처리해?
	// int Packet 
	// std::function<void(std::shared_ptr<ConnectIDPacket>)>
	// std::function<void(std::shared_ptr<GameEnginePacket>)>
	std::map<int, std::function<void(std::shared_ptr<GameEnginePacket>)>> PacketHandlers;
};


#pragma once
#include <GameEngineBase/GameEnginePacket.h>
#include <GameEngineCore/GameEngineTransform.h>
#include "PacketEnum.h"

// Ό³Έν :
class ObjectUpdatePacket : public GameEnginePacket
{
public:
	static const PacketEnum Type = PacketEnum::ObjectUpdatePacket;

public:
	// constrcuter destructer
	ObjectUpdatePacket()
	{
		SetPacketID(Type);
	}
	~ObjectUpdatePacket()
	{
	}

	float4 Rotation;
	float4 Position;

protected:
	void Serialize(GameEngineSerializer& _Ser) override
	{
		GameEnginePacket::Serialize(_Ser);
		_Ser << Rotation;
		_Ser << Position;
	}

	void DeSeralize(GameEngineSerializer& _Ser) override
	{
		GameEnginePacket::DeSeralize(_Ser);
		_Ser >> Rotation;
		_Ser >> Position;
	}


};


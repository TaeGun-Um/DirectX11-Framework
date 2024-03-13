#pragma once

#include "GameEngineDispatcher.h"
#include <mutex>

// 설명 :
// 이녀석은 서버와 클라가 동시에 사용하는 클래스
class GameEngineNet
{
public:
	// constrcuter destructer
	GameEngineNet();
	~GameEngineNet();

	// delete Function
	GameEngineNet(const GameEngineNet& _Other) = delete;
	GameEngineNet(GameEngineNet&& _Other) noexcept = delete;
	GameEngineNet& operator=(const GameEngineNet& _Other) = delete;
	GameEngineNet& operator=(GameEngineNet&& _Other) noexcept = delete;

	static void Send(SOCKET _Socket, const char* Data, unsigned int _Size);

	virtual void Send(const char* Data, unsigned int _Size, int _IgnoreID = -1) = 0;
	void SendPacket(std::shared_ptr<class GameEnginePacket> _Packet, int _IgnoreID = -1);

	inline bool IsNet() 
	{
		return IsNetValue;
	}

	GameEngineDispatcher Dispatcher;

	void UpdatePacket();

protected:
	static void RecvThreadFunction(SOCKET _Socket, GameEngineNet* _Net);

private:
	std::atomic<bool> IsNetValue = true;

	std::mutex RecvPacketLock;
	std::list<std::shared_ptr<GameEnginePacket>> RecvPacket;

	std::list<std::shared_ptr<GameEnginePacket>> ProcessPackets;


};


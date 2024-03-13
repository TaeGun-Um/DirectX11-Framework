#pragma once
#include "GameEngineNet.h"
#include "GameEngineThread.h"

// Ό³Έν :
class GameEngineNetClient : public GameEngineNet
{
public:
	// constrcuter destructer
	GameEngineNetClient();
	~GameEngineNetClient();

	// delete Function
	GameEngineNetClient(const GameEngineNetClient& _Other) = delete;
	GameEngineNetClient(GameEngineNetClient&& _Other) noexcept = delete;
	GameEngineNetClient& operator=(const GameEngineNetClient& _Other) = delete;
	GameEngineNetClient& operator=(GameEngineNetClient&& _Other) noexcept = delete;

	bool Connect(const std::string& _IP, unsigned short _Port);

protected:
	void Send(const char* Data, unsigned int _Size, int _IgnoreID = -1) override;

private:
	GameEngineThread RecvThread;
	SOCKET ClientSocket;
};


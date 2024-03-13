#pragma once
#include "GameEngineNet.h"
#include "GameEngineThread.h"
#include "GameEnginePacket.h"

// 설명 :
class GameEngineNetServer : public GameEngineNet
{
public:
	// constrcuter destructer
	GameEngineNetServer();
	~GameEngineNetServer();

	// delete Function
	GameEngineNetServer(const GameEngineNetServer& _Other) = delete;
	GameEngineNetServer(GameEngineNetServer&& _Other) noexcept = delete;
	GameEngineNetServer& operator=(const GameEngineNetServer& _Other) = delete;
	GameEngineNetServer& operator=(GameEngineNetServer&& _Other) noexcept = delete;

	void ServerOpen(short _Port, int _BackLog = 512);

	void SetAcceptCallBack(std::function<void(SOCKET, GameEngineNetServer*)> _AccpetCallBack)	
	{
		AcceptCallBack = _AccpetCallBack;
	}
	
	void AddUser(int _ID, SOCKET _UserSocket)
	{
		if (true == Users.contains(_ID))
		{
			MsgAssert("이미 존재하는 유저가 또 존재할수는 없습니다 ID 오류 입니다.");
			return;
		}

		Users[_ID] = _UserSocket;
	}

protected:
	void Send(const char* Data, unsigned int _Size, int _IgnoreID = -1) override;

private:
	int BackLog = 512;
	SOCKET AcceptSocket;

	GameEngineThread AccpetThread;

	std::map<int, SOCKET> Users;

	// std::vector<SOCKET> Users;


	std::vector<std::shared_ptr<GameEngineThread>> RecvThreads;

	std::function<void(SOCKET _AcceptSocket, GameEngineNetServer* _Net)> AcceptCallBack;

	static void AcceptThread(SOCKET _AcceptSocket, GameEngineNetServer* _Net);
};


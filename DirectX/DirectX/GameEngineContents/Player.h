#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineBase/GameEngineNetObject.h>

// 설명 :
// 아예 서버 적용 플레이어를 만들거냐.
class Player : public GameEngineActor, public GameEngineNetObject
{
public:
	static Player* MainPlayer;

public:
	// constrcuter destructer
	Player();
	~Player();

	// delete Function
	Player(const Player& _Other) = delete;
	Player(Player&& _Other) noexcept = delete;
	Player& operator=(const Player& _Other) = delete;
	Player& operator=(Player&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _Delta) override;

	void UserUpdate(float _DeltaTime);
	void NetUpdate(float _DeltaTime);

private:
	std::shared_ptr<class GameEngineComponent> Pivot;
	std::shared_ptr<class GameEngineFBXRenderer> TestRenderer;
	std::shared_ptr<GameEngineFBXRenderer> Renderer;
	std::shared_ptr<GameEngineRenderer> BoxRenderer;
};


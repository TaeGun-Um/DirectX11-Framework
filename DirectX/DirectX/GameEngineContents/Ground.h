#pragma once

// Ό³Έν :
class Ground : public GameEngineActor
{
public:
	// constrcuter destructer
	Ground();
	~Ground();

	// delete Function
	Ground(const Ground& _Other) = delete;
	Ground(Ground&& _Other) noexcept = delete;
	Ground& operator=(const Ground& _Other) = delete;
	Ground& operator=(Ground&& _Other) noexcept = delete;

protected:
	void Start() override;

private:

};


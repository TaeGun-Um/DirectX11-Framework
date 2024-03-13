#include "PrecompileHeader.h"
#include "TitleLevel.h"
#include "Player.h"

TitleLevel::TitleLevel() 
{
}

TitleLevel::~TitleLevel() 
{
}

void TitleLevel::Update(float _DeltaTime) 
{
	if (true == GameEngineInput::IsDown("LevelMoveKey"))
	{
		GameEngineCore::ChangeLevel("PlayLevel");
	}
}

void TitleLevel::Start() 
{
	if (false == GameEngineInput::IsKey("LevelMoveKey"))
	{
		GameEngineInput::CreateKey("LevelMoveKey", 'Y');
	}

	GetMainCamera()->SetProjectionType(CameraType::Perspective);
	GetMainCamera()->GetTransform()->SetLocalPosition({ 0, 0, -500.0f });

	{
		std::shared_ptr<GameEngineLight> Light = CreateActor<GameEngineLight>();
	}


	{
		std::shared_ptr<Player> NewPlayer = CreateActor<Player>();
		NewPlayer->SetUserControllType();
	}
}

void TitleLevel::LevelChangeStart() 
{

}
void TitleLevel::LevelChangeEnd()
{

}
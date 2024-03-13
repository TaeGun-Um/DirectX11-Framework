#include "PrecompileHeader.h"
#include "PlayLevel.h"
#include <GameEngineCore/GameEngineSpriteRenderer.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineCore/GameEngineCamera.h>
#include <GameEngineCore/GameEngineTexture.h>
#include <GameEngineCore/GameEngineVideo.h>
#include <GameEngineCore/GameEngineCoreWindow.h>
#include <GameEngineCore/GameEngineButton.h>
#include "FadeEffect.h"
#include <GameEngineCore/BloomEffect.h>
#include "Player.h"
#include "Ground.h"
#include "ServerWindow.h"
#include <GameEngineCore/EngineGrid.h>
#include <GameEngineCore/GameEngineParticleRenderer.h>

#include <GameEngineCore/GameEngineTexture.h>
#include <GameEngineCore/GameEngineSprite.h>

std::shared_ptr<TestObject> Object1 = nullptr;
std::shared_ptr<GameEngineSpriteRenderer> RenderTest = nullptr;

PlayLevel::PlayLevel() 
{
}

PlayLevel::~PlayLevel() 
{
}

void PlayLevel::Update(float _DeltaTime)
{
	if (true == GameEngineInput::IsDown("LevelMoveKey"))
	{
		GameEngineCore::ChangeLevel("TitleLevel");
	}

	if (nullptr != ServerWindow::NetInst)
	{
		ServerWindow::NetInst->UpdatePacket();
	}

}

void PlayLevel::Start()
{
	if (false == GameEngineInput::IsKey("LevelMoveKey"))
	{
		GameEngineInput::CreateKey("LevelMoveKey", 'Y');
	}

	// Col = std::make_shared<JPSCollision>();
	// GetMainCamera()->GetCamTarget()->DepthSettingOff();

	GetMainCamera()->SetProjectionType(CameraType::Perspective);
	GetMainCamera()->GetTransform()->SetLocalPosition({0, 200, -500.0f});
	// BloomEffectInst = GetMainCamera()->GetDeferredLightTarget()->CreateEffect<BloomEffect>();


	// GameEngineRenderTarget::Create();

	GameEngineCoreWindow::AddDebugRenderTarget(0, "AllRenderTarget", GetMainCamera()->GetCamAllRenderTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(1, "LightRenderTarget", GetMainCamera()->GetDeferredLightTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(2, "MainCameraForwardTarget", GetMainCamera()->GetCamForwardTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(3, "DeferredTarget", GetMainCamera()->GetCamDeferrdTarget());
	// GameEngineCoreWindow::AddDebugRenderTarget(4, "BloomEffect", BloomEffectInst->ResultTarget);
	//GameEngineCoreWindow::AddDebugRenderTarget(5, "BloomBlurEffect0", BloomEffectInst->BlurTarget0);
	//GameEngineCoreWindow::AddDebugRenderTarget(6, "BloomBlurEffect1", BloomEffectInst->BlurTarget1);
	GameEngineCoreWindow::AddDebugRenderTarget(8, "PostLight", GetMainCamera()->GetDeferredPostLightTarget());


	{
		{
			std::shared_ptr<GameEngineLight> Light = CreateActor<GameEngineLight>();
			Light->GetTransform()->AddLocalRotation(float4(90, 0.0f, 0.0f));
			Light->GetTransform()->AddLocalPosition(float4(0, 500.0f, 0.0f));
			Light->LightDataValue.DifLightPower = 1.0f;
			Light->LightDataValue.SpcPow = 5.0f;
			Light->LightDataValue.AmbientLight = float4::ZERO;
			GameEngineCoreWindow::AddDebugRenderTarget(7, "ShadowTarget1", Light->GetShadowTarget());
		}

		{
			std::shared_ptr<GameEngineLight> Light = CreateActor<GameEngineLight>();
			Light->GetTransform()->AddLocalRotation(float4(45, 0.0f, 0.0f));
			float4 FW = Light->GetTransform()->GetLocalForwardVector();
			Light->GetTransform()->AddLocalPosition(float4(0, 500.0f, -500.0f));
			Light->LightDataValue.DifLightPower = 1.0f;
			Light->LightDataValue.SpcPow = 5.0f;
			Light->LightDataValue.AmbientLight = float4::ZERO;
			GameEngineCoreWindow::AddDebugRenderTarget(8, "ShadowTarget2", Light->GetShadowTarget());
		}

	}


	std::shared_ptr<EngineGrid> Grid = CreateActor<EngineGrid>();

	{
		std::shared_ptr<Player> NewPlayer = CreateActor<Player>();
		NewPlayer->SetUserControllType();
	}

	{
		std::shared_ptr<GameEngineActor> TestP = CreateActor<GameEngineActor>();
		std::shared_ptr<GameEngineParticleRenderer> Particle = TestP->CreateComponent<GameEngineParticleRenderer>();

		Particle->SetTexture("Test.png");
		

	}

}
void PlayLevel::LevelChangeStart()
{
	GameEngineLevel::LevelChangeStart();
	int a = 0;
}

void PlayLevel::LevelChangeEnd() 
{
	GameEngineLevel::LevelChangeEnd();
	int a = 0;
}
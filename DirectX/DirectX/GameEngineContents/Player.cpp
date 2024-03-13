#include "PrecompileHeader.h"
#include "Player.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>
#include <GameEngineCore/GameEngineFontRenderer.h>
#include "ObjectUpdatePacket.h"

Player* Player::MainPlayer = nullptr;

Player::Player() 
{
	MainPlayer = this;
}

Player::~Player() 
{
}


void Player::Start()
{
	if (false == GameEngineInput::IsKey("MoveLeft"))
	{
		GameEngineInput::CreateKey("MoveLeft", 'A');
		GameEngineInput::CreateKey("MoveRight", 'D');
		GameEngineInput::CreateKey("MoveUp", 'Q');
		GameEngineInput::CreateKey("MoveDown", 'E');
		GameEngineInput::CreateKey("MoveForward", 'W');
		GameEngineInput::CreateKey("MoveBack", 'S');

		GameEngineInput::CreateKey("Rot", 'G');

	}

	//std::shared_ptr<GameEngineRenderer> Renderer = CreateComponent<GameEngineRenderer>();
	//Renderer->CreateRenderUnit("sphere", "MeshTexture");
	//Renderer->GetTransform()->SetLocalScale({100.0f, 100.0f, 100.0f});
	//Renderer->GetTransform()->SetLocalPosition({ 0.0f, 300.0f, 0.0f });
	//Renderer->GetShaderResHelper().SetTexture("DiffuseTexture", "BumpDif.png");
	//Renderer->GetShaderResHelper().SetTexture("NormalTexture", "BumpNormal.png");
	//Renderer->GetRenderBaseValueRef().IsNormal = 1;

	//{
	//	std::shared_ptr<GameEngineFBXRenderer> Renderer = CreateComponent<GameEngineFBXRenderer>();
	//	Renderer->SetFBXMesh("House1.FBX", "MeshTexture");
	//}


	//{
	//	std::shared_ptr<GameEngineFBXRenderer> Renderer = CreateComponent<GameEngineFBXRenderer>();
	//	Renderer->SetFBXMesh("Player_Artorias.FBX", "MeshAniTexture");

	//	GameEngineTime Time;
	//	Time.Reset();
	//	float Check0 = Time.TimeCheck();
	//	Renderer->CreateFBXAnimation("Run", "Dead.fbx");
	//	float Check1 = Time.TimeCheck();
	//	Renderer->ChangeAnimation("Run");
	//}

	//{
	//	std::shared_ptr<GameEngineFontRenderer> Renderer = CreateComponent<GameEngineFontRenderer>();
	//	Renderer->SetScale(100.0f);
	//	Renderer->SetFont("궁서");
	//	Renderer->SetText("aaaaaaaaaa");
	//}

	//Renderer->SetFBXMesh("AnimMan.FBX", "MeshAniTexture");
	//Renderer->CreateFBXAnimation("Run", "ALS_N_Run_F.FBX");
	//Renderer->ChangeAnimation("Run");


	// Renderer->SetFBXMesh("House1.FBX", "MeshTexture");
	// Renderer->SetFBXMesh("AnimMan.FBX", "MeshTexture", 0, 0);
	// Renderer->SetFBXMesh("AnimMan.FBX", "MeshTexture", 0, 2);

	if (false)
	{
		std::shared_ptr<GameEngineFBXRenderer> Renderer = CreateComponent<GameEngineFBXRenderer>();
		Renderer->SetFBXMesh("AnimMan.FBX", "MeshAniTextureDeferred");
		Renderer->GetTransform()->SetLocalPosition({ 0.0f, 50.0f, 0.0f });

		GameEngineTime Time;
		Time.Reset();
		float Check0 = Time.TimeCheck();
		Renderer->CreateFBXAnimation("Run", "ALS_N_Run_F.FBX", { 0.05f });
		float Check1 = Time.TimeCheck();
		Renderer->ChangeAnimation("Run");
		TestRenderer = Renderer;

		Renderer->ShadowOn();
	}



	if (true)
	{
		std::shared_ptr<GameEngineFBXRenderer> Renderer = CreateComponent<GameEngineFBXRenderer>();
		Renderer->SetFBXMesh("JUMPER_MESH.FBX", "MeshAniTextureDeferred");
		Renderer->GetTransform()->SetLocalPosition({ 0.0f, 200.0f, 0.0f });
		Renderer->GetTransform()->SetLocalScale({ 100.0f, 100.0f, 100.0f });

		GameEngineTime Time;
		Time.Reset();
		float Check0 = Time.TimeCheck();
		Renderer->CreateFBXAnimation("Run", "JUMPER_IDLE.FBX", { 0.05f });
		float Check1 = Time.TimeCheck();
		Renderer->ChangeAnimation("Run");

		TestRenderer = Renderer;

		Renderer->ShadowOn();
	}

	if (true)
	{
		// GetLevel()->CreateActor<>

		std::shared_ptr<GameEngineFBXRenderer> Renderer = CreateComponent<GameEngineFBXRenderer>();
		Renderer->SetFBXMesh("Boomerang.fbx", "MeshTextureAlpha");
		// Renderer->GetTransform()->SetLocalPosition({ 0.0f, 0.0f, 0.0f });
		Renderer->GetTransform()->SetLocalScale({ 200.0f, 200.0f, 200.0f });
		// Renderer->GetTransform()->SetLocalRotation({ 0.0f, 90.0f, 0.0f });
		TestRenderer->SetAttachTransform("weapon_l", Renderer->GetTransform(), float4::ZERO, float4(00.0f, 90.0f, 90.0f));


		//std::shared_ptr<GameEngineRenderer> Renderer = CreateComponent<GameEngineRenderer>();
		//std::shared_ptr<GameEngineRenderUnit> Unit = Renderer->CreateRenderUnit();

		//Renderer->GetTransform()->SetLocalScale({ 20, 20, 20 });

		//Renderer->GetRenderBaseValueRef().BaseColor = { 1.0f, 0.0f, 0.0f, 1.0f };
		//BoxRenderer = Renderer;
		//Unit->SetMesh("Boomerang.fbx");
		//Unit->SetMaterial("MeshTextureAlpha");
		//Unit->ShaderResHelper.SetTexture("DiffuseTexture", "FOGWAR.png");

		// TestRenderer->SetAttachTransform("hand_l", Renderer->GetTransform());

		// Renderer->ShadowOn();
	}



	//if (true)
	//{
	//	std::shared_ptr<GameEngineFBXRenderer> Renderer = CreateComponent<GameEngineFBXRenderer>();
	//	Renderer->SetFBXMesh("Player_Artorias.FBX", "MeshAniTexture");

	//	GameEngineTime Time;
	//	Time.Reset();
	//	float Check0 = Time.TimeCheck();
	//	Renderer->CreateFBXAnimation("Run", "Dead.fbx", { 0.05f });
	//	float Check1 = Time.TimeCheck();
	//	Renderer->ChangeAnimation("Run");

	//	Renderer->ShadowOn();
	//}

}

void Player::Update(float _DeltaTime)
{
	// 서버의 관리를 받는 오브젝트라면
	// 클라이언트의 입장에서는 
	// 상대의 패킷으로만 움직여야 한다.
	// 2가지로 나뉘게 된다.

	// TestRenderer->GetTransform()->AddLocalRotation({0.0f, 0.0f, _DeltaTime});


	// AnimationBoneData Data = TestRenderer->GetBoneData("hand_l");



	NetControllType Type = GetControllType();
	switch (Type)
	{
	case NetControllType::UserControll:
		UserUpdate(_DeltaTime);
		break;
	case NetControllType::NetControll:
		NetUpdate(_DeltaTime);
		break;
	default:
		break;
	}

}

void Player::NetUpdate(float _DeltaTime)
{
	if (false == IsPacket())
	{
		return;
	}

	while (IsPacket())
	{
		PacketEnum Type = GetFirstPacketType<PacketEnum>();

		switch (Type)
		{
		case PacketEnum::ObjectUpdatePacket:
		{
			std::shared_ptr<ObjectUpdatePacket> ObjectUpdate = PopFirstPacket<ObjectUpdatePacket>();
			GetTransform()->SetLocalPosition(ObjectUpdate->Position);
			GetTransform()->SetLocalRotation(ObjectUpdate->Rotation);
			break;
		}
		default:
			MsgAssert("처리하지 못하는 패킷이 플레이어로 날아왔습니다.");
			return;
		}
	}

}

void Player::UserUpdate(float _DeltaTime)
{
	// 직접 조작할때는 아래와 같이
	float Speed = 200.0f;

	if (true == GameEngineInput::IsPress("MoveLeft"))
	{
		GetTransform()->AddLocalPosition(GetTransform()->GetWorldLeftVector() * Speed * _DeltaTime);
	}
	if (true == GameEngineInput::IsPress("MoveRight"))
	{
		GetTransform()->AddLocalPosition(GetTransform()->GetWorldRightVector() * Speed * _DeltaTime);
	}
	if (true == GameEngineInput::IsPress("MoveUp"))
	{
		GetTransform()->AddLocalPosition(GetTransform()->GetWorldUpVector() * Speed * _DeltaTime);
	}
	if (true == GameEngineInput::IsPress("MoveDown"))
	{
		GetTransform()->AddLocalPosition(GetTransform()->GetWorldDownVector() * Speed * _DeltaTime);
	}
	if (true == GameEngineInput::IsPress("MoveForward"))
	{
		GetTransform()->AddLocalPosition(GetTransform()->GetWorldForwardVector() * Speed * _DeltaTime);
	}
	if (true == GameEngineInput::IsPress("MoveBack"))
	{
		GetTransform()->AddLocalPosition(GetTransform()->GetWorldBackVector() * Speed * _DeltaTime);
	}

	if (true == GameEngineInput::IsPress("Rot"))
	{
		GetTransform()->AddLocalRotation({ 0.0f, 360.0f * _DeltaTime, 0.0f });
	}

	static float Delta = 0.0f;

	Delta += _DeltaTime;

	if (Delta <= 1.0f / 60.0f)
	{
		return;
	}

	Delta -= 1.0f / 60.0f;
	
	if (true == IsNet())
	{
		std::shared_ptr<ObjectUpdatePacket> NewPacket = std::make_shared<ObjectUpdatePacket>();
		NewPacket->SetObjectID(GetNetObjectID());
		NewPacket->Position = GetTransform()->GetLocalPosition();
		NewPacket->Rotation = GetTransform()->GetLocalRotation();
		GetNet()->SendPacket(NewPacket);
	}
	
}
#include "PrecompileHeader.h"
#include "Ground.h"

Ground::Ground() 
{
}

Ground::~Ground() 
{
}

void Ground::Start()
{
	if (true)
	{
		std::shared_ptr<GameEngineRenderer> Renderer = CreateComponent<GameEngineRenderer>();
		std::shared_ptr<GameEngineRenderUnit> Unit = Renderer->CreateRenderUnit();

		Renderer->GetTransform()->AddLocalPosition({ 0, 25, 0 });
		Renderer->GetTransform()->SetLocalScale({2000, 50, 2000});

		Renderer->GetRenderBaseValueRef().BaseColor = { 0.7f, 0.2f, 0.0f, 1.0f};

		Unit->SetMesh("Box");
		Unit->SetMaterial("MeshColorDeferred");
	}
}
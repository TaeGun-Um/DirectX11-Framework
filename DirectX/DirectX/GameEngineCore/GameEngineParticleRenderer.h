#pragma once
#include "GameEngineRenderer.h"

struct ParticleInfo
{
	float4 StartScale;
	float4 EndScale;
	float4 StartColor;
	float4 EndColor;
};

struct ParticleData
{
	float4 vRelativePos;
	float4 vDir;

	float fMaxTime;
	float fCurTime;
	float fSpeed;
	UINT  iActive;
};

struct ParticleUpdateInfo 
{
	int PARTICLE_MAX = 1000;
	int IsWorldSpawn = 1;
	float SpawnRange = 300.0f;
	float MinSpeed = 0.0f;
	float MaxSpeed = 0.0f;
	float MinLife = 1.0f;
	float MaxLife = 5.0f;
	float Temp1;
	float4 ObjectWorldPos; // 액터나 랜더러의 월드 포지션을 

	//m_UpdateCS->SetWorldSpawn(m_WorldSpawn);
	//m_UpdateCS->SetObjectWorldPos(Transform()->GetWorldPos());
	//m_UpdateCS->SetSpawnRange(m_fSpawnRange);
	//m_UpdateCS->SetMinMaxSpeed(m_vMinMaxSpeed);
	//m_UpdateCS->SetMinMaxLifeTime(m_vMinMaxLifeTime);

};


struct ParticleComputeData
{
	// 현재 위치
	float4 vRelativePos;
	// 파티클의 방향
	float4 vDir;

	// 시간 사라지는시간
	float fMaxTime;
	// 현재 시간
	float fCurTime;
	// 이동할때의 스피드
	float fSpeed;
	// 살아있냐 죽어있냐.
	uint iActive;
};

struct ParticleShareData
{
	uint iAliveCount;
	uint iPadding0;
	uint iPadding1;
	uint iPadding2;
};


// 설명 :
class GameEngineParticleRenderer : public GameEngineRenderer
{
public:
	// constrcuter destructer
	GameEngineParticleRenderer();
	~GameEngineParticleRenderer();

	// delete Function
	GameEngineParticleRenderer(const GameEngineParticleRenderer& _Other) = delete;
	GameEngineParticleRenderer(GameEngineParticleRenderer&& _Other) noexcept = delete;
	GameEngineParticleRenderer& operator=(const GameEngineParticleRenderer& _Other) = delete;
	GameEngineParticleRenderer& operator=(GameEngineParticleRenderer&& _Other) noexcept = delete;

	void SetTexture(std::string_view _Name);

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	GameEngineComputeUnit ComUnit;
	ParticleUpdateInfo ParticleUpdateInfoValue;
	ParticleInfo ParticleInfoValue;

private:
	std::shared_ptr<class GameEngineVertexBuffer> Buffer;
	std::shared_ptr<class GameEngineStructuredBuffer> ParticleBuffer;
	std::shared_ptr<class GameEngineStructuredBuffer> ParticleShareBuffer;

	std::shared_ptr<GameEngineRenderUnit> ParticleUnit = nullptr;
	
	int MaxCount = 1000;
	float Frequency = 5.0f;
};


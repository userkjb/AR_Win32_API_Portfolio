#pragma once
#include <EngineCore/Actor.h>
#include "ContentsGlobalData.h"

class ATriScan : public AActor
{
public :
	// constrcuter destructer
	ATriScan();
	~ATriScan();

	// delete Function
	ATriScan(const ATriScan& _Other) = delete;
	ATriScan(ATriScan&& _Other) noexcept = delete;
	ATriScan& operator=(const ATriScan& _Other) = delete;
	ATriScan& operator=(ATriScan&& _Other) noexcept = delete;

	inline float SetAttackTime(float _Time)
	{
		AttackTime = _Time;
	}

	inline void SetTriScanState(ETriScanState _State)
	{
		State = _State;
	}

protected :
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private :
	void TriScanBeginPlay();

	void StateChange(ETriScanState _State);
	void StateUpdate(float _DeltaTime);
	
	void GetPlayerPos_Tick();

	void IdleStart();
	void Idle(float _DeltaTime);
	void RunStart();
	void Run(float _DeltaTime);
	void DeathStart();
	void Death(float _DeltaTime);


	void CalVector(float _DeltaTime);

	void CollisionCheck();


	ETriScanState State = ETriScanState::None;

	UImageRenderer* TriScanRender = nullptr;
	UCollision* TriScanCollision = nullptr;

	FVector RunVector = FVector::Zero;

	// Àû Ã£±â.
	float SearchTime = 0.0f;
	float AttackTime = 0.0f;

	FVector PlayerPos = FVector::Zero;
	FVector PlayerAttackPos = FVector::Zero;
	FVector PlayerDir = FVector::Zero;

	int Hp = 0;
};


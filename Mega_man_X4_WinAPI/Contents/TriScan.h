#pragma once
#include <EngineCore/Actor.h>
#include "ContentsGlobalData.h"

class ABuster;

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
	void BusterCollisionStart();
	void BusterCollision(float _DeltaTime);


	void MoveUpdate(float _DeltaTime);
	void CalGravityVector(float _DeltaTime);
	void CalLastMoveVector();
	void MoveLastMoveVector(float _DeltaTime);

	void CollisionCheck();


	ETriScanState State = ETriScanState::None;
	ETriScanState PreState = ETriScanState::None;

	UImageRenderer* TriScanRender = nullptr;
	UCollision* TriScanCollision = nullptr;
	UImageRenderer* FragmentsRender = nullptr;

	const float RunSpeed = 200.0f;
	FVector RunVector = FVector::Zero;
	const FVector GravityAcc = FVector::Down * 1000.0f;
	FVector GravityVector = FVector::Zero;

	FVector LastMoveVector = FVector::Zero;

	// 적 찾기.
	float SearchTime = 0.0f;
	float DeathTime = 0.0f;

	FVector PlayerPos = FVector::Zero;
	FVector PlayerAttackPos = FVector::Zero;
	FVector PlayerDir = FVector::Zero; // Player의 방향.

	//int CollisionCount = 0;
	int Hp = 3;

	ABuster* PrevBuster = nullptr;
};

/*
* 적 데이터
* https://megaman.fandom.com/wiki/List_of_Mega_Man_X4_enemies
*/
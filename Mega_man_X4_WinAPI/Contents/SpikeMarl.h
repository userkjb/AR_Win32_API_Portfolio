#pragma once
#include <EngineCore/Actor.h>
#include "ContentsGlobalData.h"

class ASpikeMarl : public AActor
{
public :
	// constrcuter destructer
	ASpikeMarl();
	~ASpikeMarl();

	// delete Function
	ASpikeMarl(const ASpikeMarl& _Other) = delete;
	ASpikeMarl(ASpikeMarl&& _Other) noexcept = delete;
	ASpikeMarl& operator=(const ASpikeMarl& _Other) = delete;
	ASpikeMarl& operator=(ASpikeMarl&& _Other) noexcept = delete;

	inline void SetSpikeMarlState(ESpikeMarlState _State)
	{
		State = _State;
	}

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private :
	void SpikeMarlBeginPlay();

	void StateChange(ESpikeMarlState _State);
	void StateUpdate(float _DeltaTime);

	void SummonStart();
	void Summon(float _DeltaTime);
	void IdleStart();
	void Idle(float _DeltaTime);
	void RunStart();
	void Run(float _DeltaTime);
	void AttackStart();
	void Attack(float _DeltaTime);
	void DeathStart();
	void Death(float _DeltaTime);

	void MoveUpdate(float _DeltaTime);
	void CalGravityVector(float _DeltaTime);
	void CalLastMoveVector();
	void MoveLastMoveVector(float _DeltaTime);

	void CollisionCheck(float _DeltaTime);

	ESpikeMarlState State = ESpikeMarlState::None;

	UImageRenderer* SpikeMarlRender = nullptr; // 본체
	UImageRenderer* SummonEffect = nullptr; // 소환 이펙트
	UCollision* SpikeMarlCollision = nullptr;
	UCollision* AttackCollision = nullptr;


	FVector RunVector = FVector::Zero;
	const FVector GravityAcc = FVector::Down * 1000.0f;
	FVector GravityVector = FVector::Zero;
	FVector LastMoveVector = FVector::Zero;

	int Hp = 6;
	int BodyDamage = 3;
	int RollDamage = 4;
};


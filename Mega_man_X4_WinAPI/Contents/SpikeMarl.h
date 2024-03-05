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

	// Level 에서 컨트롤 할 수 있도록 public
	//inline void SetActive(bool _bol)
	//{	SpikeMarlRender.ActiveOff	}
	
	// Collision 충돌할 때 가져가야 할 값에 대한 Get 함수
	inline int GetBodyDamage()
	{
		return BodyDamage;
	}
	inline int GetRollDamage()
	{
		return RollDamage;
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

	void MoveUpdate(float _DeltaTime, bool _Gravity = false);
	void CalGravityVector(float _DeltaTime, bool _Gravity);
	void CalLastMoveVector();
	void MoveLastMoveVector(float _DeltaTime);

	void CollisionCheck(float _DeltaTime);

	ESpikeMarlState State = ESpikeMarlState::None;

	UImageRenderer* SummonBG = nullptr;
	UImageRenderer* SpikeMarlRender = nullptr; // 본체
	UImageRenderer* SummonPosEffect = nullptr; // 소환 위치

	UCollision* SpikeMarlCollision = nullptr;
	UCollision* AttackCollision = nullptr;

	bool b_ActorLocation = false;
	FVector ActorPosFix = FVector::Zero;

	FVector RunVector = FVector::Zero;
	const FVector GravityAcc = FVector::Down * 1000.0f;
	FVector GravityVector = FVector::Zero;
	float SummonSpeed = 10.0f;

	FVector LastMoveVector = FVector::Zero;

	int Hp = 6;
	int BodyDamage = 3;
	int RollDamage = 4;
};


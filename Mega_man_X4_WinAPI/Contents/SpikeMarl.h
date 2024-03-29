#pragma once
#include <EngineCore/Actor.h>
#include "ContentsGlobalData.h"

class ABuster;

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
	inline int GetBodyDamage() const
	{
		return BodyDamage;
	}
	inline int GetRollDamage() const
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
	void SummonStart(float _DeltaTime);
	void SummonLoopStart();
	void SummonLoop(float _DeltaTime);
	void SummonEndStart();
	void SummonEnd(float _DeltaTime);
	void IdleStart();
	void Idle(float _DeltaTime);
	void RunStart();
	void Run(float _DeltaTime);
	void AttackReadyStart();
	void AttackReady(float _DeltaTime);
	void AttackStart();
	void Attack(float _DeltaTime);
	void DeathStart();
	void Death(float _DeltaTime);

	void CollisionCheck(float _DeltaTime);

	ESpikeMarlState State = ESpikeMarlState::None;
	EActorDir Dir = EActorDir::Left;

	UImageRenderer* SummonBGL = nullptr;
	UImageRenderer* SummonBGR = nullptr;
	UImageRenderer* SpikeMarlRender = nullptr; // 본체
	UImageRenderer* SummonPosEffect = nullptr; // 소환 위치

	UCollision* SpikeMarlCollision = nullptr;
	UCollision* AttackCollision = nullptr;

	FVector RunVector = FVector::Zero;
	FVector DownVector = FVector::Zero;
	float SummonSpeed = 50.0f;
	float DownSpeed = 200.0f;
	float AttackSpeed = 100.0f;
	float AttackLen = 200.0f; // 공격 길이
	FVector LastMoveVector = FVector::Zero;

	float SummonTime = 0.0f;
	float PlayerCheckTime = 0.0f;
	int AttackStartPos = 0;
	int AttackEndPos = 0;

	int Hp = 6;
	int BodyDamage = 3;
	int RollDamage = 4;

	ABuster* Prev = nullptr;
};


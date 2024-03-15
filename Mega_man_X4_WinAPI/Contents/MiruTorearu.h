#pragma once
#include <EngineCore/Actor.h>
#include "ContentsGlobalData.h"

//class ABuster;
class AEgseu;

class AMiruTorearu : public AActor
{
public:
	// constrcuter destructer
	AMiruTorearu();
	~AMiruTorearu();

	// delete Function
	AMiruTorearu(const AMiruTorearu& _Other) = delete;
	AMiruTorearu(AMiruTorearu&& _Other) noexcept = delete;
	AMiruTorearu& operator=(const AMiruTorearu& _Other) = delete;
	AMiruTorearu& operator=(AMiruTorearu&& _Other) noexcept = delete;

	inline void SetMiruTorearuState(EMiruTorearuState _State)
	{
		// Run �̳� Stop �� �ƴϸ� �׳� return.
		if (_State == EMiruTorearuState::StopCreate || _State == EMiruTorearuState::RunCreate)
		{
			State = _State;
		}
		else if (_State == EMiruTorearuState::Attack)
		{
			AttackStart();
			State = _State;
		}
		else
		{
			return;
		}
	}
	inline EMiruTorearuState GetMiruTorearuState()
	{
		return State;
	}

	void SetMiruDir(EActorDir _Dir)
	{
		MiruDir = _Dir;
	}

	void SetPlayer(AEgseu* _Set) { Player = _Set; }
protected :
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private :
	void BeginRender();
	void BeginCreateAnimation();
	std::string SetAnimationName(std::string _Name);

	void StateChange(EMiruTorearuState _State);
	void StateUpdate(float _DeltaTime);

	void None(float _DeltaTime);
	void StopCreate(float _DeltaTime);
	void RunCreate(float _DeltaTime);
	void StopStart();
	void Stop(float _DeltaTime);
	void RunStart();
	void Run(float _DeltaTime);
	void AttackStart();
	void Attack(float _DeltaTime);
	void DeathStartStart();
	void DeathStart(float _DeltaTime);
	void DeathLoopStart();
	void DeathLoop(float _DeltaTime);
	void DeathEndStart();
	void DeathEnd(float _DeltaTime);
	
	void CollisionCheck();

	EMiruTorearuState State = EMiruTorearuState::None;
	EActorDir MiruDir = EActorDir::Left;
	
	FVector RunVector = FVector::Zero;
	float RunSpeed = 200.0f;
	FVector PlayerVector = FVector::Zero;
	float PlayerMoveSpeed = 100.0f;

	UImageRenderer* MiruTorearuRender = nullptr; // ��ü �̹���
	UCollision* MiruTorearuCollision = nullptr; // ��ü �ݸ���
	UImageRenderer* DeathRender = nullptr;

	AEgseu* Player = nullptr;
};


#pragma once
#include <EngineCore/Actor.h>
#include "ContentsGlobalData.h"

class ABuster;

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
		else
		{
			return;
		}
	}
	EMiruTorearuState GetMiruTorearuState()
	{
		return State;
	}

protected :
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private :
	void BeginRender();
	void BeginCreateAnimation();

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
	

	EMiruTorearuState State = EMiruTorearuState::None;

	UImageRenderer* MiruTorearuRender = nullptr; // ��ü �̹���
	UCollision* MiruTorearuCollision = nullptr; // ��ü �ݸ���
};


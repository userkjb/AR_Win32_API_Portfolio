#pragma once
#include <EngineCore/Actor.h>
#include "ContentsGlobalData.h"

class ABuster;

class AEgseu : public AActor
{
public :
	// constrcuter destructer
	AEgseu();
	~AEgseu();

	// delete Function
	AEgseu(const AEgseu& _Other) = delete;
	AEgseu(AEgseu&& _Other) noexcept = delete;
	AEgseu& operator=(const AEgseu& _Other) = delete;
	AEgseu& operator=(AEgseu&& _Other) noexcept = delete;

	static AEgseu* GetMainPlayer();

	EActorDir GetActorDir() const
	{
		return DirState;
	}

protected :
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private :
	void ChargeBeginPlay();
	void PlayerBeginPlay();
	void DirCheck();
	std::string GetAnimationName(std::string _Name);
	void StateChange(EEgseuState _State);
	void StateUpdate(float _DeltaTime);

	//#pragma region Start Function
	void SummonStart();
	void Summon_LoopStart();
	void Summon_EndStart();
	void IdleStart();
	//void Idle_LoopStart();
	//void Idle_EndStart();
	void IdleJumpStart();
	void IdleJump_LoopStart();
	void IdleJump_EndStart();
	void JumpAttackStart();
	void JumpAttack_LoopStart();
	void JumpAttack_EndStart();
	void IdleAttackStart();
	void IdleAttack_LoopStart();
	void IdleAttack_EndStart();
	void IdleDashStart();
	void IdleDash_LoopStart();
	void IdleDash_EndStart();
	void DashAttackStart();
	void DashAttack_LoopStart();
	void DashAttack_EndStart();
	void IdleRunStart();
	void IdleRun_LoopStart();
	void IdleRun_EndStart();
	void RunAttackStart();
	void RunAttack_LoopStart();
	void RunAttack_EndStart();
	void RunDashStart();
	void RunDash_LoopStart();
	void RunDash_EndStart();
	void RunDashJumpStart();
	void RunDashJump_LoopStart();
	void RunDashJump_EndStart();
	void RunDashJumpAttackStart();
	void RunDashJumpAttack_LoopStart();
	void RunDashJumpAttack_EndStart();
	void RunJumpStart();
	void RunJump_LoopStart();
	void RunJump_EndStart();
	void RunJumpAttackStart();
	void RunJumpAttack_LoopStart();
	void RunJumpAttack_EndStart();
	//#pragma endregion


	//#pragma region Tick Function
	void Summon(float _DeltaTime);
	void Summon_Loop(float _DeltaTime);
	void Summon_End(float _DeltaTime);
	void Idle(float _DeltaTime);
	//void Idle_Loop(float _DeltaTime);
	//void Idle_End(float _DeltaTime);
	void IdleJump(float _DeltaTime);
	void IdleJump_Loop(float _DeltaTime);
	void IdleJump_End(float _DeltaTime);
	void JumpAttack(float _DeltaTime);
	void JumpAttack_Loop(float _DeltaTime);
	void JumpAttack_End(float _DeltaTime);
	void IdleAttack(float _DeltaTime);
	void IdleAttack_Loop(float _DeltaTime);
	void IdleAttack_End(float _DeltaTime);
	void IdleDash(float _DeltaTime);
	void IdleDash_Loop(float _DeltaTime);
	void IdleDash_End(float _DeltaTime);
	void DashAttack(float _DeltaTime);
	void DashAttack_Loop(float _DeltaTime);
	void DashAttack_End(float _DeltaTime);
	void IdleRun(float _DeltaTime);
	void IdleRun_Loop(float _DeltaTime);
	void IdleRun_End(float _DeltaTime);
	void RunAttack(float _DeltaTime);
	void RunAttack_Loop(float _DeltaTime);
	void RunAttack_End(float _DeltaTime);
	void RunDash(float _DeltaTime);
	void RunDash_Loop(float _DeltaTime);
	void RunDash_End(float _DeltaTime);
	void RunDashJump(float _DeltaTime);
	void RunDashJump_Loop(float _DeltaTime);
	void RunDashJump_End(float _DeltaTime);
	void RunDashJumpAttack(float _DeltaTime);
	void RunDashJumpAttack_Loop(float _DeltaTime);
	void RunDashJumpAttack_End(float _DeltaTime);
	void RunJump(float _DeltaTime);
	void RunJump_Loop(float _DeltaTime);
	void RunJump_End(float _DeltaTime);
	void RunJumpAttack(float _DeltaTime);
	void RunJumpAttack_Loop(float _DeltaTime);
	void RunJumpAttack_End(float _DeltaTime);
	//#pragma endregion

	// Vector Funcion
	
	void CalMoveVector();
	void CalGravityVector(float _DeltaTime);
 	void MoveLastMoveVector(float _DeltaTime);
	void CalLastMoveVector();
	void MoveUpdate(float _DeltaTime);


	// ===========================================================

	EEgseuState State = EEgseuState::None;
	ERenderOrder Order = ERenderOrder::None;
	EActorDir DirState = EActorDir::Right;

	std::string CurAnimationName = "None";

	UImageRenderer* PlayerRender = nullptr;
	UCollision* PlayerCollision = nullptr;
	UImageRenderer* Busterflash = nullptr; // 버스터 총구 섬광.
	UImageRenderer* MiddleChargeRender = nullptr; // 중간 차지
	UImageRenderer* PullChargeRender = nullptr; // 풀 차지

	//===== Vector ==============

	FVector RunVector = FVector::Zero;
	FVector DashVector = FVector::Zero;
	// 중력 가속도(500.0f)
	FVector GravityAcc = FVector::Down * 1000.0f;
	// 중력
	FVector GravityVector = FVector::Zero;
	// 점프 파워
	FVector JumpPower = FVector::Up * 450.0f;
	FVector JumpVector = FVector::Zero;
	// 소환 속도
	FVector SummonVector = FVector::Zero;
	// 모든 Vector들을 함한 Vector
	FVector LastMoveVector = FVector::Zero;

	//
	static AEgseu* MainPlayer;

	// 
	float MoveSpeed = 500.0f;
	float DashSpeed = 2.0f;
	float DashTime = 0.0f;
	float ChargTime = 0.0f;
	float AttackTime = 0.0f;
	float DelayTime = 0.0f;
	float ReadToBusterTime = 0.0f; // Buster 준비 시간.
	bool IsBuster = false; // Buster 쐈음?

	int TickCount = 0; // Buster가 1번 생성되도록.

	int MaxHp = 0;
	int Hp = 0;
};


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
	void ChangeAttackAnimation(std::string_view _str);
	void StateChange(EEgseuState _State);
	void StateUpdate(float _DeltaTime);

	//#pragma region Start Function
	void SummonStart();
	void Summon(float _DeltaTime);
	void Summon_LoopStart();
	void Summon_Loop(float _DeltaTime);	
	void Summon_EndStart();
	void Summon_End(float _DeltaTime);	
	void IdleStart();
	void Idle(float _DeltaTime);	
	//void Idle_LoopStart();
	//void Idle_Loop(float _DeltaTime);
	//void Idle_EndStart();
	//void Idle_End(float _DeltaTime);
	void IdleJumpStart();
	void IdleJump(float _DeltaTime);	
	void IdleJump_LoopStart();
	void IdleJump_Loop(float _DeltaTime);	
	void IdleJump_EndStart();
	void IdleJump_End(float _DeltaTime);	
	void JumpAttackStart();
	void JumpAttack(float _DeltaTime);	
	void JumpAttack_LoopStart();
	void JumpAttack_Loop(float _DeltaTime);	
	void JumpAttack_EndStart();
	void JumpAttack_End(float _DeltaTime);	
	void IdleAttack_DownStart();
	void IdleAttack_Down(float _DeltaTime);
	void IdleAttack_UpStart();
	void IdleAttack_Up(float _DeltaTime);
	void IdleAttack_Down_LoopStart();
	void IdleAttack_Down_Loop(float _DeltaTime);	
	void IdleAttack_Up_LoopStart();
	void IdleAttack_Up_Loop(float _DeltaTime);
	void IdleAttack_Down_EndStart();
	void IdleAttack_Down_End(float _DeltaTime);	
	void IdleAttack_Up_EndStart();
	void IdleAttack_Up_End(float _DeltaTime);
	void IdleDashStart();
	void IdleDash(float _DeltaTime);	
	void IdleDash_LoopStart();
	void IdleDash_Loop(float _DeltaTime);	
	void IdleDash_EndStart();
	void IdleDash_End(float _DeltaTime);	
	void DashAttackStart();
	void DashAttack(float _DeltaTime);	
	void DashAttack_LoopStart();
	void DashAttack_Loop(float _DeltaTime);	
	void DashAttack_EndStart();
	void DashAttack_End(float _DeltaTime);	
	void IdleRunStart();
	void IdleRun(float _DeltaTime);
	void IdleRun_LoopStart();
	void IdleRun_Loop(float _DeltaTime);
	void IdleRun_EndStart();
	void IdleRun_End(float _DeltaTime);

	void RunAttack_DownStart();
	void RunAttack_Down(float _DeltaTime);
	void RunAttack_Down_LoopStart();
	void RunAttack_Down_Loop(float _DeltaTime);
	void RunAttack_Down_EndStart();
	void RunAttack_Down_End(float _DeltaTime);
	void RunAttack_UpStart();
	void RunAttack_Up(float _DeltaTime);
	void RunAttack_Up_LoopStart();
	void RunAttack_Up_Loop(float _DeltaTime);
	void RunAttack_Up_EndStart();
	void RunAttack_Up_End(float _DeltaTime);

	void RunDashStart();
	void RunDash(float _DeltaTime);
	void RunDash_LoopStart();
	void RunDash_Loop(float _DeltaTime);
	void RunDash_EndStart();
	void RunDash_End(float _DeltaTime);
	void RunDashJumpStart();
	void RunDashJump(float _DeltaTime);
	void RunDashJump_LoopStart();
	void RunDashJump_Loop(float _DeltaTime);
	void RunDashJump_EndStart();
	void RunDashJump_End(float _DeltaTime);
	void RunDashJumpAttackStart();
	void RunDashJumpAttack(float _DeltaTime);
	void RunDashJumpAttack_LoopStart();
	void RunDashJumpAttack_Loop(float _DeltaTime);
	void RunDashJumpAttack_EndStart();
	void RunDashJumpAttack_End(float _DeltaTime);
	void RunJumpStart();
	void RunJump(float _DeltaTime);
	void RunJump_LoopStart();
	void RunJump_Loop(float _DeltaTime);
	void RunJump_EndStart();
	void RunJump_End(float _DeltaTime);
	void RunJumpAttackStart();
	void RunJumpAttack(float _DeltaTime);
	void RunJumpAttack_LoopStart();
	void RunJumpAttack_Loop(float _DeltaTime);
	void RunJumpAttack_EndStart();
	void RunJumpAttack_End(float _DeltaTime);
	void WallClingStart();
	void WallCling(float _DeltaTime);
	void WallCling_LoopStart();
	void WallCling_Loop(float _DeltaTime);
	void WallKickStart();
	void WallKick(float _DeltaTime);
	void HitStart();
	void Hit(float _DeltaTime);
	//#pragma endregion


	// Vector Funcion
	
	void CalMoveVector();
	void CalGravityVector(float _DeltaTime);
 	void MoveLastMoveVector(float _DeltaTime);
	void CalLastMoveVector();
	void MoveUpdate(float _DeltaTime);

	/// <summary>
	/// �浹 üũ
	/// </summary>
	void CollisionCheck(float _DeltaTime);
	
	/// <summary>
	/// ��� ���� ���¿��� ����ϴ� �Լ�.
	/// �����鿡 ������ ����� ���·� ��ȯ.
	/// </summary>
	bool CalWallCheck();

	// Buster Charge Time Function
	void BusterChargeTime(float _DeltaTime);

	void BusterCreate(EBusterState _BusterState);

	// ===========================================================

	EEgseuState State = EEgseuState::None;
	ERenderOrder Order = ERenderOrder::None;
	EActorDir DirState = EActorDir::Right;

	std::string CurAnimationName = "None";

	UImageRenderer* PlayerRender = nullptr;
	UCollision* PlayerCollision = nullptr;
	UImageRenderer* Busterflash = nullptr; // ������ �ѱ� ����.
	UImageRenderer* MiddleChargeRender = nullptr; // �߰� ����
	UImageRenderer* PullChargeRender = nullptr; // Ǯ ����

	//===== Vector ==============

	FVector RunVector = FVector::Zero;
	FVector DashVector = FVector::Zero;
	// �߷� ���ӵ�(500.0f)
	const FVector GravityAcc = FVector::Down * 1000.0f;
	// �߷�
	FVector GravityVector = FVector::Zero;
	// ���� �Ŀ�
	const FVector JumpPower = FVector::Up * 450.0f;
	FVector JumpVector = FVector::Zero;
	// ��ȯ �ӵ�
	FVector SummonVector = FVector::Zero;
	// ��� Vector���� ���� Vector
	FVector LastMoveVector = FVector::Zero;

	// �뽬�� ���� �ִ� �ӵ�
	FVector MaxSpeedVector = { 1000.0f, 1000.0f };

	// �ۿ��� ����� Player�� static ���� ��.
	static AEgseu* MainPlayer;

	// 
	bool IsBuster = false; // Buster ����?
	float MoveSpeed = 500.0f;
	float DashSpeed = 750.0f;
	float DashTime = 0.0f;
	float SummonDelayTime = 0.0f;
	float ReadToBusterTime = 0.0f; // Buster �غ� �ð�.
	float WallKickTime = 0.0f; // �� ���� �ൿ�� �����ϱ� ���� Time
	float Hit_InvincibilityTime = 0.0f; // ���� �ð�.

	float BusterChargTime = 0.0f;
	float BusterDelayTime = 0.0f;
	const float BusterDelayTimeMax = 0.5f;
	int BusterTickCount = 0; // Buster�� 1�� �����ǵ���.
	int ChangeAnimationFrame = 0;
	int Hit_Count = 0; // Hit �ߺ� ����.

	int MaxHp = 48;
	int Hp = 32; // ��Ʈ��ũ + 2
};


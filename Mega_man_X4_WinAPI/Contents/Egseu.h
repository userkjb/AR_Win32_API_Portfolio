#pragma once
#include <EngineCore/Actor.h>
#include "ContentsGlobalData.h"
#include <EnginePlatform/EngineSound.h>

class ABuster;
class ACyberSpaceMap;
class ACyberSpaceBossMap;
class AMiruTorearu;

class AEgseu : public AActor
{
	friend ACyberSpaceMap;
	friend ACyberSpaceBossMap;
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

	int GetOpDir() const
	{
		return OpDir;
	}

	/// <summary>
	/// Level에서 접근.
	/// </summary>
	/// <param name="_State"></param>
	void SetStateChange(EEgseuState _State)
	{
		StateChange(_State);
	}

	EEgseuState GetPlayerState() const
	{
		return State;
	}

	inline int GetPlayerHp() const
	{
		return Hp;
	}
	inline void SetPlayerHp(int _Value)
	{
		Hp -= _Value;
	}

	inline bool GetReSummonEnd() const
	{
		return ReSummonEnd;
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
	void WaitStart();
	void Wait(float _DeltaTime);
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
	void JumpAttack_DownStart();
	void JumpAttack_Down(float _DeltaTime);	
	void JumpAttack_Down_LoopStart();
	void JumpAttack_Down_Loop(float _DeltaTime);	
	void JumpAttack_Down_EndStart();
	void JumpAttack_Down_End(float _DeltaTime);
	void JumpAttack_UpStart();
	void JumpAttack_Up(float _DeltaTime);
	void JumpAttack_Up_LoopStart();
	void JumpAttack_Up_Loop(float _DeltaTime);
	void JumpAttack_Up_EndStart();
	void JumpAttack_Up_End(float _DeltaTime);
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
	//void IdleRun_EndStart();
	//void IdleRun_End(float _DeltaTime);

	void RunAttack_DownStart();
	void RunAttack_Down(float _DeltaTime);
	void RunAttack_Down_LoopStart();
	void RunAttack_Down_Loop(float _DeltaTime);
	//void RunAttack_Down_EndStart();
	//void RunAttack_Down_End(float _DeltaTime);
	void RunAttack_UpStart();
	void RunAttack_Up(float _DeltaTime);
	void RunAttack_Up_LoopStart();
	void RunAttack_Up_Loop(float _DeltaTime);
	//void RunAttack_Up_EndStart();
	//void RunAttack_Up_End(float _DeltaTime);

	void RunDashStart();
	void RunDash(float _DeltaTime);
	void RunDash_LoopStart();
	void RunDash_Loop(float _DeltaTime);
	void RunDash_EndStart();
	void RunDash_End(float _DeltaTime);

	void RunDashAttack_DownStart();
	void RunDashAttack_Down(float _DeltaTime);
	void RunDashAttack_Down_LoopStart();
	void RunDashAttack_Down_Loop(float _DeltaTime);
	void RunDashAttack_Down_EndStart();
	void RunDashAttack_Down_End(float _DeltaTime);
	void RunDashAttack_UpStart();
	void RunDashAttack_Up(float _DeltaTime);
	void RunDashAttack_Up_LoopStart();
	void RunDashAttack_Up_Loop(float _DeltaTime);
	void RunDashAttack_Up_EndStart();
	void RunDashAttack_Up_End(float _DeltaTime);

	void RunDashJumpStart();
	void RunDashJump(float _DeltaTime);
	void RunDashJump_LoopStart();
	void RunDashJump_Loop(float _DeltaTime);
	void RunDashJump_EndStart();
	void RunDashJump_End(float _DeltaTime);

	void RunDashJumpAttack_DownStart();
	void RunDashJumpAttack_Down(float _DeltaTime);
	void RunDashJumpAttack_Down_LoopStart();
	void RunDashJumpAttack_Down_Loop(float _DeltaTime);
	void RunDashJumpAttack_Down_EndStart();
	void RunDashJumpAttack_Down_End(float _DeltaTime);
	void RunDashJumpAttack_UpStart();
	void RunDashJumpAttack_Up(float _DeltaTime);
	void RunDashJumpAttack_Up_LoopStart();
	void RunDashJumpAttack_Up_Loop(float _DeltaTime);
	void RunDashJumpAttack_Up_EndStart();
	void RunDashJumpAttack_Up_End(float _DeltaTime);
	void RunJumpStart();
	void RunJump(float _DeltaTime);
	void RunJump_LoopStart();
	void RunJump_Loop(float _DeltaTime);
	void RunJump_EndStart();
	void RunJump_End(float _DeltaTime);
	void RunJumpAttack_DownStart();
	void RunJumpAttack_Down(float _DeltaTime);
	void RunJumpAttack_Down_LoopStart();
	void RunJumpAttack_Down_Loop(float _DeltaTime);
	void RunJumpAttack_Down_EndStart();
	void RunJumpAttack_Down_End(float _DeltaTime);
	void RunJumpAttack_UpStart();
	void RunJumpAttack_Up(float _DeltaTime);
	void RunJumpAttack_Up_LoopStart();
	void RunJumpAttack_Up_Loop(float _DeltaTime);
	void RunJumpAttack_Up_EndStart();
	void RunJumpAttack_Up_End(float _DeltaTime);
	void WallClingStart();
	void WallCling(float _DeltaTime);
	void WallCling_LoopStart();
	void WallCling_Loop(float _DeltaTime);

	void WallClingAttack_DownStart();
	void WallClingAttack_Down(float _DeltaTime);
	void WallClingAttack_Down_LoopStart();
	void WallClingAttack_Down_Loop(float _DeltaTime);
	void WallClingAttack_UpStart();
	void WallClingAttack_Up(float _DeltaTime);
	void WallClingAttack_Up_LoopStart();
	void WallClingAttack_Up_Loop(float _DeltaTime);

	void WallKickStart();
	void WallKick(float _DeltaTime);

	void WallKickAttack_DownStart();
	void WallKickAttack_Down(float _DeltaTime);
	void WallKickAttack_UpStart();
	void WallKickAttack_Up(float _DeltaTime);

	void HitStart();
	void Hit(float _DeltaTime);
	void Hit_MiruTorearuStart();
	void Hit_MiruTorearu(float _DeltaTime);
	void FocusCreateStart();
	void FocusCreate(float _DeltaTime);
	void FocusLoopStart();
	void FocusLoop(float _DeltaTime);
	void FocusEndStart();
	void FocusEnd(float _DeltaTime);
	void AutoRunRightStart();
	void AutoRunRight(float _DeltaTime);

	void BossRoomAutoRunStart();
	void BossRoomAutoRun(float _DeltaTime);
	void VictoryStart();
	void Victory(float _DeltaTime);
	void ReSummonStart();
	void ReSummon(float _DeltaTime);
	void ReSummon_UpStart();
	void ReSummon_Up(float _DeltaTime);
	//#pragma endregion


	// Vector Funcion
	void CalMoveVector();
	void CalGravityVector(float _DeltaTime);
 	void MoveLastMoveVector(float _DeltaTime);
	void CalLastMoveVector();
	void MoveUpdate(float _DeltaTime);

	/// <summary>
	/// 충돌 체크
	/// </summary>
	void CollisionCheck(float _DeltaTime);
	
	/// <summary>
	/// 모든 공중 상태에서 사용하는 함수.
	/// 벽옆면에 닿으면 벽잡기 상태로 전환.
	/// </summary>
	bool CalWallCheck();

	// Buster Charge Time Function
	void BusterChargeTime(float _DeltaTime);

	/// <summary>
	/// Buster 생성.
	/// </summary>
	/// <param name="_BusterState">Buster 종류</param>
	void WallBusterCreate(EBusterState _BusterState);

	/// <summary>
	/// 버스터 생성 함수.
	/// </summary>
	/// <param name="_BusterState">생성할 버스터 상태.</param>
	/// <param name="_Pos">생성되는 위치.</param>
	void BusterCreate(EBusterState _BusterState, FVector _Pos);

	void InvincibleTime(float _DeltaTime);

	// ===========================================================

	EEgseuState State = EEgseuState::None;
	EEgseuState PreState = EEgseuState::None;
	ERenderOrder RenderOrder = ERenderOrder::None;
	EActorDir DirState = EActorDir::Right;

	UImageRenderer* PlayerRender = nullptr;
	UCollision* PlayerCollision = nullptr;
	//UImageRenderer* Busterflash = nullptr; // 버스터 총구 섬광.
	UImageRenderer* MiddleChargeRender = nullptr; // 중간 차지
	UImageRenderer* PullChargeRender = nullptr; // 풀 차지

	AMiruTorearu* PreMiru = nullptr;
	AMiruTorearu* UseMiru = nullptr;

	// Sound
	UEngineSoundPlayer Buster_Charge_Sound;
	UEngineSoundPlayer Buster_Chargeing_Sound;
	float BusterSountTime = 0.0f;
	bool Sound_1 = false;
	bool Sound_2 = false;

	//===== Vector ==============

	FVector RunVector = FVector::Zero;
	FVector DashVector = FVector::Zero;
	// 중력 가속도(500.0f)
	const FVector GravityAcc = FVector::Down * 1000.0f;
	// 중력
	FVector GravityVector = FVector::Zero;
	// 점프 파워
	const FVector JumpPower = FVector::Up * 450.0f;
	FVector JumpVector = FVector::Zero;
	// 소환 속도
	FVector SummonVector = FVector::Zero;
	// 모든 Vector들을 함한 Vector
	FVector LastMoveVector = FVector::Zero;

	// 대쉬랑 런의 최대 속도
	FVector MaxSpeedVector = { 1000.0f, 1000.0f };
	
	// Cling Vector
	FVector ClingVector = FVector::Zero;
	const FVector ClingPower = FVector::Down * 200.0f;

	// 버스터 발싸 위치.
	FVector Idle_Muzzle = { 30.0f, 79.0f };
	FVector Run_Muzzle = { 35.0f, 79.0f };
	FVector Dash_Muzzle = { 55.0f, 45.0f };
	FVector Jump_Muzzle = { 35.0f, 95.0f };


	// 밖에서 사용할 Player를 static 으로 뺌.
	static AEgseu* MainPlayer;

	std::string CurAnimationName = "None";
	int Debug_Num = 0;

	bool AutoRightRun = false;
	bool ReSummonEnd = false;
	bool b_IsHit = false;
	const float MoveSpeed = 500.0f;
	const float DashSpeed = 750.0f;
	float DashTime = 0.0f;
	const float DashTimeMax = 0.5f;
	float SummonDelayTime = 0.0f;
	float ReadToBusterTime = 0.0f; // Buster 준비 시간.
	float WallKickTime = 0.0f; // 벽 차는 행동을 강제하기 위한 Time
	float HitDelayTime = 0.0f;
	float Hit_InvincibilityTime = 0.0f; // 무적 시간.
	float VictoryTime = 0.0f; // 승리 포즈 유지 시간.
	float ReSummon_UpTime = 0.0f; // 위로 올라가는 시간.

	float BusterChargTime = 0.0f;
	float BusterDelayTime = 0.0f;
	const float BusterDelayTimeMax = 0.5f;
	int BusterTickCount = 0; // Buster가 1번 생성되도록.
	int ChangeAnimationFrame = 0;
	int Hit_Count = 0; // Hit 중복 방지.
	int OpDir = 0;

	int MaxHp = 48;
	int Hp = 32; // 하트탱크 + 2
};


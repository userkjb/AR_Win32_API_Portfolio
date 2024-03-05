#pragma once

#include <EngineBase/EngineMath.h>
#include <EngineCore/EngineCore.h>
#include <EngineCore/ImageRenderer.h>

/// <summary>
/// Player 상태. [FSM]
/// </summary>
enum class EPlayerState
{
	None,

	Summon,			// 소환 이동.(레이저로 내려옴.)
	SummonLoop,		// 소환 중.(땅에 닿으면 Character로 변화.)
	SummonEnd,		// 소환 종료.(소환 완료.)

	Comeback,		// 복귀.(클리어 포즈.)
	ComebackLeep,	// 복귀 준비.(레이저로 변화.)
	ComebackEnd,	// 복귀(레이저가 위로 이동.)

	Idle,			// 기본.
	Idle_Attack,	// 기본 자세 공격 시작.
	Idle_AttackLoop,// 기본 자세 공격 루프.
	Idle_AttackEnd, // 기본 자세 공격 종료.

	Run,			// 움직이기.
	Run_Attack,		// 움직임 자세 공격 시작.
	Run_AttackLoop, // 움직임 자세 공격 루프.
	Run_AttackEnd,	//

	Jump,			// 점프.
	JumpLoop,		// 공중.
	JumpEnd,		// 착지.
	Jump_Attack,	//
	Jump_AttackLoop,//
	Jump_AttackEnd, //

	DashStart,		// 대쉬 시작.
	DashLoop,		// 대쉬 중.
	DashEnd,		// 대쉬 종료.
	Dash_Attack,	//
	Dash_AttackLoop,//
	Dash_AttackEnd,	//

	//Hit,
};

enum class EEgseuState
{
	None,

	Summon,			// 소환 이동.(레이저로 내려옴.)
	Summon_Loop,	// 소환 중.(땅에 닿으면 Character로 변화.)
	Summon_End,		// 소환 종료.(소환 완료.)

	Idle,			// 기본 상태.
	//Idle_Loop,
	//Idle_End,

	IdleJump,		// 기본 상태에서 점프.
	IdleJump_Loop,	// 채공 중.
	IdleJump_End,	// 착지.

	JumpAttack,
	JumpAttack_Loop,
	JumpAttack_End, // 10

	IdleAttack,
	IdleAttack_Loop,
	IdleAttack_End,

	IdleDash,
	IdleDash_Loop,
	IdleDash_End,

	DashAttack,
	DashAttack_Loop,
	DashAttack_End,

	IdleRun,
	IdleRun_Loop, // 21
	IdleRun_End,

	RunAttack, // 23
	RunAttack_Loop, //24
	RunAttack_End,

	RunDash,
	RunDash_Loop,
	RunDash_End,

	RunDashJump,
	RunDashJump_Loop,
	RunDashJump_End,

	RunDashJumpAttack,
	RunDashJumpAttack_Loop,
	RunDashJumpAttack_End,

	RunJump,
	RunJump_Loop,
	RunJump_End,

	RunJumpAttack,
	RunJumpAttack_Loop,
	RunJumpAttack_End,

	WallCling,
	WallCling_Loop,
	
	WallKick,

	Hit,
};

/// <summary>
/// X Buster 상태
/// </summary>
enum class EBusterState
{
	CreateBuster,
	DefaultCharge,	// 기본.
	MiddleCharge,	// 중 차지.
	PullCharge,		// 풀 차지.
	BusterCrash,	// 충돌.
	BusterEnd,		// 종료.
};

enum class EFocusState
{
	None,

	Create,

	B,
	A,
	S,

	Death,
};

/// <summary>
/// Boss CyberPeacock State
/// </summary>
enum class ECyberPeacockState
{
	None,

	Intro,
	IntroEnd,

	Disappear,		// 사라짐.
	Appear,			// 나타남.

	FeatherAttack,
	RisingSlash,
	TrackingShot,

	Death,
};

/// <summary>
/// Enemy TriScans 상태.
/// </summary>
enum class ETriScanState
{
	None,

	Idle,
	Run,
	Death,
};

/// <summary>
/// Enemy SpikeMarl 상태.
/// </summary>
enum class ESpikeMarlState
{
	None,

	Summon,
	Idle,
	Run,
	Attack,
	Death,
};

/// <summary>
/// Actor의 방향.
/// </summary>
enum class EActorDir
{
	Left,
	Right,
};

/// <summary>
/// 렌더 순서.
/// </summary>
enum class ERenderOrder
{
	None,
	Map,
	Player,
	MapObject,
	Buster,
	EnemyPrev,
	Enemy,
	EnemyNext,
	Boss,
};

/// <summary>
/// 콜리전 순서.
/// </summary>
enum class ECollisionOrder
{
	Player,
	Weapon,

	Boss,
	Enemy,
};

class UContentsGlobalData
{
public :
	static UWindowImage* ColMapImage;

	static FVector GetMousePos();

private :
	UContentsGlobalData();
	~UContentsGlobalData();
};


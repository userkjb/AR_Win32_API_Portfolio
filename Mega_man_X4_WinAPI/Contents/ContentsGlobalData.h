#pragma once

#include <EngineBase/EngineMath.h>
#include <EngineCore/EngineCore.h>
#include <EngineCore/ImageRenderer.h>

enum class EActorType
{
	None,
	Map,
	MapObject,
	Player,
	Buster,
	Boss,
	Enemy,
};

/// <summary>
/// Player ����. [FSM]
/// </summary>
enum class EPlayerState
{
	None,

	Summon,			// ��ȯ �̵�.(�������� ������.)
	SummonLoop,		// ��ȯ ��.(���� ������ Character�� ��ȭ.)
	SummonEnd,		// ��ȯ ����.(��ȯ �Ϸ�.)

	Comeback,		// ����.(Ŭ���� ����.)
	ComebackLeep,	// ���� �غ�.(�������� ��ȭ.)
	ComebackEnd,	// ����(�������� ���� �̵�.)

	Idle,			// �⺻.
	Idle_Attack,	// �⺻ �ڼ� ���� ����.
	Idle_AttackLoop,// �⺻ �ڼ� ���� ����.
	Idle_AttackEnd, // �⺻ �ڼ� ���� ����.

	Run,			// �����̱�.
	Run_Attack,		// ������ �ڼ� ���� ����.
	Run_AttackLoop, // ������ �ڼ� ���� ����.
	Run_AttackEnd,	//

	Jump,			// ����.
	JumpLoop,		// ����.
	JumpEnd,		// ����.
	Jump_Attack,	//
	Jump_AttackLoop,//
	Jump_AttackEnd, //

	DashStart,		// �뽬 ����.
	DashLoop,		// �뽬 ��.
	DashEnd,		// �뽬 ����.
	Dash_Attack,	//
	Dash_AttackLoop,//
	Dash_AttackEnd,	//

	//Hit,
};

/// <summary>
/// Attack�� X ��ư�� ������ �� �ش� ���¿���
/// Attack�� ���� ���·� �Ѿ�� ����.
/// </summary>
enum class EEgseuState
{
	None,

	Summon,			// ��ȯ �̵�.(�������� ������.)
	Summon_Loop,	// ��ȯ ��.(���� ������ Character�� ��ȭ.)
	Summon_End,		// ��ȯ ����.(��ȯ �Ϸ�.)

	Idle,			// �⺻ ����.
	//Idle_Loop,
	//Idle_End,

	IdleJump,		// �⺻ ���¿��� ����.
	IdleJump_Loop,	// ä�� ��.
	IdleJump_End,	// ����.

	JumpAttack_Down,
	JumpAttack_Down_Loop,
	JumpAttack_Down_End,
	JumpAttack_Up,
	JumpAttack_Up_Loop,
	JumpAttack_Up_End,

	IdleAttack_Down,
	IdleAttack_Down_Loop,
	IdleAttack_Down_End,
	IdleAttack_Up,
	IdleAttack_Up_Loop,
	IdleAttack_Up_End,

	IdleDash,
	IdleDash_Loop,
	IdleDash_End,

	// Idle -> Dash / ������ ����.
	DashAttack,
	DashAttack_Loop,
	DashAttack_End,

	IdleRun,
	IdleRun_Loop,
	//IdleRun_End, // �׸��� ����.

	// Idle -> Run
	RunAttack_Down, // �޸��鼭 ���� ����.
	RunAttack_Down_Loop, // �޸��鼭 ����.
	//RunAttack_Down_End,
	RunAttack_Up,
	RunAttack_Up_Loop,
	//RunAttack_Up_End,

	// Run -> Dash
	RunDash,
	RunDash_Loop,
	RunDash_End,

	RunDashAttack_Down,
	RunDashAttack_Down_Loop,
	RunDashAttack_Down_End,
	RunDashAttack_Up,
	RunDashAttack_Up_Loop,
	RunDashAttack_Up_End,

	RunDashJump,
	RunDashJump_Loop,
	RunDashJump_End,

	RunDashJumpAttack_Down,
	RunDashJumpAttack_Down_Loop,
	RunDashJumpAttack_Down_End,
	RunDashJumpAttack_Up,
	RunDashJumpAttack_Up_Loop,
	RunDashJumpAttack_Up_End,

	RunJump,
	RunJump_Loop,
	RunJump_End,

	RunJumpAttack_Down,
	RunJumpAttack_Down_Loop,
	RunJumpAttack_Down_End,
	RunJumpAttack_Up,
	RunJumpAttack_Up_Loop,
	RunJumpAttack_Up_End,

	WallCling,
	WallCling_Loop,
	
	WallKick,

	Hit,
	Hit_MiruTorearu,

	FocusCreate,
	FocusLoop,
	FocusEnd,

	AutoRunRight,
};

/// <summary>
/// X Buster ����
/// </summary>
enum class EBusterState
{
	CreateBuster,

	CreateDefault,
	CreateMiddle,
	CreatePull,

	DefaultCharge,	// �⺻.
	MiddleCharge,	// �� ����.
	PullCharge,		// Ǯ ����.
	BusterCrash,	// �浹.
	BusterEnd,		// ����.
};

/// <summary>
/// ���� �� ����.
/// </summary>
enum class EBossDoorState
{
	None,
	Create,
	Idle,
	Open,
	Close,
};

enum class EFocusState
{
	None,

	CallCreate,
	Create,
	CreateLoop,
	CallRun,
	Run,
	Rank,
	RunUp,
	RunRight,
	Death,

	B,
	A,
	S,
};

/// <summary>
/// Boss CyberPeacock State
/// </summary>
enum class ECyberPeacockState
{
	None,

	Intro,
	IntroEnd,

	Disappear,		// �����.
	Appear,			// ��Ÿ��.

	FeatherAttack,
	RisingSlash,
	TrackingShot,

	Death,
};

enum class ECyberSpaceMapState
{
	None,

	PlayerFocus_Start,
	PlayerFocus_Loop,
	PlayerFocus_End,
};

/// <summary>
/// Enemy TriScans ����.
/// </summary>
enum class ETriScanState
{
	None,

	Idle,
	Run,
	Death,

	BusterCollision,
};

enum class EMiruTorearuState
{
	None,

	StopCreate,
	RunCreate,
	Stop,
	Run,

	Attack, // Collision Player
	DeathStart, // Ŀ���� �۾���.
	DeathLoop, // �����.
	DeathEnd, // �Ҹ�.
};

/// <summary>
/// Enemy SpikeMarl ����.
/// </summary>
enum class ESpikeMarlState
{
	None,

	SummonStart,
	SummonLoop,
	SummonEnd,
	Idle,
	Run,
	AttackReady,
	Attack,
	Death,
};

/// <summary>
/// Actor�� ����.
/// </summary>
enum class EActorDir
{
	Left,
	Right,
};

/// <summary>
/// ���� ����.
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
	EnemyEffect,
	Boss,
};

/// <summary>
/// �ݸ��� ����.
/// </summary>
enum class ECollisionOrder
{
	Player,
	Weapon,

	Boss,
	Enemy,

	BossDoor,
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


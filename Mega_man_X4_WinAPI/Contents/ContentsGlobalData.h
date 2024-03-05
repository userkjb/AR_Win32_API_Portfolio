#pragma once

#include <EngineBase/EngineMath.h>
#include <EngineCore/EngineCore.h>
#include <EngineCore/ImageRenderer.h>

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
/// X Buster ����
/// </summary>
enum class EBusterState
{
	CreateBuster,
	DefaultCharge,	// �⺻.
	MiddleCharge,	// �� ����.
	PullCharge,		// Ǯ ����.
	BusterCrash,	// �浹.
	BusterEnd,		// ����.
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

	Disappear,		// �����.
	Appear,			// ��Ÿ��.

	FeatherAttack,
	RisingSlash,
	TrackingShot,

	Death,
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
};

/// <summary>
/// Enemy SpikeMarl ����.
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


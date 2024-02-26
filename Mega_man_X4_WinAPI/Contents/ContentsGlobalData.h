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

	Run,			// �����̱�.
	RunAndAttack,	// �����̴� �� ����.
	RunAndCharge,	// �����̴� �� ����.

	Jump,			// ����.
	Sky,			// ����.
	JumpEnd,		// ����.

	Attack,			// ����.
	AttackWait,		// ���� ��.
	AttackEnd,		// ���� ��.

	DashStart,		// �뽬 ����.
	DashLoop,		// �뽬 ��.
	DashEnd,		// �뽬 ����.

	Charge,			// ����.
	//Hit,
};

enum class EBusterState
{
	CreateBuster,
	DefaultCharge,	// �⺻.
	MiddleCharge,	// �� ����.
	PullCharge,		// Ǯ ����.
	BusterCrash,	// �浹.
	BusterEnd,		// ����.
};

enum class EBossState
{
	None,

	Intro,
	Ready,

	FeatherAttack,
	RisingSlash,
	TrackingShot,

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


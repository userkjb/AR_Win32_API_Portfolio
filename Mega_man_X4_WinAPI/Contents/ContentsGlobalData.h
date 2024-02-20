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
	Summons,		// ��ȯ.
	Comeback,		// ����.
	Idle,			// �⺻.
	Run,			// �����̱�.
	Jump,			// ����.
	Sky,			// ����.
	JumpEnd,		// ����.
	IdleAttack,		// ���ڸ� ����.
	IdleAttackWait,	// ���ڸ� ���� ��.
	IdleAttackEnd,	// ���ڸ� ���� ��.
	DashStart,		// �뽬 ����.
	DashLoop,		// �뽬 ��.
	DashEnd,		// �뽬 ����.
	Charge,			// ����.
	RunAndAttack,	// �����̴� �� ����.
	//Hit,
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
	Map,
	Player,
	MapObject,
};


class UContentsGlobalData
{
public :
	static UWindowImage* ColMapImage;

private :
	UContentsGlobalData();
	~UContentsGlobalData();
};


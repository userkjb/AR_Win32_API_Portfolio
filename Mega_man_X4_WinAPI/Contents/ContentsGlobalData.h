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
	Idle,
	Run,
	Jump,
	Sky,
	JumpEnd,
	IdleAttack,
	IdleAttackWait,
	IdleAttackEnd,
	//Hit,
};

/// <summary>
/// Actor의 방향.
/// </summary>
enum class EActorDir
{
	Left,
	Right,
};


class UContentsGlobalData
{
public :
	static UWindowImage* ColMapImage;

private :
	UContentsGlobalData();
	~UContentsGlobalData();
};


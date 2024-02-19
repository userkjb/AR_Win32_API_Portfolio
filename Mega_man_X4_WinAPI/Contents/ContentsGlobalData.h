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
	Idle,			// 기본.
	Run,			// 움직이기.
	Jump,			// 점프.
	Sky,			// 공중.
	JumpEnd,		// 착지.
	IdleAttack,		// 제자리 공격.
	IdleAttackWait,	// 제자리 공격 중.
	IdleAttackEnd,	// 제자리 공격 끝.
	DashStart,		// 대쉬 시작.
	DashLoop,		// 대쉬 중.
	DashEnd,		// 대쉬 종료.
	Charge,			// 차지.
	RunAndAttack,	// 움직이는 중 공격.
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


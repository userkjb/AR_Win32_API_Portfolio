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

	Jump,			// 점프.
	Sky,			// 공중.
	JumpEnd,		// 착지.

	DashStart,		// 대쉬 시작.
	DashLoop,		// 대쉬 중.
	DashEnd,		// 대쉬 종료.

	//Hit,
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


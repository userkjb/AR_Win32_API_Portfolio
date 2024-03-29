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
	BossObject,
	Enemy,
};

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

enum class EBackGroundMapState
{
	None,

	Title,
	GameMenu,
};

/// <summary>
/// Attack은 X 버튼이 눌렸을 때 해당 상태에서
/// Attack이 붙은 상태로 넘어가는 간다.
/// </summary>
enum class EEgseuState
{
	None,
	Wait,

	Summon,			// 소환 이동.(레이저로 내려옴.)
	Summon_Loop,	// 소환 중.(땅에 닿으면 Character로 변화.)
	Summon_End,		// 소환 종료.(소환 완료.)

	Idle,			// 기본 상태.
	//Idle_Loop,
	//Idle_End,

	IdleJump,		// 기본 상태에서 점프.
	IdleJump_Loop,	// 채공 중.
	IdleJump_End,	// 착지.

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

	// Idle -> Dash / 디테일 영역.
	DashAttack,
	DashAttack_Loop,
	DashAttack_End,

	IdleRun,
	IdleRun_Loop,
	//IdleRun_End, // 그림이 없음.

	// Idle -> Run
	RunAttack_Down, // 달리면서 공격 시작.
	RunAttack_Down_Loop, // 달리면서 공격.
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

	WallClingAttack_Down,
	WallClingAttack_Down_Loop,
	WallClingAttack_Up,
	WallClingAttack_Up_Loop,
	
	WallKick,
	WallKickAttack_Down,
	WallKickAttack_Up,

	Hit,
	Hit_MiruTorearu,

	FocusCreate,
	FocusLoop,
	FocusEnd,

	AutoRunRight, // 스테이지 이동할 때 사용.
	BossRoomAutoRun,
	Victory,
	ReSummon,
	ReSummon_Up,
};

/// <summary>
/// X Buster 상태
/// </summary>
enum class EBusterState
{
	CreateBuster,

	CreateDefault,
	CreateMiddle,
	CreatePull,

	DefaultCharge,	// 기본.
	MiddleCharge,	// 중 차지.
	PullCharge,		// 풀 차지.
	BusterCrash,	// 충돌.
	BusterEnd,		// 종료.
};

/// <summary>
/// 보스 문 상태.
/// </summary>
enum class ECyberBossMapState
{
	None,
	
	Idle,
	SlowMove,
	CheckPointRoom,
	BossSlowMove,
	BossRoom,
	White,
	Restore,
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
	Wait,

	Intro, // 원에서 나타남.
	BattleReady, // 파칭~!

	Disappear,		// 사라짐.
	Appear,			// 나타남.

	FeatherAttack, // 날개 공격
	RisingSlash, // 위로!
	RisingSlash_Loop,
	TrackingShot, // 미사일 공격 시작
	TrackingShot_Loop, // 미사일 공격 진행 중.

	Death,
	Explosion, // 폭파.
	End,
	Destroy,
};
enum class ECyberPeacockMissileState
{
	None,
	Create,
	Run,
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
/// Enemy TriScans 상태.
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
	DeathStart, // 커졌다 작아짐.
	DeathLoop, // 사라짐.
	DeathEnd, // 소멸.
};

/// <summary>
/// Enemy SpikeMarl 상태.
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

enum class ECyberStageState
{
	None,

	Ready,
	GameProgress, // 게임 진행.
};

/// <summary>
/// 보스전 Level에 대한 State.
/// </summary>
enum class EBossLevelState
{
	None,

	Front_Door,
	CheckPointRoom,
	Back_Door,
	BossRoom, // Warning
	BossIntro, // 보스 나타남.
	BossText, // 텍스트 출력
	BossBattleReadySound, // UI-> 소리
	BossReady, // 보스 싸움 준비.(UI) [ -> 파칭 -> UI]
	BossBattle, // 보스전.
	BossDefeated, // 보스 패배.

	BossEnd, // 보스 터짐, 맵 흰색으로 진행.
	BossEndMiddle, // 보스 다 터지고 맵도 흰색 됨.
	PlayerVictory,
	PlayerReverseSummon,
}; // EBossLevelState

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
	MapObject,
	EnemyPrev,
	Enemy,
	EnemyNext,
	EnemyEffect,
	Boss,
	Player,
	BossObject,
	MapSub,
	Buster,
	WhiteImage,
};

/// <summary>
/// 콜리전 순서.
/// </summary>
enum class ECollisionOrder
{
	Player,
	Weapon,

	Boss,
	BossSkill,
	CyberPeacock_Feather,
	CyberPeacock_RisingSlash,
	CyberPeacock_Missile,

	Enemy,
	MiruTorearu,
	TriScan,

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


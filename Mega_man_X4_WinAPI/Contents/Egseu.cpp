#include "Egseu.h"
#include <EngineCore/EngineCore.h> // Helper
#include "Buster.h"
#include "CyberPeacock.h"
#include "MiruTorearu.h"

AEgseu::AEgseu()
{
}

AEgseu::~AEgseu()
{
}

AEgseu* AEgseu::MainPlayer = nullptr;

AEgseu* AEgseu::GetMainPlayer()
{
	return MainPlayer;
}

void AEgseu::BeginPlay()
{
	AActor::BeginPlay();
	MainPlayer = this;

	ChargeBeginPlay();
	PlayerBeginPlay();

	StateChange(EEgseuState::Summon);
}

void AEgseu::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	StateUpdate(_DeltaTime);
	BusterChargeTime(_DeltaTime);
	CollisionCheck(_DeltaTime);
	// Debug
	// Player State 출력
	//if (Debug_Num != static_cast<int>(State))
	//{
	//	UEngineDebug::OutPutDebugText(std::to_string(static_cast<int>(State)));
	//	Debug_Num = static_cast<int>(State);
	//}
	// Debug
	if (true == UEngineInput::IsDown(VK_F3))
	{
		if (true == PlayerCollision->IsActive())
		{
			PlayerCollision->ActiveOff();
		}
		else
		{
			PlayerCollision->ActiveOn();
		}
	}
}

void AEgseu::ChargeBeginPlay()
{
	MiddleChargeRender = CreateImageRenderer(static_cast<int>(ERenderOrder::Buster));
	MiddleChargeRender->SetImage("Charging_1.png");
	MiddleChargeRender->AutoImageScale(2.0f);
	MiddleChargeRender->SetPosition({ 0, -48 });
	//UWindowImage* ChargeImage_1 = MiddleChargeRender->GetImage();
	//FVector ChargeScale_1 = ChargeImage_1->GetScale();
	//MiddleChargeRender->SetTransform({ { 0, 0 }, {ChargeScale_1.iX() / 9, ChargeScale_1.iY()} });

	PullChargeRender = CreateImageRenderer(static_cast<int>(ERenderOrder::Buster));
	PullChargeRender->SetImage("Charging_2.png");
	PullChargeRender->AutoImageScale(2.0f);
	PullChargeRender->SetPosition({ 0, -48 });
	//UWindowImage* ChargeImage_2 = PullChargeRender->GetImage();
	//FVector ChargeScale_2 = ChargeImage_2->GetScale();
	//PullChargeRender->SetTransform({ { 0, 0 }, {ChargeScale_2.iX() / 4 , ChargeScale_2.iY()} });



	MiddleChargeRender->CreateAnimation("MiddleCharge", "Charging_1.png", 0, 8, 0.05f, true);
	PullChargeRender->CreateAnimation("PullCharge", "Charging_2.png", 0, 3, 0.05f, true);

	MiddleChargeRender->ChangeAnimation("MiddleCharge");
	PullChargeRender->ChangeAnimation("PullCharge");

	MiddleChargeRender->ActiveOff();
	PullChargeRender->ActiveOff();
}

void AEgseu::PlayerBeginPlay()
{
	PlayerRender = CreateImageRenderer(static_cast<int>(ERenderOrder::Player));
	PlayerRender->SetImage("x_Idle_Right.png");
	PlayerRender->AutoImageScale(2.5f); // 2.5
	//PlayerRender->SetTransform({ {0,0}, {35 * 3, 80 * 3} });

	PlayerCollision = CreateCollision(ECollisionOrder::Player);
	PlayerCollision->SetPosition({ 0, -50 });
	PlayerCollision->SetScale({ 80, 100 });
	PlayerCollision->SetColType(ECollisionType::Rect);

	// ===== Animation ======================================================
	// Summon
	PlayerRender->CreateAnimation("Summon", "x_Start.png", 0, 0, 0.2f, true);
	PlayerRender->CreateAnimation("Summon_Loop", "x_Start.png", 1, 16, 0.07f, false);

	// Idle
	PlayerRender->CreateAnimation("Idle_Right", "X_Idle_Right.png", 0, 3, 0.3f, true);
	PlayerRender->CreateAnimation("Idle_Left", "X_Idle_Left.png", 0, 3, 0.3f, true);

	// Run
	PlayerRender->CreateAnimation("Run_Ready_Right", "x_Move_Right.png", 0, 1, 0.025f, false);
	PlayerRender->CreateAnimation("Run_Ready_Left", "x_Move_Left.png", 0, 1, 0.025f, false);
	PlayerRender->CreateAnimation("Run_Right", "x_Move_Right.png", 2, 15, 0.05f, true);
	PlayerRender->CreateAnimation("Run_Left", "x_Move_Left.png", 2, 15, 0.05f, true);

	// Jump
	PlayerRender->CreateAnimation("Jump_Start_Right", "x_Jump_Right.png", 0, 7, 0.025f, false);
	PlayerRender->CreateAnimation("Jump_Start_Left", "x_Jump_Left.png", 0, 7, 0.025f, false);
	PlayerRender->CreateAnimation("Jumping_Right", "x_Jump_Right.png", 7, 7, 0.025f, false);
	PlayerRender->CreateAnimation("Jumping_Left", "x_Jump_Left.png", 7, 7, 0.025f, false);
	PlayerRender->CreateAnimation("Jump_End_Right", "x_Jump_Right.png", 8, 10, 0.025f, false);
	PlayerRender->CreateAnimation("Jump_End_Left", "x_Jump_Left.png", 8, 10, 0.025f, false);

	// Jump Attack
	PlayerRender->CreateAnimation("Jump_Start_Attack_Right", "JumpAttack_Right.png", { 0, 2, 4, 6, 8, 10, 12, 14 }, 0.025f, false);
	PlayerRender->CreateAnimation("Jump_Start_Attack_Left", "JumpAttack_Left.png", { 0, 2, 4, 6, 8, 10, 12, 14 }, 0.025f, false);
	PlayerRender->CreateAnimation("Jump_Ing_Attack_Right", "JumpAttack_Right.png", 14, 14, 0.025f, false);
	PlayerRender->CreateAnimation("Jump_Ing_Attack_Left", "JumpAttack_Left.png", 14, 14, 0.025f, false);
	PlayerRender->CreateAnimation("Jump_End_Attack_Right", "JumpAttack_Right.png", { 16, 18, 20 }, 0.025f, false);
	PlayerRender->CreateAnimation("Jump_End_Attack_Left", "JumpAttack_Left.png", { 16, 18, 20 }, 0.025f, false);

	//PlayerRender->CreateAnimation("Jump_Attack_Shoot_Right", "JumpAttack_Right.png", { 1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21 }, 0.05f, false);
	//PlayerRender->CreateAnimation("Jump_Attack_Shoot_Left", "JumpAttack_Left.png", { 1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21 }, 0.05f, false);

	// Dash
	PlayerRender->CreateAnimation("Dash_Start_Right", "x_Dash_Right.png", 0, 1, 0.05f, false);
	PlayerRender->CreateAnimation("Dash_Start_Left", "x_Dash_Left.png", 0, 1, 0.05f, false);
	PlayerRender->CreateAnimation("Dash_Loop_Right", "x_Dash_Right.png", 2, 3, 0.03f, true);
	PlayerRender->CreateAnimation("Dash_Loop_Left", "x_Dash_Left.png", 2, 3, 0.03f, true);
	PlayerRender->CreateAnimation("Dash_End_Right", "x_Dash_Right.png", 4, 7, 0.05f, false);
	PlayerRender->CreateAnimation("Dash_End_Left", "x_Dash_Left.png", 4, 7, 0.05f, false);
	// Dash Effect
	//PlayerRender->CreateAnimation("Dash_Right", "x_Dash_Left.png", 4, 7, 0.1f, false);
	//PlayerRender->CreateAnimation("Dash_Left", "x_Dash_Left.png", 4, 7, 0.1f, false);

	// Attack.
	// Idle - Attack
	PlayerRender->CreateAnimation("Idle_Attack_Start_Right", "X_Idle_Attack_Right.png", 0, 2, 0.1f, false);
	PlayerRender->CreateAnimation("Idle_Attack_Start_Left", "X_Idle_Attack_Left.png", 0, 2, 0.1f, false);
	PlayerRender->CreateAnimation("Idle_Attack_Loop_Right", "X_Idle_Attack_Right.png", 0, 0, 0.25f, true);
	PlayerRender->CreateAnimation("Idle_Attack_Loop_Left", "X_Idle_Attack_Left.png", 0, 0, 0.25f, true);
	PlayerRender->CreateAnimation("Idle_Attack_End_Right", "X_Idle_Attack_End_Right.png", 0, 1, 0.05f, false);
	PlayerRender->CreateAnimation("Idle_Attack_End_Left", "X_Idle_Attack_End_Left.png", 0, 1, 0.05f, false);

	// Run - Attack
	PlayerRender->CreateAnimation("Run_Attack_Start_Right", "Run_Attack_Right.png", 0, 1, 0.025f, false);
	PlayerRender->CreateAnimation("Run_Attack_Start_Left", "Run_Attack_Left.png", 0, 1, 0.025f, false);
	PlayerRender->CreateAnimation("Run_Attack_Loop_Right", "Run_Attack_Right.png", 2, 15, 0.05f, true);
	PlayerRender->CreateAnimation("Run_Attack_Loop_Left", "Run_Attack_Left.png", 2, 15, 0.05f, true);

	// Dash - Attack
	PlayerRender->CreateAnimation("Dash_Attack_Start_Right", "x_Dash_Attack_Right.png", { 0, 2 }, 0.025f, false);
	PlayerRender->CreateAnimation("Dash_Attack_Start_Left", "x_Dash_Attack_Left.png", { 0, 2 }, 0.025f, false);
	PlayerRender->CreateAnimation("Dash_Attack_Loop_Right", "x_Dash_Attack_Right.png", { 4, 6 }, 0.025f, true);
	PlayerRender->CreateAnimation("Dash_Attack_Loop_Left", "x_Dash_Attack_Left.png", { 4, 6 }, 0.025f, true);
	PlayerRender->CreateAnimation("Dash_Attack_End_Right", "x_Dash_Attack_Right.png", { 8, 10, 12, 14 }, 0.025f, false);
	PlayerRender->CreateAnimation("Dash_Attack_End_Left", "x_Dash_Attack_Left.png", { 8, 10, 12, 14 }, 0.025f, false);


	//PlayerRender->CreateAnimation("Run_Attack_Shoot_Start_Right", "x_Move_Attack_Right.png", { 1, 3 }, 0.05f, true);
	//PlayerRender->CreateAnimation("Run_Attack_Shoot_Start_Left", "x_Move_Attack_Left.png", { 1, 3 }, 0.05f, true);
	//PlayerRender->CreateAnimation("Run_Attack_Shoot_Loop_Right", "x_Move_Attack_Right.png", { 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31 }, 0.05f, true);
	//PlayerRender->CreateAnimation("Run_Attack_Shoot_Loop_Left", "x_Move_Attack_Left.png", { 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31 }, 0.05f, true);

	//PlayerRender->CreateAnimation("Dash_Attack_Shoot_Start_Right", "x_Dash_Attack_Right.png", { 1, 3 }, 0.05f, false);
	//PlayerRender->CreateAnimation("Dash_Attack_Shoot_Start_Left", "x_Dash_Attack_Left.png", { 1, 3 }, 0.05f, false);
	//PlayerRender->CreateAnimation("Dash_Attack_Shoot_Loop_Right", "x_Dash_Attack_Right.png", { 5, 7 }, 0.05f, false);
	//PlayerRender->CreateAnimation("Dash_Attack_Shoot_Loop_Left", "x_Dash_Attack_Left.png", { 5, 7 }, 0.05f, false);
	//PlayerRender->CreateAnimation("Dash_Attack_Shoot_End_Right", "x_Dash_Attack_Right.png", { 9, 11, 13, 15 }, 0.05f, false);
	//PlayerRender->CreateAnimation("Dash_Attack_Shoot_End_Left", "x_Dash_Attack_Left.png", { 9, 11, 13, 15 }, 0.05f, false);

	// Wall
	PlayerRender->CreateAnimation("WallCling_Start_Right", "Wall_Cling_Right.png", 0, 3, 0.025f, false);
	PlayerRender->CreateAnimation("WallCling_Start_Left", "Wall_Cling_Left.png", 0, 3, 0.025f, false);
	PlayerRender->CreateAnimation("WallCling_Loop_Right", "Wall_Cling_Right.png", 3, 3, 0.25f, false);
	PlayerRender->CreateAnimation("WallCling_Loop_Left", "Wall_Cling_Left.png", 3, 3, 0.25f, false);
	// Wall Attack
	PlayerRender->CreateAnimation("WallCling_Attack_Start_Right", "Wall_Cling_Attack_Right.png", 0, 3, 0.025f, false);
	PlayerRender->CreateAnimation("WallCling_Attack_Start_Left", "Wall_Cling_Attack_Left.png", 0, 3, 0.025f, false);
	PlayerRender->CreateAnimation("WallCling_Attack_Loop_Right", "Wall_Cling_Attack_Right.png", 3, 3, 0.025f, false);
	PlayerRender->CreateAnimation("WallCling_Attack_Loop_Left", "Wall_Cling_Attack_Left.png", 3, 3, 0.025f, false);

	// Wall Kick
	PlayerRender->CreateAnimation("WallKick_Right", "Wall_Cling_Kick_Right.png", 0, 2, 0.025f, false);
	PlayerRender->CreateAnimation("WallKick_Left", "Wall_Cling_Kick_Left.png", 0, 2, 0.025f, false);
	// Wall Kick Attack
	PlayerRender->CreateAnimation("WallKick_Attack_Right", "Wall_Cling_Kick_Attack_Right.png", 0, 2, 0.025f, false);
	PlayerRender->CreateAnimation("WallKick_Attack_Left", "Wall_Cling_Kick_Attack_Left.png", 0, 2, 0.025f, false);

	// Wall Effect
	//PlayerRender->CreateAnimation("WallCling_Effect_Right", "WallEffect_Right.png", 0, 7, 0.5f, false);
	//PlayerRender->CreateAnimation("WallCling_Effect_Left", "WallEffect_Left.png", 0, 7, 0.5f, false);
	//PlayerRender->CreateAnimation("WallKick_Effect_Right", "WallKick_Right.png", 0, 3, 0.5f, false);
	//PlayerRender->CreateAnimation("WallKick_Effect_Left", "WallKick_Left.png", 0, 3, 0.5f, false);

	// Hit
	PlayerRender->CreateAnimation("Hit_Right", "x_Damage_Right.png", 0, 3, 0.05f, true);
	PlayerRender->CreateAnimation("Hit_Left", "x_Damage_Left.png", 0, 3, 0.05f, true);

	// Death
	PlayerRender->CreateAnimation("Death_Right", "x_Damage_Right.png", 0, 0, 0.05f, true);
	PlayerRender->CreateAnimation("Death_Left", "x_Damage_Left.png", 0, 0, 0.05f, true);

	// =====================================================================

	PlayerRender->ChangeAnimation("Summon");
	StateChange(EEgseuState::Summon);
}

void AEgseu::DirCheck()
{
	EActorDir Dir = DirState;

	if (UEngineInput::IsPress(VK_LEFT))
	{
		Dir = EActorDir::Left;
	}
	if (UEngineInput::IsPress(VK_RIGHT))
	{
		Dir = EActorDir::Right;
	}

	// 이전 방향과 다르다면, 
	if (Dir != DirState)
	{
		// 현재 애니메이션에서 반대되는 애니메이션으로 바꿔줘야 한다.

		DirState = Dir;

		// Animation을 바꿔줘야 한다.
		// 쉽게 바꿔주기 위해 GetAnimationName 함수를 만들어 주고 사용한다.
		std::string Name = GetAnimationName(CurAnimationName);

		// ChangeAnimation은 Animation 도중에 바뀌기 때문에 현재 Frame을 알아야 함다.
		// GetCurAnimationFrame 함수를 만들고 사용한다.
		// 비슷한 이유로 Animation의 시간도 알아야 한다.
		// GetCurAnimationTime 함수를 만들고 사용한다.
		PlayerRender->ChangeAnimation(Name, true, PlayerRender->GetCurAnimationFrame(), PlayerRender->GetCurAnimationTime());
	}
}

std::string AEgseu::GetAnimationName(std::string _Name)
{
	std::string DirName = "";

	switch (DirState)
	{
	case EActorDir::Left:
		DirName = "_Left";
		break;
	case EActorDir::Right:
		DirName = "_Right";
		break;
	default:
		break;
	}

	// 상태를 받는다. Ex) Idle
	CurAnimationName = _Name;

	// Ex) Idle_Left
	return _Name + DirName;
}

void AEgseu::StateChange(EEgseuState _State)
{
	if (State != _State)
	{
		switch (_State)
		{
		case EEgseuState::Wait:
			WaitStart();
			break;
		case EEgseuState::Summon :
			SummonStart();
			break;
		case EEgseuState::Summon_Loop:
			Summon_LoopStart();
			break;
		case EEgseuState::Summon_End:
			Summon_EndStart();
			break;
		case EEgseuState::Idle:
			IdleStart();
			break;
		//case EEgseuState::Idle_Loop:
		//	Idle_LoopStart();
		//	break;
		//case EEgseuState::Idle_End:
		//	Idle_EndStart();
		//	break;
		case EEgseuState::IdleJump:
			IdleJumpStart();
			break;
		case EEgseuState::IdleJump_Loop:
			IdleJump_LoopStart();
			break;
		case EEgseuState::IdleJump_End:
			IdleJump_EndStart();
			break;
		case EEgseuState::JumpAttack_Down:
			JumpAttack_DownStart();
			break;
		case EEgseuState::JumpAttack_Down_Loop:
			JumpAttack_Down_LoopStart();
			break;
		case EEgseuState::JumpAttack_Down_End:
			JumpAttack_Down_EndStart();
			break;
		case EEgseuState::JumpAttack_Up:
			JumpAttack_UpStart();
			break;
		case EEgseuState::JumpAttack_Up_Loop:
			JumpAttack_Up_LoopStart();
			break;
		case EEgseuState::JumpAttack_Up_End:
			JumpAttack_Up_EndStart();
			break;
		case EEgseuState::IdleAttack_Down:
			IdleAttack_DownStart();
			break;
		case EEgseuState::IdleAttack_Down_Loop:
			IdleAttack_Down_LoopStart();
			break;
		case EEgseuState::IdleAttack_Down_End:
			IdleAttack_Down_EndStart();
			break;
		case EEgseuState::IdleAttack_Up:
			IdleAttack_UpStart();
			break;
		case EEgseuState::IdleAttack_Up_Loop:
			IdleAttack_Up_LoopStart();
			break;
		case EEgseuState::IdleAttack_Up_End:
			IdleAttack_Up_EndStart();
			break;
		case EEgseuState::IdleDash:
			IdleDashStart();
			break;
		case EEgseuState::IdleDash_Loop:
			IdleDash_LoopStart();
			break;
		case EEgseuState::IdleDash_End:
			IdleDash_EndStart();
			break;
		case EEgseuState::DashAttack:
			DashAttackStart();
			break;
		case EEgseuState::DashAttack_Loop:
			DashAttack_LoopStart();
			break;
		case EEgseuState::DashAttack_End:
			DashAttack_EndStart();
			break;
		case EEgseuState::IdleRun:
			IdleRunStart();
			break;
		case EEgseuState::IdleRun_Loop:
			IdleRun_LoopStart();
			break;
		//case EEgseuState::IdleRun_End:
		//	IdleRun_EndStart();
		//	break;
		case EEgseuState::RunAttack_Down:
			RunAttack_DownStart();
			break;
		case EEgseuState::RunAttack_Down_Loop:
			RunAttack_Down_LoopStart();
			break;
		//case EEgseuState::RunAttack_Down_End:
		//	RunAttack_Down_EndStart();
		//	break;
		case EEgseuState::RunAttack_Up:
			RunAttack_UpStart();
			break;
		case EEgseuState::RunAttack_Up_Loop:
			RunAttack_Up_LoopStart();
			break;
		//case EEgseuState::RunAttack_Up_End:
		//	RunAttack_Up_EndStart();
		//	break;
		case EEgseuState::RunDash:
			RunDashStart();
			break;
		case EEgseuState::RunDash_Loop:
			RunDash_LoopStart();
			break;
		case EEgseuState::RunDash_End:
			RunDash_EndStart();
			break;
		case EEgseuState:: RunDashAttack_Down:
			RunDashAttack_DownStart();
			break;
		case EEgseuState:: RunDashAttack_Down_Loop:
			RunDashAttack_Down_LoopStart();
			break;
		case EEgseuState:: RunDashAttack_Down_End:
			RunDashAttack_Down_EndStart();
			break;
		case EEgseuState:: RunDashAttack_Up:
			RunDashAttack_UpStart();
			break;
		case EEgseuState:: RunDashAttack_Up_Loop:
			RunDashAttack_Up_LoopStart();
			break;
		case EEgseuState:: RunDashAttack_Up_End:
			RunDashAttack_Up_EndStart();
			break;
		case EEgseuState::RunDashJump:
			RunDashJumpStart();
			break;
		case EEgseuState::RunDashJump_Loop:
			RunDashJump_LoopStart();
			break;
		case EEgseuState::RunDashJump_End:
			RunDashJump_EndStart();
			break;
		case EEgseuState::RunDashJumpAttack_Down:
			RunDashJumpAttack_DownStart();
			break;
		case EEgseuState::RunDashJumpAttack_Down_Loop:
			RunDashJumpAttack_Down_LoopStart();
			break;
		case EEgseuState::RunDashJumpAttack_Down_End:
			RunDashJumpAttack_Down_EndStart();
			break;
		case EEgseuState::RunDashJumpAttack_Up:
			RunDashJumpAttack_UpStart();
			break;
		case EEgseuState::RunDashJumpAttack_Up_Loop:
			RunDashJumpAttack_Up_LoopStart();
			break;
		case EEgseuState::RunDashJumpAttack_Up_End:
			RunDashJumpAttack_Up_EndStart();
			break;
		case EEgseuState::RunJump:
			RunJumpStart();
			break;
		case EEgseuState::RunJump_Loop:
			RunJump_LoopStart();
			break;
		case EEgseuState::RunJump_End:
			RunJump_EndStart();
			break;
		case EEgseuState::RunJumpAttack_Down:
			RunJumpAttack_DownStart();
			break;
		case EEgseuState::RunJumpAttack_Down_Loop:
			RunJumpAttack_Down_LoopStart();
			break;
		case EEgseuState::RunJumpAttack_Down_End:
			RunJumpAttack_Down_EndStart();
			break;
		case EEgseuState::RunJumpAttack_Up:
			RunJumpAttack_UpStart();
			break;
		case EEgseuState::RunJumpAttack_Up_Loop:
			RunJumpAttack_Up_LoopStart();
			break;
		case EEgseuState::RunJumpAttack_Up_End:
			RunJumpAttack_Up_EndStart();
			break;
		case EEgseuState::WallCling:
			WallClingStart();
			break;
		case EEgseuState::WallCling_Loop:
			WallCling_LoopStart();
			break;
		case EEgseuState::WallClingAttack_Down:
			WallClingAttack_DownStart();
			break;
		case EEgseuState::WallClingAttack_Down_Loop:
			WallClingAttack_Down_LoopStart();
			break;
		case EEgseuState::WallClingAttack_Up:
			WallClingAttack_UpStart();
			break;
		case EEgseuState::WallClingAttack_Up_Loop:
			WallClingAttack_Up_LoopStart();
			break;
		case EEgseuState::WallKick:
			WallKickStart();
			break;
		case EEgseuState::WallKickAttack_Down:
			WallKickAttack_DownStart();
			break;
		case EEgseuState::WallKickAttack_Up:
			WallKickAttack_UpStart();
			break;
		case EEgseuState::Hit:
			HitStart();
			break;
		case EEgseuState::Hit_MiruTorearu:
			Hit_MiruTorearuStart();
			break;
		case EEgseuState::FocusCreate:
			FocusCreateStart();
			break;
		case EEgseuState::FocusLoop:
			FocusLoopStart();
			break;
		case EEgseuState::FocusEnd:
			FocusEndStart();
			break;
		case EEgseuState::AutoRunRight:
			AutoRunRightStart();
			break;
		case EEgseuState::BossRoomAutoRun:
			BossRoomAutoRunStart();
			break;
		default :
			break;
		}
	}
	State = _State;
}

void AEgseu::StateUpdate(float _DeltaTime)
{
	switch (State)
	{
	case EEgseuState::Wait:
		Wait(_DeltaTime);
		break;
	case EEgseuState::Summon:
		Summon(_DeltaTime);
		break;
	case EEgseuState::Summon_Loop:
		Summon_Loop(_DeltaTime);
		break;
	case EEgseuState::Summon_End:
		Summon_End(_DeltaTime);
		break;
	case EEgseuState::Idle:
		Idle(_DeltaTime);
		break;
	//case EEgseuState::Idle_Loop:
	//	Idle_Loop(_DeltaTime);
	//	break;
	//case EEgseuState::Idle_End:
	//	Idle_End(_DeltaTime);
	//	break;
	case EEgseuState::IdleJump:
		IdleJump(_DeltaTime);
		break;
	case EEgseuState::IdleJump_Loop:
		IdleJump_Loop(_DeltaTime);
		break;
	case EEgseuState::IdleJump_End:
		IdleJump_End(_DeltaTime);
		break;
	case EEgseuState::JumpAttack_Down:
		JumpAttack_Down(_DeltaTime);
		break;
	case EEgseuState::JumpAttack_Down_Loop:
		JumpAttack_Down_Loop(_DeltaTime);
		break;
	case EEgseuState::JumpAttack_Down_End:
		JumpAttack_Down_End(_DeltaTime);
		break;
	case EEgseuState::JumpAttack_Up:
		JumpAttack_Up(_DeltaTime);
		break;
	case EEgseuState::JumpAttack_Up_Loop:
		JumpAttack_Up_Loop(_DeltaTime);
		break;
	case EEgseuState::JumpAttack_Up_End:
		JumpAttack_Up_End(_DeltaTime);
		break;
	case EEgseuState::IdleAttack_Down:
		IdleAttack_Down(_DeltaTime);
		break;
	case EEgseuState::IdleAttack_Down_Loop:
		IdleAttack_Down_Loop(_DeltaTime);
		break;
	case EEgseuState::IdleAttack_Down_End:
		IdleAttack_Down_End(_DeltaTime);
		break;
	case EEgseuState::IdleAttack_Up:
		IdleAttack_Up(_DeltaTime);
		break;
	case EEgseuState::IdleAttack_Up_Loop:
		IdleAttack_Up_Loop(_DeltaTime);
		break;
	case EEgseuState::IdleAttack_Up_End:
		IdleAttack_Up_End(_DeltaTime);
		break;
	case EEgseuState::IdleDash:
		IdleDash(_DeltaTime);
		break;
	case EEgseuState::IdleDash_Loop:
		IdleDash_Loop(_DeltaTime);
		break;
	case EEgseuState::IdleDash_End:
		IdleDash_End(_DeltaTime);
		break;
	case EEgseuState::DashAttack:
		DashAttack(_DeltaTime);
		break;
	case EEgseuState::DashAttack_Loop:
		DashAttack_Loop(_DeltaTime);
		break;
	case EEgseuState::DashAttack_End:
		DashAttack_End(_DeltaTime);
		break;
	case EEgseuState::IdleRun:
		IdleRun(_DeltaTime);
		break;
	case EEgseuState::IdleRun_Loop:
		IdleRun_Loop(_DeltaTime);
		break;
	//case EEgseuState::IdleRun_End:
	//	IdleRun_End(_DeltaTime);
	//	break;
	case EEgseuState::RunAttack_Down:
		RunAttack_Down(_DeltaTime);
		break;
	case EEgseuState::RunAttack_Down_Loop:
		RunAttack_Down_Loop(_DeltaTime);
		break;
	//case EEgseuState::RunAttack_Down_End:
	//	RunAttack_Down_End(_DeltaTime);
	//	break;
	case EEgseuState::RunAttack_Up:
		RunAttack_Up(_DeltaTime);
		break;
	case EEgseuState::RunAttack_Up_Loop:
		RunAttack_Up_Loop(_DeltaTime);
		break;
	//case EEgseuState::RunAttack_Up_End:
	//	RunAttack_Up_End(_DeltaTime);
	//	break;
	case EEgseuState::RunDash:
		RunDash(_DeltaTime);
		break;
	case EEgseuState::RunDash_Loop:
		RunDash_Loop(_DeltaTime);
		break;
	case EEgseuState::RunDash_End:
		RunDash_End(_DeltaTime);
		break;
	case EEgseuState::RunDashAttack_Down:
		RunDashAttack_Down(_DeltaTime);
		break;
	case EEgseuState::RunDashAttack_Down_Loop:
		RunDashAttack_Down_Loop(_DeltaTime);
		break;
	case EEgseuState::RunDashAttack_Down_End:
		RunDashAttack_Down_End(_DeltaTime);
		break;
	case EEgseuState::RunDashAttack_Up:
		RunDashAttack_Up(_DeltaTime);
		break;
	case EEgseuState::RunDashAttack_Up_Loop:
		RunDashAttack_Up_Loop(_DeltaTime);
		break;
	case EEgseuState::RunDashAttack_Up_End:
		RunDashAttack_Up_End(_DeltaTime);
		break;
	case EEgseuState::RunDashJump:
		RunDashJump(_DeltaTime);
		break;
	case EEgseuState::RunDashJump_Loop:
		RunDashJump_Loop(_DeltaTime);
		break;
	case EEgseuState::RunDashJump_End:
		RunDashJump_End(_DeltaTime);
		break;
	case EEgseuState::RunDashJumpAttack_Down:
		RunDashJumpAttack_Down(_DeltaTime);
		break;
	case EEgseuState::RunDashJumpAttack_Down_Loop:
		RunDashJumpAttack_Down_Loop(_DeltaTime);
		break;
	case EEgseuState::RunDashJumpAttack_Down_End:
		RunDashJumpAttack_Down_End(_DeltaTime);
		break;
	case EEgseuState::RunDashJumpAttack_Up:
		RunDashJumpAttack_Up(_DeltaTime);
		break;
	case EEgseuState::RunDashJumpAttack_Up_Loop:
		RunDashJumpAttack_Up_Loop(_DeltaTime);
		break;
	case EEgseuState::RunDashJumpAttack_Up_End:
		RunDashJumpAttack_Up_End(_DeltaTime);
		break;
	case EEgseuState::RunJump:
		RunJump(_DeltaTime);
		break;
	case EEgseuState::RunJump_Loop:
		RunJump_Loop(_DeltaTime);
		break;
	case EEgseuState::RunJump_End:
		RunJump_End(_DeltaTime);
		break;
	case EEgseuState::RunJumpAttack_Down:
		RunJumpAttack_Down(_DeltaTime);
		break;
	case EEgseuState::RunJumpAttack_Down_Loop:
		RunJumpAttack_Down_Loop(_DeltaTime);
		break;
	case EEgseuState::RunJumpAttack_Down_End:
		RunJumpAttack_Down_End(_DeltaTime);
		break;
	case EEgseuState::RunJumpAttack_Up:
		RunJumpAttack_Up(_DeltaTime);
		break;
	case EEgseuState::RunJumpAttack_Up_Loop:
		RunJumpAttack_Up_Loop(_DeltaTime);
		break;
	case EEgseuState::RunJumpAttack_Up_End:
		RunJumpAttack_Up_End(_DeltaTime);
		break;
	case EEgseuState::WallCling:
		WallCling(_DeltaTime);
		break;
	case EEgseuState::WallCling_Loop:
		WallCling_Loop(_DeltaTime);
		break;
	case EEgseuState::WallClingAttack_Down:
		WallClingAttack_Down(_DeltaTime);
		break;
	case EEgseuState::WallClingAttack_Down_Loop:
		WallClingAttack_Down_Loop(_DeltaTime);
		break;
	case EEgseuState::WallClingAttack_Up:
		WallClingAttack_Up(_DeltaTime);
		break;
	case EEgseuState::WallClingAttack_Up_Loop:
		WallClingAttack_Up_Loop(_DeltaTime);
		break;
	case EEgseuState::WallKick:
		WallKick(_DeltaTime);
		break;
	case EEgseuState::WallKickAttack_Down:
		WallKickAttack_Down(_DeltaTime);
		break;
	case EEgseuState::WallKickAttack_Up:
		WallKickAttack_Up(_DeltaTime);
		break;
	case EEgseuState::Hit:
		Hit(_DeltaTime);
		break;
	case EEgseuState::Hit_MiruTorearu:
		Hit_MiruTorearu(_DeltaTime);
		break;
	case EEgseuState::FocusCreate:
		FocusCreate(_DeltaTime);
		break;
	case EEgseuState::FocusLoop:
		FocusLoop(_DeltaTime);
		break;
	case EEgseuState::FocusEnd:
		FocusEnd(_DeltaTime);
		break;
	case EEgseuState::AutoRunRight:
		AutoRunRight(_DeltaTime);
		break;
	case EEgseuState::BossRoomAutoRun:
		BossRoomAutoRun(_DeltaTime);
		break;
	default :
		break;
	}
}

// Begin & Tick
#pragma region Wait
void AEgseu::WaitStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Idle"));
	if (DashTime != 0.0f)
	{
		DashTime = 0.0f;
	}
}

void AEgseu::Wait(float _DeltaTime)
{

}
#pragma endregion

#pragma region Summon
void AEgseu::SummonStart()
{
	PlayerRender->ChangeAnimation("Summon");
}

void AEgseu::Summon_LoopStart()
{
	PlayerRender->ChangeAnimation("Summon_Loop");
}

void AEgseu::Summon_EndStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Idle"));
}

void AEgseu::Summon(float _DeltaTime)
{
	SummonVector = FVector::Down * 1000.0f * _DeltaTime;
	//MoveUpdate(_DeltaTime);
	AddActorLocation(SummonVector);

	// Actor의 기준점 가져오기.
	FVector CheckPos = GetActorLocation();
	Color8Bit Color = UContentsGlobalData::ColMapImage->GetColor(CheckPos.iX(), CheckPos.iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
	{
		SummonVector = FVector::Zero;
		StateChange(EEgseuState::Summon_Loop);
		return;
	}
}

void AEgseu::Summon_Loop(float _DeltaTime)
{
	SummonDelayTime += _DeltaTime;
	if (true == PlayerRender->IsCurAnimationEnd())
	{
		if (1.5f < SummonDelayTime)
		{
			StateChange(EEgseuState::Summon_End);
			SummonDelayTime = 0.0f;
			return;
		}
	}
}

void AEgseu::Summon_End(float _DeltaTime)
{
	// 딜레이 넣으면 좋은 디테일.
	StateChange(EEgseuState::Idle);
	return;
}
#pragma endregion

#pragma region Idle
void AEgseu::IdleStart()
{
	JumpVector = FVector::Zero; // 벽 타고 내려왔을 대 점프 초기화.
	BusterDelayTime = 0.0f;
	PlayerRender->ChangeAnimation(GetAnimationName("Idle"));
	DirCheck();
}
//void AEgseu::Idle_LoopStart()
//{
//}
//
//void AEgseu::Idle_EndStart()
//{
//}

void AEgseu::Idle(float _DeltaTime)
{
	// 보스 문 통과 Error 체크
	FVector Dash = DashVector;
	if (DashVector.X != 0.0f)
	{
		DashVector = FVector::Zero;
	}
	if (DashTime != 0.0f)
	{
		DashTime = 0.0f;
	}

	// 가만히 있는데 뱡향 키가 눌렸을 때.
	if (true == UEngineInput::IsPress(VK_LEFT) ||
		true == UEngineInput::IsPress(VK_RIGHT))
	{
		StateChange(EEgseuState::IdleRun);
		return;
	}

	// 점프 C
	if (true == UEngineInput::IsDown('C'))
	{
		StateChange(EEgseuState::IdleJump);
		return;
	}
	
	// 공격 X
	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault, Idle_Muzzle);
		StateChange(EEgseuState::IdleAttack_Down);
		return;
	}

	// 발싸!!!
	if (true == UEngineInput::IsUp('X'))
	{
		if (BusterChargTime >= 1.0f)
		{
			BusterDelayTime = 0.0f;
			if (1.0f <= BusterChargTime && BusterChargTime < 2.0f)
			{
				BusterCreate(EBusterState::CreateMiddle, Idle_Muzzle);
			}
			else if (2.0f <= BusterChargTime)
			{
				BusterCreate(EBusterState::CreatePull, Idle_Muzzle);
			}
			StateChange(EEgseuState::IdleAttack_Up);
			return;
		}
	}

	// 대쉬 Z
	if (true == UEngineInput::IsDown('Z'))
	{
		StateChange(EEgseuState::IdleDash);
		return;
	}

	Color8Bit Color = UContentsGlobalData::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit(0, 0, 0, 0))
	{
		StateChange(EEgseuState::IdleJump_Loop);
		return;
	}

	//MoveUpdate(_DeltaTime);
}

//void AEgseu::Idle_Loop(float _DeltaTime)
//{
//}
//
//void AEgseu::Idle_End(float _DeltaTime)
//{
//}
#pragma endregion

#pragma region IdleJump
void AEgseu::IdleJumpStart()
{
	JumpVector = JumpPower;
	//int CurFrame = PlayerRender->GetCurAnimationFrame();
	PlayerRender->ChangeAnimation(GetAnimationName("Jump_Start"));
	DirCheck();
}

void AEgseu::IdleJump(float _DeltaTime)
{
	DirCheck();
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		RunVector = FVector::Left * MoveSpeed;
	}
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		RunVector = FVector::Right * MoveSpeed;
	}

	MoveUpdate(_DeltaTime);

	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault, Jump_Muzzle);
		StateChange(EEgseuState::JumpAttack_Down);
		return;
	}

	if (true == UEngineInput::IsUp('X'))
	{
		if (BusterChargTime <= 0.8f)
		{
			return;
		}
		if (1.0f <= BusterChargTime && BusterChargTime < 2.0f)
		{
			BusterCreate(EBusterState::CreateMiddle, Jump_Muzzle);
		}
		else if (2.0f <= BusterChargTime)
		{
			BusterCreate(EBusterState::CreatePull, Jump_Muzzle);
		}
		BusterDelayTime = 0.0f;
		StateChange(EEgseuState::JumpAttack_Up);
		return;
	}

	if (true == PlayerRender->IsCurAnimationEnd())
	{
		StateChange(EEgseuState::IdleJump_Loop);
		return;
	}
}
#pragma endregion

#pragma region IdleJump_Loop
void AEgseu::IdleJump_LoopStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Jumping"));
	DirCheck();
}

void AEgseu::IdleJump_Loop(float _DeltaTime)
{
	DirCheck();
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		RunVector = FVector::Left * MoveSpeed;
	}
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		RunVector = FVector::Right * MoveSpeed;
	}

	MoveUpdate(_DeltaTime);

	// 공격
	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault, Jump_Muzzle);
		StateChange(EEgseuState::JumpAttack_Down_Loop);
		return;
	}

	// 챠지 공격!!
	if (true == UEngineInput::IsUp('X'))
	{
		if (BusterChargTime <= 0.8f)
		{
			return;
		}
		if (1.0f <= BusterChargTime && BusterChargTime < 2.0f)
		{
			BusterCreate(EBusterState::CreateMiddle, Jump_Muzzle);
		}
		else if (2.0f <= BusterChargTime)
		{
			BusterCreate(EBusterState::CreatePull, Jump_Muzzle);
		}
		BusterDelayTime = 0.0f;
		StateChange(EEgseuState::JumpAttack_Up_Loop);
		return;
	}

	// '벽'에 닿음
	bool WallChecl = CalWallCheck();
	if (true == WallChecl)
	{
		StateChange(EEgseuState::WallCling);
		return;
	}

	// '땅'에 닿음
	Color8Bit Color = UContentsGlobalData::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY() + 5, Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
	{
		JumpVector = FVector::Zero;
		StateChange(EEgseuState::IdleJump_End);
		return;
	}
}
#pragma endregion

#pragma region IdleJump_End
void AEgseu::IdleJump_EndStart()
{
	JumpVector = FVector::Zero;
	PlayerRender->ChangeAnimation(GetAnimationName("Jump_End"));
}

void AEgseu::IdleJump_End(float _DeltaTime)
{
	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault, Jump_Muzzle);
		StateChange(EEgseuState::JumpAttack_Down_End);
		return;
	}

	if (true == UEngineInput::IsPress(VK_RIGHT) || true == UEngineInput::IsPress(VK_LEFT))
	{
		StateChange(EEgseuState::IdleRun_Loop);
		return;
	}

	if (true == PlayerRender->IsCurAnimationEnd())
	{
		StateChange(EEgseuState::Idle);
		return;
	}
}
#pragma endregion

#pragma region JumpAttack_Down
void AEgseu::JumpAttack_DownStart()
{
	int CurFrame  = PlayerRender->GetCurAnimationFrame();
	PlayerRender->ChangeAnimation(GetAnimationName("Jump_Start_Attack"), false, CurFrame);
}

void AEgseu::JumpAttack_Down(float _DeltaTime)
{
	BusterDelayTime += _DeltaTime;
	
	DirCheck();
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		RunVector = FVector::Left * MoveSpeed;
	}
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		RunVector = FVector::Right * MoveSpeed;
	}
	MoveUpdate(_DeltaTime);

	// 또 공격?
	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault, Jump_Muzzle);
	}

	// 0.5초가 지났다.
	if (BusterDelayTime >= BusterDelayTimeMax)
	{
		StateChange(EEgseuState::IdleJump);
		return;
	}

	// 바닥이네? -> 낮은 바닥...
	Color8Bit Color = UContentsGlobalData::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
	{
		JumpVector = FVector::Zero;
		StateChange(EEgseuState::IdleJump_End);
		return;
	}
}
#pragma endregion

#pragma region JumpAttack_Down_Loop
void AEgseu::JumpAttack_Down_LoopStart()
{
	int CurFrame = PlayerRender->GetCurAnimationFrame();
	PlayerRender->ChangeAnimation(GetAnimationName("Jump_Ing_Attack"), false, CurFrame);
	DirCheck();
}

void AEgseu::JumpAttack_Down_Loop(float _DeltaTime)
{
	BusterDelayTime += _DeltaTime;
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		RunVector = FVector::Left * MoveSpeed;
	}
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		RunVector = FVector::Right * MoveSpeed;
	}
	MoveUpdate(_DeltaTime);

	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault, Jump_Muzzle);
	}

	// 특정 시간동안 이벤트가 없으면 다시 IdleJump_Loop로 이동.
	if (BusterDelayTime >= BusterDelayTimeMax)
	{
		StateChange(EEgseuState::IdleJump_Loop);
		return;
	}

	Color8Bit Color = UContentsGlobalData::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
	{
		JumpVector = FVector::Zero;
		StateChange(EEgseuState::IdleJump_End);
		return;
	}
}
#pragma endregion

#pragma region JumpAttack_Down_End
void AEgseu::JumpAttack_Down_EndStart()
{
	int CurFrame = PlayerRender->GetCurAnimationFrame();
	PlayerRender->ChangeAnimation(GetAnimationName("Jump_End_Attack"), false, CurFrame);
}

void AEgseu::JumpAttack_Down_End(float _DeltaTime)
{
	BusterDelayTime += _DeltaTime;
	JumpVector = FVector::Zero;
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		RunVector = FVector::Left * MoveSpeed;
	}
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		RunVector = FVector::Right * MoveSpeed;
	}
	MoveUpdate(_DeltaTime);

	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault, Jump_Muzzle);
	}

	if (BusterDelayTime >= BusterDelayTimeMax)
	{
		StateChange(EEgseuState::IdleJump_End);
		return;
	}

	if (true == PlayerRender->IsCurAnimationEnd())
	{
		StateChange(EEgseuState::Idle);
		return;
	}
}
#pragma endregion

#pragma region JumpAttack Up
void AEgseu::JumpAttack_UpStart()
{
	int CurFrame = PlayerRender->GetCurAnimationFrame();
	PlayerRender->ChangeAnimation(GetAnimationName("Jump_Start_Attack"), false, CurFrame);
}

void AEgseu::JumpAttack_Up(float _DeltaTime)
{
	BusterDelayTime += _DeltaTime;
	DirCheck();
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		RunVector = FVector::Left * MoveSpeed;
	}
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		RunVector = FVector::Right * MoveSpeed;
	}
	MoveUpdate(_DeltaTime);

	// 또 공격?
	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault, Jump_Muzzle);
	}

	// 0.5초가 지났다.
	if (BusterDelayTime >= BusterDelayTimeMax)
	{
		StateChange(EEgseuState::IdleJump);
		return;
	}

	// 바닥이네? -> 낮은 바닥...
	Color8Bit Color = UContentsGlobalData::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
	{
		JumpVector = FVector::Zero;
		StateChange(EEgseuState::IdleJump_End);
		return;
	}

	if (true == PlayerRender->IsCurAnimationEnd())
	{
		StateChange(EEgseuState::JumpAttack_Up_Loop);
		return;
	}
}

void AEgseu::JumpAttack_Up_LoopStart()
{
	if (BusterDelayTime == 0.0f) // 공중 차지 공격
	{
		int CurFrame = PlayerRender->GetCurAnimationFrame();
		PlayerRender->ChangeAnimation(GetAnimationName("Jump_Ing_Attack"), false, CurFrame);
	}
	else // 이전 상태에서 이어서 들어옴.
	{
		PlayerRender->ChangeAnimation(GetAnimationName("Jump_Ing_Attack"));
	}
	DirCheck();
}

void AEgseu::JumpAttack_Up_Loop(float _DeltaTime)
{
	BusterDelayTime += _DeltaTime;
	DirCheck();
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		RunVector = FVector::Left * MoveSpeed;
	}
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		RunVector = FVector::Right * MoveSpeed;
	}

	MoveUpdate(_DeltaTime);

	// 0.5초 지남.
	if (BusterDelayTime >= BusterDelayTimeMax)
	{
		StateChange(EEgseuState::RunJump_Loop);
		return;
	}

	// 또 쏴?
	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault, Jump_Muzzle);
	}

	bool WallChecl = CalWallCheck();
	if (true == WallChecl)
	{
		StateChange(EEgseuState::WallCling);
		return;
	}

	// 땅에 닿음
	Color8Bit Color = UContentsGlobalData::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
	{
		JumpVector = FVector::Zero;
		if (BusterDelayTime == 0.0f)
		{
			StateChange(EEgseuState::RunJump_End);
			return;
		}
		else
		{
			StateChange(EEgseuState::RunJumpAttack_Up_End);
			return;
		}
	}
}

void AEgseu::JumpAttack_Up_EndStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Jump_End_Attack"));
}

void AEgseu::JumpAttack_Up_End(float _DeltaTime)
{
	BusterDelayTime += _DeltaTime;
	JumpVector = FVector::Zero;
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		RunVector = FVector::Left * MoveSpeed;
	}
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		RunVector = FVector::Right * MoveSpeed;
	}
	MoveUpdate(_DeltaTime);

	if (BusterDelayTime >= BusterDelayTimeMax)
	{
		StateChange(EEgseuState::IdleJump_End);
		return;
	}

	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault, Jump_Muzzle);
	}

	if (true == PlayerRender->IsCurAnimationEnd())
	{
		StateChange(EEgseuState::Idle);
		return;
	}
}
#pragma endregion

#pragma region IdleAttack Down
void AEgseu::IdleAttack_DownStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Idle_Attack_Start"));
}

void AEgseu::IdleAttack_Down(float _DeltaTime)
{
	if (true == UEngineInput::IsPress(VK_LEFT) ||
		true == UEngineInput::IsPress(VK_RIGHT))
	{
		BusterDelayTime += _DeltaTime;
		StateChange(EEgseuState::IdleRun);
		return;
	}
	
	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault, Idle_Muzzle);
	}

	if (true == UEngineInput::IsDown('C'))
	{
		StateChange(EEgseuState::IdleJump);
		return;
	}

	if (true == PlayerRender->IsCurAnimationEnd())
	{
		StateChange(EEgseuState::IdleAttack_Down_Loop);
		BusterDelayTime += _DeltaTime;
		return;
	}
}

void AEgseu::IdleAttack_Down_LoopStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Idle_Attack_Loop"));
}

void AEgseu::IdleAttack_Down_Loop(float _DeltaTime) // Down 이던 Press 이던 여기 까지는 들어와야 함.
{
	BusterDelayTime += _DeltaTime;

	if (true == UEngineInput::IsPress(VK_LEFT) ||
		true == UEngineInput::IsPress(VK_RIGHT))
	{
		StateChange(EEgseuState::IdleRun_Loop);
		return;
	}

	// 모션 중에 X 키가 눌리면,
	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault, Idle_Muzzle);
	}

	// 0.5초가 지났다면,
	if (BusterDelayTimeMax <= BusterDelayTime) // 차지 모션 대기 시간.
	{
		StateChange(EEgseuState::IdleAttack_Down_End);
		return;
	}

	if (true == UEngineInput::IsDown('C'))
	{
		StateChange(EEgseuState::IdleJump);
		return;
	}
}

void AEgseu::IdleAttack_Down_EndStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Idle_Attack_End"));
	BusterDelayTime = 0.0f;
}

void AEgseu::IdleAttack_Down_End(float _DeltaTime)
{
	if (true == PlayerRender->IsCurAnimationEnd())
	{
		StateChange(EEgseuState::Idle);
		return;
	}
}
#pragma endregion

#pragma region IdleAttack Up
void AEgseu::IdleAttack_UpStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Idle_Attack_Start"));
}

void AEgseu::IdleAttack_Up(float _DeltaTime)
{
	BusterDelayTime += _DeltaTime;

	if (true == UEngineInput::IsDown(VK_RIGHT) || true == UEngineInput::IsDown(VK_RIGHT))
	{
		StateChange(EEgseuState::RunAttack_Up_Loop);
		return;
	}

	if (true == PlayerRender->IsCurAnimationEnd())
	{
		StateChange(EEgseuState::IdleAttack_Up_Loop);
		BusterDelayTime += _DeltaTime;
		return;
	}
}

void AEgseu::IdleAttack_Up_LoopStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Idle_Attack_Loop"));
}

void AEgseu::IdleAttack_Up_Loop(float _DeltaTime)
{
	BusterDelayTime += _DeltaTime;

	// 0.5초가 지났다면,
	if (BusterDelayTimeMax <= BusterDelayTime) // 차지 모션 대기 시간.
	{
		StateChange(EEgseuState::IdleAttack_Up_End);
		return;
	}

	// 모션 중에 X 키가 눌리면,
	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault, Idle_Muzzle);
	}
}

void AEgseu::IdleAttack_Up_EndStart()
{
	if (BusterDelayTime == 0.0f)
	{
		int CurFrame = PlayerRender->GetCurAnimationFrame();
		PlayerRender->ChangeAnimation(GetAnimationName("Idle_Attack_End"), false, CurFrame);
	}
	else
	{
		PlayerRender->ChangeAnimation(GetAnimationName("Idle_Attack_End"));
	}
}

void AEgseu::IdleAttack_Up_End(float _DeltaTime)
{
	if (true == PlayerRender->IsCurAnimationEnd())
	{
		StateChange(EEgseuState::Idle);
		return;
	}
}
#pragma endregion

#pragma region IdleDash
void AEgseu::IdleDashStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Dash_Start"));
}

void AEgseu::IdleDash_LoopStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Dash_Loop"));
}

void AEgseu::IdleDash_EndStart()
{
	if (BusterDelayTime == 0.0f)
	{
		PlayerRender->ChangeAnimation(GetAnimationName("Dash_End"));
	}
	else // 공격 하다 왔음.
	{
		BusterDelayTime = 0.0f;
		int CurFrame = PlayerRender->GetCurAnimationFrame();
		PlayerRender->ChangeAnimation(GetAnimationName("Dash_End"), false, CurFrame);
	}
}

void AEgseu::IdleDash(float _DeltaTime)
{
	DashTime += _DeltaTime;
	switch (DirState)
	{
	case EActorDir::Right :
		DashVector = FVector::Right * DashSpeed;
		break;
	case EActorDir::Left :
		DashVector = FVector::Left * DashSpeed;
		break;
	default:
		break;
	}

	MoveUpdate(_DeltaTime);

	if (true == UEngineInput::IsPress('Z'))
	{
		StateChange(EEgseuState::IdleDash_Loop);
		return; 
	}
}

void AEgseu::IdleDash_Loop(float _DeltaTime)
{
	DashTime += _DeltaTime;
	switch (DirState)
	{
	case EActorDir::Right:
		DashVector = FVector::Right * DashSpeed;
		break;
	case EActorDir::Left:
		DashVector = FVector::Left * DashSpeed;
		break;
	default:
		break;
	}

	MoveUpdate(_DeltaTime);

	// 점프 (나중에 추가하자. 급한 것은 아니다. 대쉬점프를 참고하면 쉽게 할 수 있다.)

	// 도중에 대쉬 끊기.
	if (true == UEngineInput::IsUp('Z'))
	{
		DashTime = 0.0f;
		DashVector = FVector::Zero;
		if (true == UEngineInput::IsPress(VK_LEFT) || true == UEngineInput::IsPress(VK_RIGHT))
		{
			StateChange(EEgseuState::IdleRun_Loop);
			return;
		}
		else
		{
			StateChange(EEgseuState::IdleDash_End);
			return;
		}
	}

	if (0.5f <= DashTime)
	{
		DashTime = 0.0f;
		if (true == UEngineInput::IsPress(VK_LEFT) || true == UEngineInput::IsPress(VK_RIGHT))
		{
			DashVector = FVector::Zero;
			StateChange(EEgseuState::IdleRun_Loop);
			return;
		}
		else
		{
			StateChange(EEgseuState::IdleDash_End);
			return;
		}
	}
}

void AEgseu::IdleDash_End(float _DeltaTime)
{
	DashVector = FVector::Zero;

	// 감속 기능 추가하는 디테일.

	if (true == PlayerRender->IsCurAnimationEnd())
	{
		StateChange(EEgseuState::Idle);
		return;
	}
}
#pragma endregion

#pragma region DashAttack Idle->Dash -- TODO
void AEgseu::DashAttackStart()
{
}

void AEgseu::DashAttack_LoopStart()
{
}

void AEgseu::DashAttack_EndStart()
{
}

void AEgseu::DashAttack(float _DeltaTime)
{
}

void AEgseu::DashAttack_Loop(float _DeltaTime)
{
}

void AEgseu::DashAttack_End(float _DeltaTime)
{
}
#pragma endregion

#pragma region Run / Idle->Run
void AEgseu::IdleRunStart()
{
	BusterDelayTime = 0.0f;
	PlayerRender->ChangeAnimation(GetAnimationName("Run_Ready"));
	DirCheck();
}

void AEgseu::IdleRun(float _DeltaTime)
{
	DirCheck();
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		RunVector = FVector::Left * MoveSpeed;
	}
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		RunVector = FVector::Right * MoveSpeed;
	}

	MoveUpdate(_DeltaTime);

	// Down
	if (true == UEngineInput::IsDown('X'))
	{
		BusterCreate(EBusterState::CreateDefault, Run_Muzzle);
		BusterDelayTime = 0.0f;
		StateChange(EEgseuState::RunAttack_Down);
		return;
	}

	// Up
	if (true == UEngineInput::IsUp('X'))
	{
		if (BusterChargTime <= 0.8f)
		{
			return;
		}
		if (1.0f <= BusterChargTime && BusterChargTime < 2.0f)
		{
			BusterCreate(EBusterState::CreateMiddle, Run_Muzzle);
		}
		else if (2.0f <= BusterChargTime)
		{
			BusterCreate(EBusterState::CreatePull, Run_Muzzle);
		}
		BusterDelayTime = 0.0f;
		StateChange(EEgseuState::RunAttack_Up);
		return;
	}

	if (true == PlayerRender->IsCurAnimationEnd())
	{
		StateChange(EEgseuState::IdleRun_Loop);
		return;
	}
}

void AEgseu::IdleRun_LoopStart()
{
	JumpVector = FVector::Zero; // 벽 타고 왔을 때 점프 Vector 초기화.
	
	PlayerRender->ChangeAnimation(GetAnimationName("Run"));
	DirCheck();
}

void AEgseu::IdleRun_Loop(float _DeltaTime)
{
	DirCheck();
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		RunVector = FVector::Left * MoveSpeed;
	}
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		RunVector = FVector::Right * MoveSpeed;
	}
	MoveUpdate(_DeltaTime);
		
	// 점프
	if (true == UEngineInput::IsDown('C'))
	{
		StateChange(EEgseuState::RunJump);
		return;
	}

	// 공격
	if (true == UEngineInput::IsDown('X'))
	{
		BusterChargTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault, Run_Muzzle);
		StateChange(EEgseuState::RunAttack_Down_Loop);
		return;
	}

	// 발싸!!
	if (true == UEngineInput::IsUp('X'))
	{
		if (BusterChargTime <= 0.8f)
		{
			return;
		}
		if (1.0f <= BusterChargTime && BusterChargTime < 2.0f)
		{
			BusterCreate(EBusterState::CreateMiddle, Run_Muzzle);
		}
		else if (2.0f <= BusterChargTime)
		{
			BusterCreate(EBusterState::CreatePull, Run_Muzzle);
		}
		BusterDelayTime = 0.0f;
		StateChange(EEgseuState::RunAttack_Up_Loop);
		return;
	}

	// 대쉬
	if (true == UEngineInput::IsDown('Z'))
	{
		StateChange(EEgseuState::RunDash);
		return;
	}

	if (true == UEngineInput::IsFree(VK_LEFT) &&
		true == UEngineInput::IsFree(VK_RIGHT) &&
		true == UEngineInput::IsFree(VK_UP) &&
		true == UEngineInput::IsFree(VK_DOWN))
	{
		//StateChange(EEgseuState::IdleRun_End);
		StateChange(EEgseuState::Idle);
		return;
	}

	Color8Bit Color = UContentsGlobalData::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit(0, 0, 0, 0))
	{
		StateChange(EEgseuState::IdleJump_Loop);
		return;
	}
}

//void AEgseu::IdleRun_EndStart()
//{
//	PlayerRender->ChangeAnimation(GetAnimationName("Idle"));
//	DirCheck();
//}
//
//void AEgseu::IdleRun_End(float _DeltaTime)
//{
//	StateChange(EEgseuState::Idle);
//	return;
//}
#pragma endregion

#pragma region RunAttack Down / IdleRun
void AEgseu::RunAttack_DownStart()
{
	int CurFrame = PlayerRender->GetCurAnimationFrame();
	PlayerRender->ChangeAnimation(GetAnimationName("Run_Attack_Start"), false, CurFrame);
	DirCheck();
}

void AEgseu::RunAttack_Down(float _DeltaTime)
{
	BusterDelayTime += _DeltaTime;

	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		RunVector = FVector::Left * MoveSpeed;
	}
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		RunVector = FVector::Right * MoveSpeed;
	}

	MoveUpdate(_DeltaTime);

	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault, Run_Muzzle);
	}

	// 딜레이가 끝나면,
	if (BusterDelayTime >= BusterDelayTimeMax)
	{
		StateChange(EEgseuState::IdleRun);
		return;
	}

	if (true == PlayerRender->IsCurAnimationEnd())
	{
		StateChange(EEgseuState::IdleRun); // 지금은 어색할 수 있다.
		return;
	}
	
	// 움직임을 멈추면 Idle로.
	if (true == UEngineInput::IsFree(VK_LEFT) &&
		true == UEngineInput::IsFree(VK_RIGHT) &&
		true == UEngineInput::IsFree(VK_UP) &&
		true == UEngineInput::IsFree(VK_DOWN))
	{
		StateChange(EEgseuState::Idle);
		return;
	}
}

void AEgseu::RunAttack_Down_LoopStart()
{
	int CurFrame = PlayerRender->GetCurAnimationFrame();
	PlayerRender->ChangeAnimation(GetAnimationName("Run_Attack_Loop"), false, CurFrame);
	DirCheck();
	BusterDelayTime = 0.0f;
}

void AEgseu::RunAttack_Down_Loop(float _DeltaTime)
{
	BusterDelayTime += _DeltaTime;

	DirCheck();
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		RunVector = FVector::Left * MoveSpeed;
	}
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		RunVector = FVector::Right * MoveSpeed;
	}

	MoveUpdate(_DeltaTime);

	// 또 눌러?
	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault, Run_Muzzle);
	}

	if (true == UEngineInput::IsDown('C'))
	{
		BusterDelayTime = 0.0f;
		StateChange(EEgseuState::RunJump);
		return;
	}

	// 대쉬!
	if (true == UEngineInput::IsDown('Z'))
	{
		StateChange(EEgseuState::RunDashAttack_Down);
		return;
	}

	if (true == UEngineInput::IsFree(VK_LEFT) &&
		true == UEngineInput::IsFree(VK_RIGHT) &&
		true == UEngineInput::IsFree(VK_UP) &&
		true == UEngineInput::IsFree(VK_DOWN))
	{
		StateChange(EEgseuState::Idle);
		return;
	}

	// 0.5초가 지나면,
	if (BusterDelayTimeMax <= BusterDelayTime)
	{
		StateChange(EEgseuState::IdleRun_Loop);
		return;
	}
}

//void AEgseu::RunAttack_Down_EndStart()
//{
//	BusterDelayTime = 0.0f;
//	int CurFrame = PlayerRender->GetCurAnimationFrame();
//	PlayerRender->ChangeAnimation(GetAnimationName(""), false, CurFrame);
//	//PlayerRender->ChangeAnimation(GetAnimationName("Run"));
//}
//
//void AEgseu::RunAttack_Down_End(float _DeltaTime)
//{
//	StateChange(EEgseuState::IdleRun_Loop);
//	return;
//}
#pragma endregion

#pragma region RunAttack Up
void AEgseu::RunAttack_UpStart()
{
	int CurFrame = PlayerRender->GetCurAnimationFrame();
	PlayerRender->ChangeAnimation(GetAnimationName("Run"), false, CurFrame);
}
void AEgseu::RunAttack_Up(float _DeltaTime)
{
	BusterDelayTime += _DeltaTime;
	DirCheck();
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		RunVector = FVector::Left * MoveSpeed;
	}
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		RunVector = FVector::Right * MoveSpeed;
	}
	MoveUpdate(_DeltaTime);

	if (true == PlayerRender->IsCurAnimationEnd())
	{
		StateChange(EEgseuState::RunAttack_Up_Loop);
		return;
	}
}

void AEgseu::RunAttack_Up_LoopStart()
{
	if (BusterDelayTime == 0.0f) // Run Loop 에서 들어옴.
	{
		int CurFrame = PlayerRender->GetCurAnimationFrame();
		PlayerRender->ChangeAnimation(GetAnimationName("Run_Attack_Loop"), false, CurFrame);
	}
	else
	{
		PlayerRender->ChangeAnimation(GetAnimationName("Run_Attack_Loop"));
	}
}
void AEgseu::RunAttack_Up_Loop(float _DeltaTime)
{
	BusterDelayTime += _DeltaTime;
	DirCheck();
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		RunVector = FVector::Left * MoveSpeed;
	}
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		RunVector = FVector::Right * MoveSpeed;
	}
	MoveUpdate(_DeltaTime);

	if (true == UEngineInput::IsDown('X'))
	{
		StateChange(EEgseuState::RunAttack_Down_Loop);
		return;
	}

	if (true == UEngineInput::IsFree(VK_LEFT) &&
		true == UEngineInput::IsFree(VK_RIGHT) &&
		true == UEngineInput::IsFree(VK_UP) &&
		true == UEngineInput::IsFree(VK_DOWN))
	{
		if (BusterDelayTime <= BusterDelayTimeMax)
		{
			StateChange(EEgseuState::IdleAttack_Up_Loop);
			return;
		}
	}

	// 0.5초가 지나면,
	if (BusterDelayTime >= BusterDelayTimeMax)
	{
		StateChange(EEgseuState::IdleRun_Loop);
		return;
	}
}
//void AEgseu::RunAttack_Up_EndStart()
//{
//}
//void AEgseu::RunAttack_Up_End(float _DeltaTime)
//{
//}
#pragma endregion

#pragma region RunDash
void AEgseu::RunDashStart() //////////
{
	if (State == EEgseuState::IdleRun_Loop)
	{
		PlayerRender->ChangeAnimation(GetAnimationName("Dash_Start"));
	}
	else if (State == EEgseuState::RunDashAttack_Down)
	{
		int CurFrame = PlayerRender->GetCurAnimationFrame();
		PlayerRender->ChangeAnimation(GetAnimationName("Dash_Start"), false, CurFrame);
	}
	else if (State == EEgseuState::RunDashAttack_Up)
	{
		int CurFrame = PlayerRender->GetCurAnimationFrame();
		PlayerRender->ChangeAnimation(GetAnimationName("Dash_Start"), false, CurFrame);
	}

	//if (BusterDelayTime == 0.0f)
	//{
	//	PlayerRender->ChangeAnimation(GetAnimationName("Dash_Start"));
	//}
	//else // 공격 하다가 왔으면,
	//{
	//	int CurFrame = PlayerRender->GetCurAnimationFrame();
	//	PlayerRender->ChangeAnimation(GetAnimationName("Dash_Start"), false, CurFrame);
	//}
}
void AEgseu::RunDash(float _DeltaTime)
{
	DashTime += _DeltaTime;

	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		DashVector = FVector::Right * DashSpeed;
	}
	else if (true == UEngineInput::IsPress(VK_LEFT))
	{
		DashVector = FVector::Left * DashSpeed;
	}
	RunVector = FVector::Zero;
	MoveUpdate(_DeltaTime);

	if (true == UEngineInput::IsUp('Z'))
	{
		DashVector = FVector::Zero;
		StateChange(EEgseuState::IdleRun_Loop);
		return;
	}

	// 점프
	if (true == UEngineInput::IsDown('C'))
	{
		DashTime = 0.0f;
		StateChange(EEgseuState::RunDashJump);
		return;
	}

	// 공격
	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault, Run_Muzzle);
		StateChange(EEgseuState::RunDashAttack_Down);
		return;
	}

	// 차지 공격
	if (true == UEngineInput::IsUp('X'))
	{
		if (BusterChargTime <= 0.8f) // 순간 중복 방지.
		{
			return;
		}
		if (1.0f <= BusterChargTime && BusterChargTime < 2.0f)
		{
			BusterCreate(EBusterState::CreateMiddle, Run_Muzzle);
		}
		else if (2.0f <= BusterChargTime)
		{
			BusterCreate(EBusterState::CreatePull, Run_Muzzle);
		}
		BusterDelayTime = 0.0f;
		StateChange(EEgseuState::RunDashAttack_Up);
		return;
	}


	if (true == UEngineInput::IsPress('Z') && true == PlayerRender->IsCurAnimationEnd())
	{
		StateChange(EEgseuState::RunDash_Loop);
		return;
	}
}

void AEgseu::RunDash_LoopStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Dash_Loop"));
}
void AEgseu::RunDash_Loop(float _DeltaTime)
{
	DashTime += _DeltaTime;

	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		DashVector = FVector::Right * DashSpeed;
	}
	else if (true == UEngineInput::IsPress(VK_LEFT))
	{
		DashVector = FVector::Left * DashSpeed;
	}

	MoveUpdate(_DeltaTime);

	// 점프
	if (true == UEngineInput::IsDown('C'))
	{
		DashTime = 0.0f;
		StateChange(EEgseuState::RunDashJump);
		return;
	}

	// 공격
	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		StateChange(EEgseuState::RunDashAttack_Down_Loop);
		BusterCreate(EBusterState::CreateDefault, Dash_Muzzle);
		return;
	}

	// 차지
	if (true == UEngineInput::IsUp('X'))
	{
		if (BusterChargTime <= 0.8f) // 순간 중복 방지.
		{
			return;
		}
		if (1.0f <= BusterChargTime && BusterChargTime < 2.0f)
		{
			BusterCreate(EBusterState::CreateMiddle, Dash_Muzzle);
		}
		else if (2.0f <= BusterChargTime)
		{
			BusterCreate(EBusterState::CreatePull, Dash_Muzzle);
		}
		BusterDelayTime = 0.0f;
		StateChange(EEgseuState::RunDashAttack_Up_Loop);
		return;
	}

	// 대쉬 끝!
	if (true == UEngineInput::IsUp('Z'))
	{
		DashTime = 0.0f;
		if (true == UEngineInput::IsPress(VK_LEFT) || true == UEngineInput::IsPress(VK_RIGHT))
		{
			DashVector = FVector::Zero;
			StateChange(EEgseuState::IdleRun_Loop);
			return;
		}
		else
		{
			StateChange(EEgseuState::RunDash_End);
			return;
		}
	}

	// 대쉬 지속 시간
	if (DashTimeMax <= DashTime)
	{
		DashTime = 0.0f;
		if (true == UEngineInput::IsPress(VK_LEFT) || true == UEngineInput::IsPress(VK_RIGHT))
		{
			DashVector = FVector::Zero;
			StateChange(EEgseuState::IdleRun_Loop);
			return;
		}
		else
		{
			StateChange(EEgseuState::RunDash_End);
			return;
		}
	}
}

void AEgseu::RunDash_EndStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Dash_End"));
}
void AEgseu::RunDash_End(float _DeltaTime)
{
	DashVector = FVector::Zero;

	// 감속 기능 추가하는 디테일.


	// 공격
	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		StateChange(EEgseuState::RunDashAttack_Down_End);
		BusterCreate(EBusterState::CreateDefault, Dash_Muzzle);
		return;
	}

	// 발싸!!
	if (true == UEngineInput::IsUp('X'))
	{
		if (BusterChargTime <= 0.8f)
		{
			return;
		}
		if (1.0f <= BusterChargTime && BusterChargTime < 2.0f)
		{
			BusterCreate(EBusterState::CreateMiddle, Dash_Muzzle);
		}
		else if (2.0f <= BusterChargTime)
		{
			BusterCreate(EBusterState::CreatePull, Dash_Muzzle);
		}
		BusterDelayTime = 0.0f;
		StateChange(EEgseuState::RunDashAttack_Up_End);
		return;
	}

	if (true == PlayerRender->IsCurAnimationEnd())
	{
		StateChange(EEgseuState::Idle);
		return;
	}
}
#pragma endregion

#pragma region RunDashAttack Down
void AEgseu::RunDashAttack_DownStart()
{
	DashTime = 0.0f;
	if (BusterDelayTime == 0)
	{
		int CurFrame = PlayerRender->GetCurAnimationFrame();
		PlayerRender->ChangeAnimation(GetAnimationName("Dash_Attack_Start"), false, CurFrame);
	}
	else // 공격 하다가 왔음.
	{
		PlayerRender->ChangeAnimation(GetAnimationName("Dash_Attack_Start"));
	}
}
void AEgseu::RunDashAttack_Down(float _DeltaTime)
{
	DashTime += _DeltaTime;
	BusterDelayTime += _DeltaTime;

	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		DashVector = FVector::Right * DashSpeed;
	}
	else if (true == UEngineInput::IsPress(VK_LEFT))
	{
		DashVector = FVector::Left * DashSpeed;
	}
	RunVector = FVector::Zero;
	MoveUpdate(_DeltaTime);

	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault, Dash_Muzzle);
	}
	
	// 점프


	// 0.5
	if (BusterDelayTime >= BusterDelayTimeMax)
	{
		StateChange(EEgseuState::RunDash);
		return;
	}

	if (true == UEngineInput::IsPress('Z') && true == PlayerRender->IsCurAnimationEnd())
	{
		if (BusterDelayTime != 0.0f)
		{
			StateChange(EEgseuState::RunDashAttack_Down_Loop);
			return;
		}
		else
		{
			StateChange(EEgseuState::RunDash_Loop);
			return;
		}
	}
}

void AEgseu::RunDashAttack_Down_LoopStart()
{
	if (BusterDelayTime == 0.0f)
	{
		int CurFrame = PlayerRender->GetCurAnimationFrame();
		PlayerRender->ChangeAnimation(GetAnimationName("Dash_Attack_Loop"), false, CurFrame);
	}
	else
	{
		PlayerRender->ChangeAnimation(GetAnimationName("Dash_Attack_Loop"));
	}
}
void AEgseu::RunDashAttack_Down_Loop(float _DeltaTime)
{
	BusterDelayTime += _DeltaTime;
	DashTime += _DeltaTime;

	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		DashVector = FVector::Right * DashSpeed;
	}
	else if (true == UEngineInput::IsPress(VK_LEFT))
	{
		DashVector = FVector::Left * DashSpeed;
	}
	RunVector = FVector::Zero;
	MoveUpdate(_DeltaTime);

	// 또 공격!
	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault, Dash_Muzzle);
	}

	// 0.5 초가 지났어.
	if (BusterDelayTime >= BusterDelayTimeMax)
	{
		StateChange(EEgseuState::IdleDash_Loop);
		return;
	}

	// 대쉬 도중 종료.
	if (true == UEngineInput::IsUp('Z'))
	{
		DashTime = 0.0f;
		DashVector = FVector::Zero;

		// 방향키 좌우를 누르고 있다면,
		if (true == UEngineInput::IsPress(VK_LEFT) || true == UEngineInput::IsPress(VK_RIGHT))
		{
			if (BusterDelayTime != 0.0f)
			{
				StateChange(EEgseuState::RunAttack_Down_Loop);
				return;
			}
			else
			{
				StateChange(EEgseuState::IdleRun_Loop);
				return;
			}
		}
		else // 방향키를 누르고 있지 않다면,
		{
			if (BusterDelayTime != 0.0f)
			{
				StateChange(EEgseuState::IdleAttack_Down_End);
				return;
			}
			else
			{
				StateChange(EEgseuState::IdleDash_End);
				return;
			}
		}
	}

	// 대쉬 지속 시간 끝.
	if (DashTimeMax <= DashTime)
	{
		DashVector = FVector::Zero;
		DashTime = 0.0f;
		// 방향키 좌우를 누르고 있다면,
		if (true == UEngineInput::IsPress(VK_LEFT) || true == UEngineInput::IsPress(VK_RIGHT))
		{
			if (BusterDelayTime != 0.0f)
			{
				StateChange(EEgseuState::RunAttack_Down_Loop);
				return;
			}
			else
			{
				StateChange(EEgseuState::IdleRun_Loop);
				return;
			}
		}
		else // 방향키를 누르고 있지 않다면,
		{
			if (BusterDelayTime != 0.0f)
			{
				StateChange(EEgseuState::RunDashAttack_Down_End);
				return;
			}
			else
			{
				StateChange(EEgseuState::IdleDash_End);
				return;
			}
		}
	}
}

void AEgseu::RunDashAttack_Down_EndStart()
{
	if (BusterDelayTime == 0.0f) // RunDash_End 에서 넘어 왔으니 바꿔 줘야 함.
	{
		int CurFrame = PlayerRender->GetCurAnimationFrame();
		PlayerRender->ChangeAnimation(GetAnimationName("Dash_Attack_End"), false, CurFrame);
	}
	else // Attack_Down_Loop 에서 넘어 옴.
	{
		PlayerRender->ChangeAnimation(GetAnimationName("Dash_Attack_End"));
	}
}
void AEgseu::RunDashAttack_Down_End(float _DeltaTime)
{
	DashVector = FVector::Zero;

	// 0.5 초가 지났어.
	if (BusterDelayTime >= BusterDelayTimeMax)
	{
		StateChange(EEgseuState::IdleDash_End);
		return;
	}

	if (true == PlayerRender->IsCurAnimationEnd())
	{
		StateChange(EEgseuState::Idle);
		return;
	}
}
#pragma endregion

#pragma region RunDashAttack Up
void AEgseu::RunDashAttack_UpStart()
{
	DashTime = 0.0f;
	int CurFrame = PlayerRender->GetCurAnimationFrame();
	PlayerRender->ChangeAnimation(GetAnimationName("Dash_Attack_Start"), false, CurFrame);
}

void AEgseu::RunDashAttack_Up(float _DeltaTime)
{
	DashTime += _DeltaTime;
	BusterDelayTime += _DeltaTime;

	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		DashVector = FVector::Right * DashSpeed;
	}
	else if (true == UEngineInput::IsPress(VK_LEFT))
	{
		DashVector = FVector::Left * DashSpeed;
	}
	RunVector = FVector::Zero;
	MoveUpdate(_DeltaTime);

	// 점프


	// 공격
	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault, Dash_Muzzle);
	}

	// 0.5
	if (BusterDelayTime >= BusterDelayTimeMax)
	{
		StateChange(EEgseuState::RunDash);
		return;
	}

	if (true == UEngineInput::IsPress('Z') && true == PlayerRender->IsCurAnimationEnd())
	{
		if (BusterDelayTime != 0.0f)
		{
			StateChange(EEgseuState::RunDashAttack_Up_Loop);
			return;
		}
		else
		{
			StateChange(EEgseuState::RunDash_Loop);
			return;
		}
	}
}
#pragma endregion

#pragma region RunDashAttack Up Loop
void AEgseu::RunDashAttack_Up_LoopStart()
{
	if (BusterDelayTime == 0.0f)
	{
		int CurFrame = PlayerRender->GetCurAnimationFrame();
		PlayerRender->ChangeAnimation(GetAnimationName("Dash_Attack_Loop"), false, CurFrame);
	}
	else
	{
		PlayerRender->ChangeAnimation(GetAnimationName("Dash_Attack_Loop"));
	}
}

void AEgseu::RunDashAttack_Up_Loop(float _DeltaTime)
{
	BusterDelayTime += _DeltaTime;
	DashTime += _DeltaTime;

	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		DashVector = FVector::Right * DashSpeed;
	}
	else if (true == UEngineInput::IsPress(VK_LEFT))
	{
		DashVector = FVector::Left * DashSpeed;
	}
	RunVector = FVector::Zero;
	MoveUpdate(_DeltaTime);

	// 또 공격!
	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault, Dash_Muzzle);
	}

	// 0.5 초가 지났어.
	if (BusterDelayTime >= BusterDelayTimeMax)
	{
		StateChange(EEgseuState::IdleDash_Loop);
		return;
	}

	// 대쉬 도중 종료.
	if (true == UEngineInput::IsUp('Z'))
	{
		DashTime = 0.0f;
		DashVector = FVector::Zero;

		// 방향키 좌우를 누르고 있다면,
		if (true == UEngineInput::IsPress(VK_LEFT) || true == UEngineInput::IsPress(VK_RIGHT))
		{
			if (BusterDelayTime != 0.0f)
			{
				StateChange(EEgseuState::RunAttack_Up_Loop);
				return;
			}
			else
			{
				StateChange(EEgseuState::IdleRun_Loop);
				return;
			}
		}
		else // 방향키를 누르고 있지 않다면,
		{
			if (BusterDelayTime != 0.0f)
			{
				StateChange(EEgseuState::IdleAttack_Up_End);
				return;
			}
			else
			{
				StateChange(EEgseuState::IdleDash_End);
				return;
			}
		}
	}

	// 대쉬 지속 시간 끝.
	if (DashTimeMax <= DashTime)
	{
		DashVector = FVector::Zero;
		DashTime = 0.0f;
		// 방향키 좌우를 누르고 있다면,
		if (true == UEngineInput::IsPress(VK_LEFT) || true == UEngineInput::IsPress(VK_RIGHT))
		{
			if (BusterDelayTime != 0.0f)
			{
				StateChange(EEgseuState::RunAttack_Up_Loop);
				return;
			}
			else
			{
				StateChange(EEgseuState::IdleRun_Loop);
				return;
			}
		}
		else // 방향키를 누르고 있지 않다면,
		{
			if (BusterDelayTime != 0.0f)
			{
				StateChange(EEgseuState::RunDashAttack_Up_End);
				return;
			}
			else
			{
				StateChange(EEgseuState::IdleDash_End);
				return;
			}
		}
	}
}
#pragma endregion


void AEgseu::RunDashAttack_Up_EndStart()
{
	if (BusterDelayTime == 0.0f) // RunDash_End 에서 넘어 왔으니 바꿔 줘야 함.
	{
		int CurFrame = PlayerRender->GetCurAnimationFrame();
		PlayerRender->ChangeAnimation(GetAnimationName("Dash_Attack_End"), false, CurFrame);
	}
	else // Attack_Up_Loop 에서 넘어 옴.
	{
		PlayerRender->ChangeAnimation(GetAnimationName("Dash_Attack_End"));
	}
}

void AEgseu::RunDashAttack_Up_End(float _DeltaTime)
{
	DashVector = FVector::Zero;

	// 0.5 초가 지났어.
	if (BusterDelayTime >= BusterDelayTimeMax)
	{
		StateChange(EEgseuState::IdleDash_End);
		return;
	}

	if (true == PlayerRender->IsCurAnimationEnd())
	{
		StateChange(EEgseuState::Idle);
		return;
	}
}
#pragma endregion

#pragma region RunDashJump
void AEgseu::RunDashJumpStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Jump_Start"));
	JumpVector = JumpPower;
	DirCheck();
}
void AEgseu::RunDashJump(float _DeltaTime)
{
	DirCheck();
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		DashVector = FVector::Left * DashSpeed;
	}
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		DashVector = FVector::Right * DashSpeed;
	}
	
	Color8Bit CeilingColor = UContentsGlobalData::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY() - 130, Color8Bit::MagentaA);
	if (CeilingColor == Color8Bit(255, 0, 255, 0))
	{
		JumpVector = FVector::Zero;
	}

	RunVector = FVector::Zero;
	MoveUpdate(_DeltaTime);

	// 공격
	if (true == UEngineInput::IsDown('X'))
	{
		BusterCreate(EBusterState::CreateDefault, Dash_Muzzle);
		StateChange(EEgseuState::RunDashJumpAttack_Down);
		return;
	}

	// 차지 공격
	if (true == UEngineInput::IsUp('X'))
	{
		if (BusterChargTime <= 0.8f)
		{
			return;
		}
		if (1.0f <= BusterChargTime && BusterChargTime < 2.0f)
		{
			BusterCreate(EBusterState::CreateMiddle, Dash_Muzzle);
		}
		else if (2.0f <= BusterChargTime)
		{
			BusterCreate(EBusterState::CreatePull, Dash_Muzzle);
		}
		BusterDelayTime = 0.0f;
		StateChange(EEgseuState::RunDashJumpAttack_Up);
		return;
	}

	// 애니메이션이 끝나면,
	if (true == PlayerRender->IsCurAnimationEnd())
	{
		StateChange(EEgseuState::RunDashJump_Loop);
		return;
	}
}

void AEgseu::RunDashJump_LoopStart()
{
	if (BusterDelayTime == 0.0f)
	{
		PlayerRender->ChangeAnimation(GetAnimationName("Jumping"));
	}
	else
	{
		int CurFrame = PlayerRender->GetCurAnimationFrame();
		PlayerRender->ChangeAnimation(GetAnimationName("Jumping"), false, CurFrame);
	}
	DirCheck();
}
void AEgseu::RunDashJump_Loop(float _DeltaTime)
{
	DirCheck();
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		DashVector = FVector::Left * DashSpeed;
	}
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		DashVector = FVector::Right * DashSpeed;
	}

	Color8Bit CeilingColor = UContentsGlobalData::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY() - 130, Color8Bit::MagentaA);
	if (CeilingColor == Color8Bit(255, 0, 255, 0))
	{
		JumpVector = FVector::Zero;
	}

	RunVector = FVector::Zero;
	MoveUpdate(_DeltaTime);

	// 점프 중 벽 체크
	bool WallChecl = CalWallCheck();
	if (true == WallChecl)
	{
		StateChange(EEgseuState::WallCling);
		return;
	}

	// 공격
	if (true == UEngineInput::IsDown('X'))
	{
		BusterCreate(EBusterState::CreateDefault, Jump_Muzzle);
		StateChange(EEgseuState::RunDashJumpAttack_Down_Loop);
		return;
	}

	// 차지 공격.
	if (true == UEngineInput::IsUp('X'))
	{
		if (BusterChargTime <= 0.8f)
		{
			return;
		}
		if (1.0f <= BusterChargTime && BusterChargTime < 2.0f)
		{
			BusterCreate(EBusterState::CreateMiddle, Jump_Muzzle);
		}
		else if (2.0f <= BusterChargTime)
		{
			BusterCreate(EBusterState::CreatePull, Jump_Muzzle);
		}
		BusterDelayTime = 0.0f;
		StateChange(EEgseuState::RunDashJumpAttack_Up_Loop);
		return;
	}

	// 땅에 닿음
	Color8Bit Color = UContentsGlobalData::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
	{
		DashVector = FVector::Zero;
		StateChange(EEgseuState::RunDashJump_End);
		return;
	}
}

void AEgseu::RunDashJump_EndStart()
{
	JumpVector = FVector::Zero;
	PlayerRender->ChangeAnimation(GetAnimationName("Jump_End"));
}
void AEgseu::RunDashJump_End(float _DeltaTime)
{
	// 공격
	if (true == UEngineInput::IsDown('X'))
	{
		BusterCreate(EBusterState::CreateDefault, Jump_Muzzle);
		BusterDelayTime = 0.0f;
		StateChange(EEgseuState::RunDashJumpAttack_Down_End);
		return;
	}

	// 차지 공격.
	if (true == UEngineInput::IsUp('X'))
	{
		if (BusterChargTime <= 0.8f)
		{
			return;
		}
		if (1.0f <= BusterChargTime && BusterChargTime < 2.0f)
		{
			BusterCreate(EBusterState::CreateMiddle, Jump_Muzzle);
		}
		else if (2.0f <= BusterChargTime)
		{
			BusterCreate(EBusterState::CreatePull, Jump_Muzzle);
		}
		BusterDelayTime = 0.0f;
		StateChange(EEgseuState::RunDashJumpAttack_Up_End);
		return;
	}

	if (true == UEngineInput::IsPress(VK_RIGHT) || true == UEngineInput::IsPress(VK_LEFT))
	{
		StateChange(EEgseuState::IdleRun_Loop);
		return;
	}

	if (true == PlayerRender->IsCurAnimationEnd())
	{
		StateChange(EEgseuState::Idle);
		return;
	}
}
#pragma endregion

#pragma region RunDashJumpAttack Down
void AEgseu::RunDashJumpAttack_DownStart()
{
	int CurFrame = PlayerRender->GetCurAnimationFrame();
	PlayerRender->ChangeAnimation(GetAnimationName("Jump_Start_Attack"), false, CurFrame);
	//JumpVector = JumpPower;
	DirCheck();
}
void AEgseu::RunDashJumpAttack_Down(float _DeltaTime)
{
	BusterDelayTime += _DeltaTime;
	DirCheck();
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		DashVector = FVector::Left * DashSpeed;
	}
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		DashVector = FVector::Right * DashSpeed;
	}
	RunVector = FVector::Zero;
	MoveUpdate(_DeltaTime);

	// 또 공격?
	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault, Jump_Muzzle);
	}

	// 0.5초가 지났다.
	if (BusterDelayTime >= BusterDelayTimeMax)
	{
		StateChange(EEgseuState::IdleJump);
		return;
	}

	// 바닥이네? -> 낮은 바닥...
	Color8Bit Color = UContentsGlobalData::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
	{
		JumpVector = FVector::Zero;
		DashVector = FVector::Zero;
		StateChange(EEgseuState::RunJump_End);
		return;
	}

	if (true == PlayerRender->IsCurAnimationEnd())
	{
		StateChange(EEgseuState::RunDashJumpAttack_Down_Loop);
		return;
	}
}

void AEgseu::RunDashJumpAttack_Down_LoopStart()
{
	// RunDashJump_Loop 에서 들어오기. -> BusterDelayTime == 0;
	// RunDashJumpAttack_Down 에 이어서 들어오기. -> BusterDelayTime != 0;
	if (BusterDelayTime == 0.0f)
	{
		int CurFrame = PlayerRender->GetCurAnimationFrame();
		PlayerRender->ChangeAnimation(GetAnimationName("Jump_Ing_Attack"), false, CurFrame);
	}
	else
	{
		PlayerRender->ChangeAnimation(GetAnimationName("Jump_Ing_Attack"));
	}
}
void AEgseu::RunDashJumpAttack_Down_Loop(float _DeltaTime)
{
	BusterDelayTime += _DeltaTime;
	DirCheck();
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		DashVector = FVector::Left * DashSpeed;
	}
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		DashVector = FVector::Right * DashSpeed;
	}
	RunVector = FVector::Zero;
	MoveUpdate(_DeltaTime);

	// 또 공격?
	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault, Jump_Muzzle);
	}

	// 0.5초가 지났다.
	if (BusterDelayTime >= BusterDelayTimeMax)
	{
		StateChange(EEgseuState::IdleJump);
		return;
	}

	// 바닥이네? -> 낮은 바닥...
	Color8Bit Color = UContentsGlobalData::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
	{
		JumpVector = FVector::Zero;
		DashVector = FVector::Zero;
		RunVector = FVector::Zero;
		if (BusterDelayTime != 0)
		{
			StateChange(EEgseuState::RunDashJumpAttack_Down_End);
			return;
		}
		else
		{
			StateChange(EEgseuState::IdleJump_End);
			return;
		}
	}
}

void AEgseu::RunDashJumpAttack_Down_EndStart()
{
	if (BusterDelayTime == 0.0f)
	{
		int CurFrame = PlayerRender->GetCurAnimationFrame();
		PlayerRender->ChangeAnimation(GetAnimationName("Jump_End_Attack"), false, CurFrame);
	}
	else
	{
		PlayerRender->ChangeAnimation(GetAnimationName("Jump_End_Attack"));
	}
	JumpVector = FVector::Zero;
	RunVector = FVector::Zero;
	DashVector = FVector::Zero;
}
void AEgseu::RunDashJumpAttack_Down_End(float _DeltaTime)
{
	BusterDelayTime += _DeltaTime;
	JumpVector = FVector::Zero;
	RunVector = FVector::Zero;
	DirCheck();
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		DashVector = FVector::Left * DashSpeed * _DeltaTime;
	}
	else if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		DashVector = FVector::Right * DashSpeed * _DeltaTime;
	}
	MoveUpdate(_DeltaTime);

	// 0.5초
	if (BusterDelayTime >= BusterDelayTimeMax)
	{
		StateChange(EEgseuState::RunJump_End);
		return;
	}


	if (true == PlayerRender->IsCurAnimationEnd())
	{
		if (true == UEngineInput::IsPress(VK_LEFT) || true == UEngineInput::IsPress(VK_RIGHT))
		{
			StateChange(EEgseuState::IdleRun);
			return;
		}
		else
		{
			StateChange(EEgseuState::Idle);
			return;
		}
	}
}
#pragma endregion

#pragma region RunDashJumpAttack Up
void AEgseu::RunDashJumpAttack_UpStart()
{
	int CurFrame = PlayerRender->GetCurAnimationFrame();
	PlayerRender->ChangeAnimation(GetAnimationName("Jump_Start_Attack"), false, CurFrame);
}
void AEgseu::RunDashJumpAttack_Up(float _DeltaTime)
{
	BusterDelayTime += _DeltaTime;
	DirCheck();
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		DashVector = FVector::Left * DashSpeed;
	}
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		DashVector = FVector::Right * DashSpeed;
	}
	RunVector = FVector::Zero;
	MoveUpdate(_DeltaTime);

	// 또 공격?
	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault, Jump_Muzzle);
	}

	// 바닥이네? -> 낮은 바닥...
	Color8Bit Color = UContentsGlobalData::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
	{
		JumpVector = FVector::Zero;
		DashVector = FVector::Zero;
		StateChange(EEgseuState::RunJump_End);
		return;
	}

	// 0.5초가 지났다.
	if (BusterDelayTime >= BusterDelayTimeMax)
	{
		StateChange(EEgseuState::IdleJump);
		return;
	}

	// 0.5초가 지나지 않은 상황에서 애니메이션이 끝나면,
	if (true == PlayerRender->IsCurAnimationEnd())
	{
		StateChange(EEgseuState::RunDashJumpAttack_Up_Loop);
		return;
	}
}

void AEgseu::RunDashJumpAttack_Up_LoopStart()
{
	if (BusterDelayTime == 0.0f)
	{
		int CurFrame = PlayerRender->GetCurAnimationFrame();
		PlayerRender->ChangeAnimation(GetAnimationName("Jump_Ing_Attack"), false, CurFrame);
	}
	else
	{
		PlayerRender->ChangeAnimation(GetAnimationName("Jump_Ing_Attack"));
	}
}
void AEgseu::RunDashJumpAttack_Up_Loop(float _DeltaTime)
{
	BusterDelayTime += _DeltaTime;
	DirCheck();
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		DashVector = FVector::Left * DashSpeed;
	}
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		DashVector = FVector::Right * DashSpeed;
	}
	RunVector = FVector::Zero;
	MoveUpdate(_DeltaTime);

	// 또 공격?
	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault, Jump_Muzzle);
	}

	// 0.5초
	if (BusterDelayTime >= BusterDelayTimeMax)
	{
		StateChange(EEgseuState::RunDashJump_Loop);
		return;
	}

	// 땅.
	Color8Bit Color = UContentsGlobalData::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
	{
		JumpVector = FVector::Zero;
		DashVector = FVector::Zero;
		RunVector = FVector::Zero;
		if (BusterDelayTime != 0)
		{
			StateChange(EEgseuState::RunDashJumpAttack_Up_End);
			return;
		}
		else
		{
			if (true == UEngineInput::IsPress(VK_LEFT) || true == UEngineInput::IsPress(VK_RIGHT))
			{
				StateChange(EEgseuState::IdleRun_Loop);
				return;
			}
			else
			{
				StateChange(EEgseuState::Idle);
				return;
			}
		}
	}
}

void AEgseu::RunDashJumpAttack_Up_EndStart()
{
	if (BusterDelayTime == 0.0f)
	{
		int CurFrame = PlayerRender->GetCurAnimationFrame();
		PlayerRender->ChangeAnimation(GetAnimationName("Jump_End_Attack"), false, CurFrame);
	}
	else
	{
		PlayerRender->ChangeAnimation(GetAnimationName("Jump_End_Attack"));
	}
}
void AEgseu::RunDashJumpAttack_Up_End(float _DeltaTime)
{
	BusterDelayTime += _DeltaTime;

	// 또 공격?
	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault, Jump_Muzzle);
	}

	// 0.5초
	if (BusterDelayTime >= BusterDelayTimeMax)
	{
		StateChange(EEgseuState::RunDashJump_Loop);
		return;
	}

	if (true == PlayerRender->IsCurAnimationEnd())
	{
		if (true == UEngineInput::IsPress(VK_LEFT) || true == UEngineInput::IsPress(VK_RIGHT))
		{
			if (BusterDelayTime != 0.0f)
			{
				StateChange(EEgseuState::RunAttack_Up_Loop);
				return;
			}
			else
			{
				StateChange(EEgseuState::IdleRun_Loop);
				return;
			}
		}
		else
		{
			if (BusterDelayTime != 0.0f)
			{
				StateChange(EEgseuState::IdleAttack_Up_End);
				return;
			}
			else
			{
				StateChange(EEgseuState::Idle);
				return;
			}
		}
	}
}
#pragma endregion

#pragma region RunJump / Idle -> Run -> Jump
void AEgseu::RunJumpStart()
{
	JumpVector = JumpPower;
	PlayerRender->ChangeAnimation(GetAnimationName("Jump_Start"));
	DirCheck();
}

void AEgseu::RunJump(float _DeltaTime)
{
	DirCheck();
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		RunVector = FVector::Left * MoveSpeed;
	}
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		RunVector = FVector::Right * MoveSpeed;
	}

	Color8Bit CeilingColor = UContentsGlobalData::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY() - 130, Color8Bit::MagentaA);
	if (CeilingColor == Color8Bit(255, 0, 255, 0))
	{
		JumpVector = FVector::Zero;
	}

	MoveUpdate(_DeltaTime);

	// 공격
	if (true == UEngineInput::IsDown('X'))
	{
		BusterCreate(EBusterState::CreateDefault, Jump_Muzzle);
		StateChange(EEgseuState::RunJumpAttack_Down);
		return;
	}

	// 차지 공격
	if (true == UEngineInput::IsUp('X'))
	{
		if (BusterChargTime <= 0.8f)
		{
			return;
		}
		if (1.0f <= BusterChargTime && BusterChargTime < 2.0f)
		{
			BusterCreate(EBusterState::CreateMiddle, Jump_Muzzle);
		}
		else if (2.0f <= BusterChargTime)
		{
			BusterCreate(EBusterState::CreatePull, Jump_Muzzle);
		}
		BusterDelayTime = 0.0f;
		StateChange(EEgseuState::RunJumpAttack_Up);
		return;
	}


	if (true == PlayerRender->IsCurAnimationEnd())
	{
		StateChange(EEgseuState::RunJump_Loop);
		return;
	}
}

void AEgseu::RunJump_LoopStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Jumping"));
	DirCheck();
}

void AEgseu::RunJump_Loop(float _DeltaTime)
{
	DirCheck();
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		RunVector = FVector::Left * MoveSpeed;
	}
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		RunVector = FVector::Right * MoveSpeed;
	}

	// 천장 닿음
	Color8Bit CeilingColor = UContentsGlobalData::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY() - 130, Color8Bit::MagentaA);
	if (CeilingColor == Color8Bit(255, 0, 255, 0))
	{
		JumpVector = FVector::Zero;
	}

	MoveUpdate(_DeltaTime);

	// 공격
	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault, Jump_Muzzle);
		StateChange(EEgseuState::RunJumpAttack_Down_Loop);
		return;
	}

	// 차지 공격
	if (true == UEngineInput::IsUp('X'))
	{
		if (BusterChargTime <= 0.8f)
		{
			return;
		}
		if (1.0f <= BusterChargTime && BusterChargTime < 2.0f)
		{
			BusterCreate(EBusterState::CreateMiddle, Jump_Muzzle);
		}
		else if (2.0f <= BusterChargTime)
		{
			BusterCreate(EBusterState::CreatePull, Jump_Muzzle);
		}
		BusterDelayTime = 0.0f;
		StateChange(EEgseuState::RunJumpAttack_Up_Loop);
		return;
	}


	// 점프 중 벽 체크
	bool WallChecl = CalWallCheck();
	if (true == WallChecl)
	{
		StateChange(EEgseuState::WallCling);
		return;
	}

	// 땅에 닿음
	Color8Bit Color = UContentsGlobalData::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
	{
		JumpVector = FVector::Zero;
		StateChange(EEgseuState::RunJump_End);
		return;
	}
}

void AEgseu::RunJump_EndStart()
{
	JumpVector = FVector::Zero;
	if (BusterDelayTime == 0.0f)
	{
		PlayerRender->ChangeAnimation(GetAnimationName("Jump_End"));
	}
	else
	{
		BusterDelayTime = 0.0f;
		int CurFrame = PlayerRender->GetCurAnimationFrame();
		PlayerRender->ChangeAnimation(GetAnimationName("Jump_End"), false, CurFrame);
	}
}

void AEgseu::RunJump_End(float _DeltaTime)
{
	// 점프 중 벽 체크
	bool WallChecl = CalWallCheck();
	if (true == WallChecl)
	{
		StateChange(EEgseuState::WallCling);
		return;
	}
	if (true == UEngineInput::IsPress(VK_RIGHT) || true == UEngineInput::IsPress(VK_LEFT))
	{
		StateChange(EEgseuState::IdleRun_Loop);
		return;
	}
	if (true == PlayerRender->IsCurAnimationEnd())
	{
		StateChange(EEgseuState::Idle);
		return;
	}
}
#pragma endregion

#pragma region RunJumpAttack Down
void AEgseu::RunJumpAttack_DownStart()
{
	int CurFrame = PlayerRender->GetCurAnimationFrame();
	PlayerRender->ChangeAnimation(GetAnimationName("Jump_Start_Attack"), false, CurFrame);
	DirCheck();
}
void AEgseu::RunJumpAttack_Down(float _DeltaTime)
{
	BusterDelayTime += _DeltaTime;
	DirCheck();
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		RunVector = FVector::Left * MoveSpeed;
	}
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		RunVector = FVector::Right * MoveSpeed;
	}

	MoveUpdate(_DeltaTime);

	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault, Jump_Muzzle);
	}

	// 0.5초가 지나면(가능성 낮음)
	if (BusterDelayTime >= BusterDelayTimeMax)
	{
		StateChange(EEgseuState::RunJump);
		return;
	}
	
	if (true == PlayerRender->IsCurAnimationEnd())
	{
		StateChange(EEgseuState::RunJumpAttack_Down_Loop);
		return;
	}
}

void AEgseu::RunJumpAttack_Down_LoopStart()
{
	if (BusterDelayTime == 0.0f) // x 눌렀을 때 들어옴.
	{
		int CurFrame = PlayerRender->GetCurAnimationFrame();
		PlayerRender->ChangeAnimation(GetAnimationName("Jump_Ing_Attack"), false, CurFrame);
	}
	else // 이전 상태에서 이어서 들어옴.
	{
		PlayerRender->ChangeAnimation(GetAnimationName("Jump_Ing_Attack"));
	}
	
	DirCheck();
}
void AEgseu::RunJumpAttack_Down_Loop(float _DeltaTime)
{
	BusterDelayTime += _DeltaTime;
	DirCheck();
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		RunVector = FVector::Left * MoveSpeed;
	}
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		RunVector = FVector::Right * MoveSpeed;
	}

	MoveUpdate(_DeltaTime);

	// 또 쏴?
	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault, Jump_Muzzle);
	}

	if (BusterDelayTime >= BusterDelayTimeMax)
	{
		StateChange(EEgseuState::RunJump_Loop);
		return;
	}

	// 벽 체크
	bool WallChecl = CalWallCheck();
	if (true == WallChecl)
	{
		StateChange(EEgseuState::WallCling);
		return;
	}

	// 땅에 닿음
	Color8Bit Color = UContentsGlobalData::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
	{
		JumpVector = FVector::Zero;
		if (BusterDelayTime == 0.0f)
		{
			StateChange(EEgseuState::RunJump_End);
			return;
		}
		else
		{
			StateChange(EEgseuState::RunJumpAttack_Down_End);
			return;
		}
	}
}

void AEgseu::RunJumpAttack_Down_EndStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Jump_End_Attack"));
}
void AEgseu::RunJumpAttack_Down_End(float _DeltaTime) // 공격 모션 착지.
{
	BusterDelayTime += _DeltaTime;
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		RunVector = FVector::Left * MoveSpeed;
	}
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		RunVector = FVector::Right * MoveSpeed;
	}
	MoveUpdate(_DeltaTime);

	if (BusterDelayTime >= BusterDelayTimeMax)
	{
		StateChange(EEgseuState::RunJump_End);
		return;
	}

	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault, Jump_Muzzle);
	}

	if (true == PlayerRender->IsCurAnimationEnd())
	{
		if (true == UEngineInput::IsPress(VK_RIGHT) || true == UEngineInput::IsPress(VK_LEFT))
		{
			StateChange(EEgseuState::IdleRun);
			return;
		}
		else
		{
			StateChange(EEgseuState::Idle);
			return;
		}
	}
}
#pragma endregion

#pragma region RunJumpAttack Up
void AEgseu::RunJumpAttack_UpStart()
{
	int CurFrame = PlayerRender->GetCurAnimationFrame();
	PlayerRender->ChangeAnimation(GetAnimationName("Jump_Start_Attack"), false, CurFrame);
	DirCheck();
}
void AEgseu::RunJumpAttack_Up(float _DeltaTime)
{
	BusterDelayTime += _DeltaTime;
	DirCheck();
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		RunVector = FVector::Left * MoveSpeed;
	}
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		RunVector = FVector::Right * MoveSpeed;
	}

	MoveUpdate(_DeltaTime);

	// 바닥이네? -> 낮은 바닥...
	Color8Bit Color = UContentsGlobalData::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
	{
		JumpVector = FVector::Zero;
		StateChange(EEgseuState::IdleJump_End);
		return;
	}

	bool WallChecl = CalWallCheck();
	if (true == WallChecl)
	{
		StateChange(EEgseuState::WallCling);
		return;
	}

	if (true == PlayerRender->IsCurAnimationEnd())
	{
		StateChange(EEgseuState::RunJumpAttack_Up_Loop);
		return;
	}
}
#pragma endregion

#pragma region RunJumpAttack_Up_Loop
void AEgseu::RunJumpAttack_Up_LoopStart()
{
	if (BusterDelayTime == 0.0f) // x 땠을 때 들어옴.
	{
		int CurFrame = PlayerRender->GetCurAnimationFrame();
		PlayerRender->ChangeAnimation(GetAnimationName("Jump_Ing_Attack"), false, CurFrame);
	}
	else // 이전 상태에서 이어서 들어옴.
	{
		PlayerRender->ChangeAnimation(GetAnimationName("Jump_Ing_Attack"));
	}
}
void AEgseu::RunJumpAttack_Up_Loop(float _DeltaTime)
{
	BusterDelayTime += _DeltaTime;
	DirCheck();
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		RunVector = FVector::Left * MoveSpeed;
	}
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		RunVector = FVector::Right * MoveSpeed;
	}

	MoveUpdate(_DeltaTime);

	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault, Jump_Muzzle);
	}

	if (BusterDelayTime >= BusterDelayTimeMax)
	{
		StateChange(EEgseuState::RunJump_Loop);
		return;
	}

	// 벽 체크
	bool WallChecl = CalWallCheck();
	if (true == WallChecl)
	{
		StateChange(EEgseuState::WallCling);
		return;
	}

	// 땅에 닿음
	Color8Bit Color = UContentsGlobalData::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
	{
		JumpVector = FVector::Zero;
		if (BusterChargTime == 0.0f)
		{
			StateChange(EEgseuState::RunJump_End);
			return;
		}
		else
		{
			StateChange(EEgseuState::RunJumpAttack_Up_End);
			return;
		}
	}
}
#pragma endregion

#pragma region RunJumpAttack_Up_End
void AEgseu::RunJumpAttack_Up_EndStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Jump_End_Attack"));
}

void AEgseu::RunJumpAttack_Up_End(float _DeltaTime)
{
	BusterDelayTime += _DeltaTime;
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		RunVector = FVector::Left * MoveSpeed;
	}
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		RunVector = FVector::Right * MoveSpeed;
	}
	MoveUpdate(_DeltaTime);

	if (BusterDelayTime >= BusterDelayTimeMax)
	{
		StateChange(EEgseuState::RunJump_End);
		return;
	}

	if (true == PlayerRender->IsCurAnimationEnd())
	{
		if (true == UEngineInput::IsPress(VK_RIGHT) || true == UEngineInput::IsPress(VK_LEFT))
		{
			StateChange(EEgseuState::IdleRun);
			return;
		}
		else
		{
			StateChange(EEgseuState::Idle);
			return;
		}
	}
}
#pragma endregion

#pragma region WallCling
void AEgseu::WallClingStart()
{
	// 벽 잡는 애니메이션.
	PlayerRender->ChangeAnimation(GetAnimationName("WallCling_Start"));
}
void AEgseu::WallCling(float _DeltaTime)
{
	ClingVector = FVector::Zero;
	ClingVector = ClingPower;
	AActor::AddActorLocation(ClingVector * _DeltaTime);

	// 벽 잡는 중에 공격
	if (true == UEngineInput::IsDown('X'))
	{
		WallBusterCreate(EBusterState::CreateDefault);
		StateChange(EEgseuState::WallClingAttack_Down);
		return;
	}

	// 벽 잡는 중 중에 점프
	if (true == UEngineInput::IsDown('C'))
	{
		StateChange(EEgseuState::WallKick);
		return;
	}
	
	// 벽 잡는 애니메이션이 끝나면,
	//if (3 == PlayerRender->GetCurAnimationFrame())
	if (true == PlayerRender->IsCurAnimationEnd())
	{
		StateChange(EEgseuState::WallCling_Loop);
		return;
	}

	// 낮은 땅인 경우가 있다.
	FVector CheckPos = GetActorLocation();
	switch (DirState)
	{
	case EActorDir::Left:
		CheckPos.X += 5;
		break;
	case EActorDir::Right:
		CheckPos.X -= 5;
		break;
	default:
		break;
	}
	Color8Bit Color = UContentsGlobalData::ColMapImage->GetColor(CheckPos.iX(), CheckPos.iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
	{
		if (true == UEngineInput::IsPress(VK_RIGHT) || true == UEngineInput::IsPress(VK_LEFT))
		{
			JumpVector = FVector::Zero;
			StateChange(EEgseuState::IdleRun_Loop);
			return;
		}
		else
		{
			JumpVector = FVector::Zero;
			StateChange(EEgseuState::Idle);
			return;
		}
	}
}
#pragma endregion

#pragma region WallCling_Loop
void AEgseu::WallCling_LoopStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("WallCling_Loop"));
}

void AEgseu::WallCling_Loop(float _DeltaTime)
{
	ClingVector = FVector::Zero;
	ClingVector = ClingPower;
	AActor::AddActorLocation(ClingVector * _DeltaTime);

	// 벽을 잡고 있는 중에 공격
	if (true == UEngineInput::IsDown('X'))
	{
		WallBusterCreate(EBusterState::CreateDefault);
		StateChange(EEgseuState::WallClingAttack_Down_Loop);
		return;
	}

	// 벽을 잡고 있는 중에 다시 점프
	if (true == UEngineInput::IsDown('C'))
	{
		StateChange(EEgseuState::WallKick);
		return;
	}

	// 땅에 닿으면,
	FVector CheckPos = GetActorLocation();
	switch (DirState)
	{
	case EActorDir::Left:
		CheckPos.X += 5;
		break;
	case EActorDir::Right:
		CheckPos.X -= 5;
		break;
	default:
		break;
	}
	//CheckPos.Y -= 10;

	Color8Bit Color = UContentsGlobalData::ColMapImage->GetColor(CheckPos.iX(), CheckPos.iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
	{
		if (true == UEngineInput::IsPress(VK_RIGHT) || true == UEngineInput::IsPress(VK_LEFT))
		{
			JumpVector = FVector::Zero;
			StateChange(EEgseuState::IdleRun_Loop);
			return;
		}
		else
		{
			JumpVector = FVector::Zero;
			StateChange(EEgseuState::Idle);
			return;
		}
	}
}
#pragma endregion

#pragma region WallClingAttack_Down
void AEgseu::WallClingAttack_DownStart()
{
	if (State == EEgseuState::WallCling)
	{
		BusterDelayTime = 0.0f;
		int CurFrame = PlayerRender->GetCurAnimationFrame();
		PlayerRender->ChangeAnimation(GetAnimationName("WallCling_Attack_Start"), false, CurFrame);
	}
	else
	{
		int test = 0;
	}
}
void AEgseu::WallClingAttack_Down(float _DeltaTime)
{
	BusterDelayTime += _DeltaTime;

	ClingVector = FVector::Zero;
	ClingVector = ClingPower;
	AActor::AddActorLocation(ClingVector * _DeltaTime);

	// 또 공격?
	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		WallBusterCreate(EBusterState::CreateDefault);
	}

	// 벽 잡는 중 중에 점프
	if (true == UEngineInput::IsDown('C'))
	{
		StateChange(EEgseuState::WallKickAttack_Down);
		return;
	}

	// 0.5초가 지났다.
	if (BusterDelayTime >= BusterDelayTimeMax)
	{
		StateChange(EEgseuState::WallCling);
		return;
	}

	// 애니메이션이 끝나면,
	if (true == PlayerRender->IsCurAnimationEnd())
	{
		StateChange(EEgseuState::WallClingAttack_Down_Loop);
		return;
	}

	// 낮은 땅인 경우가 있다.
	FVector CheckPos = GetActorLocation();
	switch (DirState)
	{
	case EActorDir::Left:
		CheckPos.X += 5;
		break;
	case EActorDir::Right:
		CheckPos.X -= 5;
		break;
	default:
		break;
	}
	Color8Bit Color = UContentsGlobalData::ColMapImage->GetColor(CheckPos.iX(), CheckPos.iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
	{
		if (true == UEngineInput::IsPress(VK_RIGHT) || true == UEngineInput::IsPress(VK_LEFT))
		{
			JumpVector = FVector::Zero;
			StateChange(EEgseuState::IdleRun_Loop);
			return;
		}
		else
		{
			JumpVector = FVector::Zero;
			StateChange(EEgseuState::Idle);
			return;
		}
	}
}
#pragma endregion

#pragma region WallClingAttack_Down_Loop
void AEgseu::WallClingAttack_Down_LoopStart()
{
	//EEgseuState PreState = State;
	if (State == EEgseuState::WallCling_Loop)
	{
		int CurFrame = PlayerRender->GetCurAnimationFrame();
		PlayerRender->ChangeAnimation(GetAnimationName("WallCling_Attack_Loop"), false, CurFrame);
		BusterDelayTime = 0.0f;
	}
	else if (State == EEgseuState::WallClingAttack_Down)
	{
		PlayerRender->ChangeAnimation(GetAnimationName("WallCling_Attack_Loop"));
	}
}
void AEgseu::WallClingAttack_Down_Loop(float _DeltaTime)
{
	BusterDelayTime += _DeltaTime;

	ClingVector = FVector::Zero;
	ClingVector = ClingPower;
	AActor::AddActorLocation(ClingVector * _DeltaTime);

	// 또 공격?
	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		WallBusterCreate(EBusterState::CreateDefault);
	}

	// 0.5초가 지났다.
	if (BusterDelayTime >= BusterDelayTimeMax)
	{
		StateChange(EEgseuState::WallCling_Loop);
		return;
	}

	// 벽을 잡고 있는 중에 다시 점프
	if (true == UEngineInput::IsDown('C'))
	{
		StateChange(EEgseuState::WallKickAttack_Down);
		return;
	}
	
	// 땅 체크
	FVector CheckPos = GetActorLocation();
	switch (DirState)
	{
	case EActorDir::Left:
		CheckPos.X += 5;
		break;
	case EActorDir::Right:
		CheckPos.X -= 5;
		break;
	default:
		break;
	}
	Color8Bit Color = UContentsGlobalData::ColMapImage->GetColor(CheckPos.iX(), CheckPos.iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
	{
		if (true == UEngineInput::IsPress(VK_RIGHT) || true == UEngineInput::IsPress(VK_LEFT))
		{
			JumpVector = FVector::Zero;
			StateChange(EEgseuState::IdleRun_Loop);
			return;
		}
		else
		{
			JumpVector = FVector::Zero;
			StateChange(EEgseuState::Idle);
			return;
		}
	}
}
#pragma endregion

#pragma region WallClingAttack_Up
void AEgseu::WallClingAttack_UpStart()
{
}
void AEgseu::WallClingAttack_Up(float _DeltaTime)
{
}
#pragma endregion

#pragma region WallClingAttack_Up_Loop
void AEgseu::WallClingAttack_Up_LoopStart()
{
}
void AEgseu::WallClingAttack_Up_Loop(float _DeltaTime)
{
}
#pragma endregion

#pragma region WallKick
void AEgseu::WallKickStart()
{
	// 중력 값 초기화.(안하면 계속 쌓여서 점프를 할 수 없음)
	GravityVector = FVector::Zero;
	
	// 반대 방향으로 점프
	if (0 == static_cast<int>(DirState)) // Left
	{
		JumpVector = JumpPower + (FVector::Right * MoveSpeed);
	}
	else // Right
	{
		JumpVector = JumpPower + (FVector::Left * MoveSpeed);
	}

	// 애니메이션 설정.
	if (State == EEgseuState::WallKickAttack_Down)
	{
		int CurFrame = PlayerRender->GetCurAnimationFrame();
		PlayerRender->ChangeAnimation(GetAnimationName("WallKick"), false, CurFrame);
	}
	else
	{
		PlayerRender->ChangeAnimation(GetAnimationName("WallKick"));
	}

	WallKickTime = 0.0f;
}

void AEgseu::WallKick(float _DeltaTime)
{
	WallKickTime += _DeltaTime;
	MoveUpdate(_DeltaTime);
	if (WallKickTime < 0.2f) // 해당 시간 동안 아무것도 못함.(선 딜)
	{
		return;
	}
	
	// JumpVector 에 남아있던 좌우 방향 힘 초기화
	float JumpXValue = JumpVector.X;
	if (JumpXValue != 0.0f)
	{
		JumpVector.X = 0.0f;
	}
	//JumpVector = JumpVector - (FVector::Right * MoveSpeed);

	// 벽 차는 동작 끝났어.
	if (2 == PlayerRender->GetCurAnimationImageFrame())
	{
		// 대쉬 버튼을 누르고 있다면,
		if (true == UEngineInput::IsPress('Z'))
		{
			StateChange(EEgseuState::RunDashJump_Loop);
			return;
		}
		// 대쉬 버튼을 누르고 있지 않다면,
		else if (true == UEngineInput::IsFree('Z'))
		{
			StateChange(EEgseuState::RunJump_Loop);
			return;
		}
		// 방향키를 누르고 있지 않다면,
		else if (true == UEngineInput::IsFree(VK_LEFT) && true == UEngineInput::IsFree(VK_RIGHT))
		{
			StateChange(EEgseuState::IdleJump_Loop);
			return;
		}
	}
}
#pragma endregion

#pragma region WallKickAttack_Down
void AEgseu::WallKickAttack_DownStart()
{
	// 중력 값 초기화.(안하면 계속 쌓여서 점프를 할 수 없음)
	GravityVector = FVector::Zero;

	// 반대 방향으로 점프
	if (0 == static_cast<int>(DirState)) // Left
	{
		JumpVector = JumpPower + (FVector::Right * MoveSpeed);
	}
	else // Right
	{
		JumpVector = JumpPower + (FVector::Left * MoveSpeed);
	}

	// 애니메이션 설정
	if (State == EEgseuState::WallClingAttack_Down)
	{
		PlayerRender->ChangeAnimation(GetAnimationName("WallKick_Attack"));
	}
	else if (State == EEgseuState::WallClingAttack_Down_Loop)
	{
		PlayerRender->ChangeAnimation(GetAnimationName("WallKick_Attack"));
	}
	else
	{
		int CurFrame = PlayerRender->GetCurAnimationFrame();
		PlayerRender->ChangeAnimation(GetAnimationName("WallKick_Attack"), false, CurFrame);
	}

	WallKickTime = 0.0f;
}
void AEgseu::WallKickAttack_Down(float _DeltaTime)
{
	WallKickTime += _DeltaTime;
	MoveUpdate(_DeltaTime);
	if (WallKickTime < 0.2f) // 해당 시간 동안 아무것도 못함.(선 딜)
	{
		return;
	}
	BusterDelayTime += _DeltaTime;

	// JumpVector 에 남아있던 좌우 방향 힘 초기화
	float JumpXValue = JumpVector.X;
	if (JumpXValue != 0.0f)
	{
		JumpVector.X = 0.0f;
	}


	// 0.5초가 지났다.
	if (BusterDelayTime >= BusterDelayTimeMax)
	{
		StateChange(EEgseuState::WallKick);
		return;
	}

	if (true == PlayerRender->IsCurAnimationEnd())
	{
		// 대쉬 버튼을 누르고 있다면,
		if (true == UEngineInput::IsPress('Z'))
		{
			StateChange(EEgseuState::RunDashJump_Loop);
			return;
		}
		// 대쉬 버튼을 누르고 있지 않다면,
		else if (true == UEngineInput::IsFree('Z'))
		{
			StateChange(EEgseuState::RunJump_Loop);
			return;
		}
		// 방향키를 누르고 있지 않다면,
		else if (true == UEngineInput::IsFree(VK_LEFT) && true == UEngineInput::IsFree(VK_RIGHT))
		{
			StateChange(EEgseuState::IdleJump_Loop);
			return;
		}
	}
}
#pragma endregion

#pragma region WallKickAttack_Up
void AEgseu::WallKickAttack_UpStart()
{
}
void AEgseu::WallKickAttack_Up(float _DeltaTime)
{
}
#pragma endregion

#pragma region Hit
void AEgseu::HitStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Hit"));
	Hit_InvincibilityTime = 0.0f;
}

void AEgseu::Hit(float _DeltaTime)
{
	Hit_InvincibilityTime += _DeltaTime;
	MoveUpdate(_DeltaTime);

	// 무적시간.
	if (Hit_InvincibilityTime < 0.5f)
	{
		return;
	}
	else
	{
		if (true == UEngineInput::IsPress(VK_RIGHT) || true == UEngineInput::IsPress(VK_LEFT))
		{
			Hit_Count = 0;
			StateChange(EEgseuState::IdleRun_Loop);
			return;
		}
		else
		{
			Hit_Count = 0;
			StateChange(EEgseuState::Idle);
			return;
		}
	}
}
#pragma endregion

#pragma region Hit_MiruTorearu
void AEgseu::Hit_MiruTorearuStart()
{
	//PlayerRender->ChangeAnimation(GetAnimationName(""));
	int a = 0;
}
void AEgseu::Hit_MiruTorearu(float _DeltaTime)
{
	// 공의 중앙 방향.
	// 이동 속도.

}
#pragma endregion

#pragma region Focus
void AEgseu::FocusCreateStart()
{
	GetWorld()->SetAllTimeScale(0.0f);
	GetWorld()->SetTimeScale(EActorType::Map, 1.0f);
	GetWorld()->SetTimeScale(EActorType::MapObject, 1.0f);
}
void AEgseu::FocusCreate(float _DeltaTime)
{
	return;
}


void AEgseu::FocusLoopStart()
{
	GetWorld()->SetAllTimeScale(1.0f);
}
void AEgseu::FocusLoop(float _DeltaTime)
{
	if (true == UEngineInput::IsPress(VK_RIGHT) || true == UEngineInput::IsPress(VK_LEFT))
	{
		StateChange(EEgseuState::IdleRun_Loop);
		return;
	}
	else if (true == UEngineInput::IsFree(VK_RIGHT) && true == UEngineInput::IsFree(VK_LEFT))
	{
		StateChange(EEgseuState::Idle);
		return;
	}
}


void AEgseu::FocusEndStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Idle"));
}

void AEgseu::FocusEnd(float _DeltaTime)
{
	// 랭크 받고 오른쪽으로 이동.
	if (true == AutoRightRun)
	{
		StateChange(EEgseuState::AutoRunRight);
		return;
	}
}
#pragma endregion

#pragma region AutoRunRight / Next Stage
void AEgseu::AutoRunRightStart()
{
	RunVector = FVector::Zero;
	PlayerRender->ChangeAnimation(GetAnimationName("Run"));
}

void AEgseu::AutoRunRight(float _DeltaTime)
{
	RunVector = FVector::Right * MoveSpeed * _DeltaTime;
	AddActorLocation(RunVector);
}
#pragma endregion

#pragma region BossRoomAutoRun
void AEgseu::BossRoomAutoRunStart()
{
	std::string CurAniName = PlayerRender->GetCurAnimation()->Name;
	PlayerRender->ChangeAnimation(CurAniName);
}

void AEgseu::BossRoomAutoRun(float _DeltaTime)
{

}
#pragma endregion




// === Vector =============================================
#pragma region Vector
void AEgseu::CalMoveVector()
{
	// Actor의 기준점 가져오기.
	FVector CheckPos = GetActorLocation();

	// 충돌을 체크하기 위해
	// 방향키에 맞춰 Actor의 기준점을 옮긴다. 
	switch (DirState)
	{
	case EActorDir::Left:
		CheckPos.X -= 40;
		break;
	case EActorDir::Right:
		CheckPos.X += 40;
		break;
	default:
		break;
	}
	CheckPos.Y -= 10;

	// 위의 CheckPos를 사용해서 Map의 충돌 체크를 한다.
	Color8Bit Color = UContentsGlobalData::ColMapImage->GetColor(CheckPos.iX(), CheckPos.iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
	{
		RunVector = FVector::Zero;
		DashVector = FVector::Zero;
	}


	// 키보드 입력이 없으면 속도를 Zero로.
	if (true == UEngineInput::IsFree(VK_LEFT) && true == UEngineInput::IsFree(VK_RIGHT) &&
		true == UEngineInput::IsFree(VK_UP) && true == UEngineInput::IsFree(VK_DOWN))
	{
		RunVector = float4::Zero;
	}
}

void AEgseu::CalGravityVector(float _DeltaTime)
{
	GravityVector += GravityAcc * _DeltaTime;

	Color8Bit Color = UContentsGlobalData::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
	{
		GravityVector = FVector::Zero;
	}
	if (State == EEgseuState::Summon)
	{
		GravityVector = FVector::Zero;
	}
}

void AEgseu::MoveLastMoveVector(float _DeltaTime)
{
	AActor::AddActorLocation(LastMoveVector * _DeltaTime);
}

void AEgseu::CalLastMoveVector()
{
	LastMoveVector = FVector::Zero;
	LastMoveVector = LastMoveVector + RunVector;
	LastMoveVector = LastMoveVector + JumpVector;
	LastMoveVector = LastMoveVector + GravityVector;
	LastMoveVector = LastMoveVector + DashVector;
	LastMoveVector = LastMoveVector + SummonVector;
	LastMoveVector + JumpVector;
}

void AEgseu::MoveUpdate(float _DeltaTime)
{
	CalMoveVector();
	CalGravityVector(_DeltaTime);
	CalLastMoveVector();
	MoveLastMoveVector(_DeltaTime);
}
#pragma endregion


// 충돌.
void AEgseu::CollisionCheck(float _DeltaTime)
{
	std::vector<UCollision*> Result;

	if (true == PlayerCollision->CollisionCheck(ECollisionOrder::Boss, Result))
	{
		Hit_Count++;
		if (Hit_Count == 1)
		{
			Hit_InvincibilityTime += _DeltaTime;
			ACyberPeacock* Boss = (ACyberPeacock*)Result[0]->GetOwner();
			int BossBodyDamage = Boss->GetBodyDamage();
			Hp -= BossBodyDamage;

			StateChange(EEgseuState::Hit);
			return;
		}
	}

	if (true == PlayerCollision->CollisionCheck(ECollisionOrder::Enemy, Result))
	{
		Hit_Count++;
		if (Hit_Count == 1)
		{
			AMiruTorearu* Enemy = (AMiruTorearu*)Result[0]->GetOwner();
			std::string x = Enemy->GetName();

		}
	}
}

/// <summary>
/// 벽의 옆면 체크
/// </summary>
/// <returns></returns>
bool AEgseu::CalWallCheck()
{
	// Actor의 기준점 가져오기.
	FVector CheckPos_1 = GetActorLocation();
	FVector CheckPos_2 = GetActorLocation();

	// 충돌을 체크하기 위해
	// 방향키에 맞춰 Actor의 기준점을 옮긴다. 
	switch (DirState)
	{
	case EActorDir::Left:
		CheckPos_1.X -= 40;
		CheckPos_2.X -= 40;
		break;
	case EActorDir::Right:
		CheckPos_1.X += 40;
		CheckPos_2.X += 40;
		break;
	default:
		break;
	}
	CheckPos_1.Y -= 10;
	CheckPos_2.Y -= PlayerRender->GetImage()->GetScale().Y / 4.0f; // 캐릭터 중앙.

	// 위의 CheckPos를 사용해서 Map의 충돌 체크를 한다.
	Color8Bit Color_1 = UContentsGlobalData::ColMapImage->GetColor(CheckPos_1.iX(), CheckPos_1.iY(), Color8Bit::MagentaA);
	Color8Bit Color_2 = UContentsGlobalData::ColMapImage->GetColor(CheckPos_2.iX(), CheckPos_2.iY(), Color8Bit::MagentaA);
	if (Color_1 == Color8Bit(255, 0, 255, 0) && Color_2 == Color8Bit(255, 0, 255, 0))
	{
		return true;
	}
	return false;
}

void AEgseu::BusterChargeTime(float _DeltaTime)
{
	if (true == UEngineInput::IsPress('X'))
	{
		BusterChargTime += _DeltaTime;
	}

	// 차지 이미지
	if (1.0f <= BusterChargTime && BusterChargTime < 2.0f)
	{
		MiddleChargeRender->ActiveOn();
	}
	else if (2.0f <= BusterChargTime)
	{
		PullChargeRender->ActiveOn();
	}

	if (true == UEngineInput::IsFree('X'))
	{
		if (true == MiddleChargeRender->IsActive())
		{
			MiddleChargeRender->ActiveOff();
		}
		if (true == PullChargeRender->IsActive())
		{
			PullChargeRender->ActiveOff();
		}
		BusterChargTime = 0.0f;
	}
}

void AEgseu::WallBusterCreate(EBusterState _BusterState)
{
	ABuster* A_Buster = GetWorld()->SpawnActor<ABuster>(static_cast<int>(EActorType::Buster));
	FVector ShotPos = FVector::Zero;
	FVector PlayerPos = GetActorLocation();

	ShotPos.Y = PlayerPos.Y - 85.0f;
	if (DirState == EActorDir::Right)
	{
		A_Buster->SetDirState(EActorDir::Left);
		ShotPos.X = PlayerPos.X - 45.0f;
	}
	else if (DirState == EActorDir::Left)
	{
		A_Buster->SetDirState(EActorDir::Right);
		ShotPos.X = PlayerPos.X + 45.0f;
	}

	A_Buster->SetActorLocation(ShotPos);
	A_Buster->SetBusterState(_BusterState);
}

void AEgseu::BusterCreate(EBusterState _BusterState, FVector _Pos)
{
	ABuster* A_Buster = GetWorld()->SpawnActor<ABuster>(static_cast<int>(EActorType::Buster));
	FVector ShotPos = FVector::Zero;
	FVector PlayerPos = GetActorLocation();

	ShotPos.Y = PlayerPos.Y - _Pos.Y;
	if (DirState == EActorDir::Right)
	{
		A_Buster->SetDirState(EActorDir::Right);
		ShotPos.X = PlayerPos.X + _Pos.X;
	}
	else if (DirState == EActorDir::Left)
	{
		A_Buster->SetDirState(EActorDir::Left);
		ShotPos.X = PlayerPos.X - _Pos.X;
	}

	A_Buster->SetActorLocation(ShotPos);
	A_Buster->SetBusterState(_BusterState);
}


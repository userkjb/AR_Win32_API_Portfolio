#include "Egseu.h"
#include <EngineCore/EngineCore.h> // Helper
#include "Buster.h"
#include "CyberPeacock.h"

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
	UEngineDebug::OutPutDebugText(std::to_string(static_cast<int>(State)));
}

void AEgseu::ChargeBeginPlay()
{
	MiddleChargeRender = CreateImageRenderer(static_cast<int>(ERenderOrder::Buster));
	MiddleChargeRender->SetImage("Charging_1.png");
	UWindowImage* ChargeImage_1 = MiddleChargeRender->GetImage();
	FVector ChargeScale_1 = ChargeImage_1->GetScale();
	MiddleChargeRender->AutoImageScale();
	//MiddleChargeRender->SetTransform({ { 0, 0 }, {ChargeScale_1.iX() / 9, ChargeScale_1.iY()} });

	PullChargeRender = CreateImageRenderer(static_cast<int>(ERenderOrder::Buster));
	PullChargeRender->SetImage("Charging_2.png");
	UWindowImage* ChargeImage_2 = PullChargeRender->GetImage();
	FVector ChargeScale_2 = ChargeImage_2->GetScale();
	PullChargeRender->AutoImageScale();
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
	Hp = MaxHp;

	PlayerRender = CreateImageRenderer(static_cast<int>(ERenderOrder::Player));
	PlayerRender->SetImage("x_Idle_Right.png");
	PlayerRender->AutoImageScale(2.0f);
	//PlayerRender->SetTransform({ {0,0}, {35 * 3, 80 * 3} });

	PlayerCollision = CreateCollision(ECollisionOrder::Player);
	PlayerCollision->SetScale({ 35 * 3, 80 * 3 });
	PlayerCollision->SetColType(ECollisionType::Rect);

	// ===== Animation ======================================================
	// Summon
	PlayerRender->CreateAnimation("Summon", "x_Start.png", 0, 0, 0.2f, true);
	PlayerRender->CreateAnimation("Summon_Loop", "x_Start.png", 1, 16, 0.13f, false);

	// Idle
	PlayerRender->CreateAnimation("Idle_Right", "x_Idle_Right.png", { 0,1,2,3,4,3,2,1 }, 0.2f, true);
	PlayerRender->CreateAnimation("Idle_Left", "x_Idle_Left.png", { 0,1,2,3,4,3,2,1 }, 0.2f, true);

	// Run
	PlayerRender->CreateAnimation("Run_Ready_Right", "x_Move_Right.png", 0, 1, 0.05f, false);
	PlayerRender->CreateAnimation("Run_Ready_Left", "x_Move_Left.png", 0, 1, 0.05f, false);
	PlayerRender->CreateAnimation("Run_Right", "x_Move_Right.png", 2, 15, 0.1f, true);
	PlayerRender->CreateAnimation("Run_Left", "x_Move_Left.png", 2, 15, 0.1f, true);

	// Jump
	PlayerRender->CreateAnimation("Jump_Start_Right", "x_Jump_Right.png", 0, 7, 0.1f, false);
	PlayerRender->CreateAnimation("Jump_Start_Left", "x_Jump_Left.png", 0, 7, 0.1f, false);
	PlayerRender->CreateAnimation("Jumping_Right", "x_Jump_Right.png", 7, 7, 0.1f, false);
	PlayerRender->CreateAnimation("Jumping_Left", "x_Jump_Left.png", 7, 7, 0.1f, false);
	PlayerRender->CreateAnimation("Jump_End_Right", "x_Jump_Right.png", 8, 10, 0.01f, false);
	PlayerRender->CreateAnimation("Jump_End_Left", "x_Jump_Left.png", 8, 10, 0.01f, false);

	// Jump Attack
	PlayerRender->CreateAnimation("Jump_Start_Attack_Right", "JumpAttack_Right.png", { 0, 2, 4, 6, 8, 10, 12, 14 }, 0.05f, false);
	PlayerRender->CreateAnimation("Jump_Start_Attack_Left", "JumpAttack_Left.png", { 0, 2, 4, 6, 8, 10, 12, 14 }, 0.05f, false);
	PlayerRender->CreateAnimation("Jump_Ing_Attack_Right", "JumpAttack_Right.png", 14, 14, 0.05f, false);
	PlayerRender->CreateAnimation("Jump_Ing_Attack_Left", "JumpAttack_Left.png", 14, 14, 0.05f, false);
	PlayerRender->CreateAnimation("Jump_End_Attack_Right", "JumpAttack_Right.png", { 16, 18, 20 }, 0.05f, false);
	PlayerRender->CreateAnimation("Jump_End_Attack_Left", "JumpAttack_Left.png", { 16, 18, 20 }, 0.05f, false);

	//PlayerRender->CreateAnimation("Jump_Attack_Shoot_Right", "JumpAttack_Right.png", { 1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21 }, 0.05f, false);
	//PlayerRender->CreateAnimation("Jump_Attack_Shoot_Left", "JumpAttack_Left.png", { 1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21 }, 0.05f, false);

	// Dash
	PlayerRender->CreateAnimation("Dash_Start_Right", "x_Dash_Right.png", 0, 1, 0.1f, false);
	PlayerRender->CreateAnimation("Dash_Start_Left", "x_Dash_Left.png", 0, 1, 0.1f, false);
	PlayerRender->CreateAnimation("Dash_Loop_Right", "x_Dash_Right.png", 2, 3, 0.1f, true);
	PlayerRender->CreateAnimation("Dash_Loop_Left", "x_Dash_Left.png", 2, 3, 0.1f, true);
	PlayerRender->CreateAnimation("Dash_End_Right", "x_Dash_Right.png", 4, 7, 0.1f, false);
	PlayerRender->CreateAnimation("Dash_End_Left", "x_Dash_Left.png", 4, 7, 0.1f, false);
	// Dash Effect
	//PlayerRender->CreateAnimation("Dash_Right", "x_Dash_Left.png", 4, 7, 0.1f, false);
	//PlayerRender->CreateAnimation("Dash_Left", "x_Dash_Left.png", 4, 7, 0.1f, false);

	// Attack
	// x_Attack_Right 이미지가 좋은 이미지가 아니여서 후에 바꿔야 함.
	PlayerRender->CreateAnimation("Idle_Attack_Start_Right", "x_Attack_Right.png", 0, 5, 0.1f, false);
	PlayerRender->CreateAnimation("Idle_Attack_Start_Left", "x_Attack_Left.png", 0, 5, 0.1f, false);
	PlayerRender->CreateAnimation("Idle_Attack_Loop_Right", "x_Attack_Right.png", 5, 5, 0.5f, true);
	PlayerRender->CreateAnimation("Idle_Attack_Loop_Left", "x_Attack_Left.png", 5, 5, 0.5f, true);
	PlayerRender->CreateAnimation("Idle_Attack_End_Right", "x_Attack_Right.png", 5, 7, 0.1f, false);
	PlayerRender->CreateAnimation("Idle_Attack_End_Left", "x_Attack_Left.png", 5, 7, 0.1f, false);

	PlayerRender->CreateAnimation("Run_Attack_Start_Right", "x_Move_Attack_Right.png", { 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30 }, 0.05f, true);
	PlayerRender->CreateAnimation("Run_Attack_Start_Left", "x_Move_Attack_Left.png", { 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30 }, 0.05f, true);
	PlayerRender->CreateAnimation("Run_Attack_Loop_Right", "x_Move_Attack_Right.png", { 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30 }, 0.05f, true);
	PlayerRender->CreateAnimation("Run_Attack_Loop_Left", "x_Move_Attack_Left.png", { 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30 }, 0.05f, true);

	PlayerRender->CreateAnimation("Dash_Attack_Start_Right", "x_Dash_Attack_Right.png", { 0, 2 }, 0.05f, false);
	PlayerRender->CreateAnimation("Dash_Attack_Start_Left", "x_Dash_Attack_Left.png", { 0, 2 }, 0.05f, false);
	PlayerRender->CreateAnimation("Dash_Attack_Loop_Right", "x_Dash_Attack_Right.png", { 4, 6 }, 0.05f, false);
	PlayerRender->CreateAnimation("Dash_Attack_Loop_Left", "x_Dash_Attack_Left.png", { 4, 6 }, 0.05f, false);
	PlayerRender->CreateAnimation("Dash_Attack_End_Right", "x_Dash_Attack_Right.png", { 8, 10, 12, 14 }, 0.05f, false);
	PlayerRender->CreateAnimation("Dash_Attack_End_Left", "x_Dash_Attack_Left.png", { 8, 10, 12, 14 }, 0.05f, false);


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
	PlayerRender->CreateAnimation("WallCling_Start_Right", "Wall_Cling_Right.png", 0, 3, 0.05f, false);
	PlayerRender->CreateAnimation("WallCling_Start_Left", "Wall_Cling_Left.png", 0, 3, 0.05f, false);
	PlayerRender->CreateAnimation("WallCling_Loop_Right", "Wall_Cling_Right.png", 3, 3, 0.5f, false);
	PlayerRender->CreateAnimation("WallCling_Loop_Left", "Wall_Cling_Left.png", 3, 3, 0.5f, false);
	PlayerRender->CreateAnimation("WallKick_Right", "Wall_Cling_Kick_Right.png", 0, 2, 0.05f, false);
	PlayerRender->CreateAnimation("WallKick_Left", "Wall_Cling_Kick_Left.png", 0, 2, 0.05f, false);

	// Wall Effect
	//PlayerRender->CreateAnimation("WallCling_Effect_Right", "WallEffect_Right.png", 0, 7, 0.5f, false);
	//PlayerRender->CreateAnimation("WallCling_Effect_Left", "WallEffect_Left.png", 0, 7, 0.5f, false);
	//PlayerRender->CreateAnimation("WallKick_Effect_Right", "WallKick_Right.png", 0, 3, 0.5f, false);
	//PlayerRender->CreateAnimation("WallKick_Effect_Left", "WallKick_Left.png", 0, 3, 0.5f, false);

	// Hit
	PlayerRender->CreateAnimation("Hit_Right", "x_Damage_Right.png", 0, 3, 0.05f, true);
	PlayerRender->CreateAnimation("Hit_Left", "x_Damage_Left.png", 0, 3, 0.05f, true);

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
		case EEgseuState::JumpAttack:
			JumpAttackStart();
			break;
		case EEgseuState::JumpAttack_Loop:
			JumpAttack_LoopStart();
			break;
		case EEgseuState::JumpAttack_End:
			JumpAttack_EndStart();
			break;
		case EEgseuState::IdleAttack:
			IdleAttackStart();
			break;
		case EEgseuState::IdleAttack_Loop:
			IdleAttack_LoopStart();
			break;
		case EEgseuState::IdleAttack_End:
			IdleAttack_EndStart();
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
		case EEgseuState::IdleRun_End:
			IdleRun_EndStart();
			break;
		case EEgseuState::RunAttack:
			RunAttackStart();
			break;
		case EEgseuState::RunAttack_Loop:
			RunAttack_LoopStart();
			break;
		case EEgseuState::RunAttack_End:
			RunAttack_EndStart();
			break;
		case EEgseuState::RunDash:
			RunDashStart();
			break;
		case EEgseuState::RunDash_Loop:
			RunDash_LoopStart();
			break;
		case EEgseuState::RunDash_End:
			RunDash_EndStart();
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
		case EEgseuState::RunDashJumpAttack:
			RunDashJumpAttackStart();
			break;
		case EEgseuState::RunDashJumpAttack_Loop:
			RunDashJumpAttack_LoopStart();
			break;
		case EEgseuState::RunDashJumpAttack_End:
			RunDashJumpAttack_EndStart();
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
		case EEgseuState::RunJumpAttack:
			RunJumpAttackStart();
			break;
		case EEgseuState::RunJumpAttack_Loop:
			RunJumpAttack_LoopStart();
			break;
		case EEgseuState::RunJumpAttack_End:
			RunJumpAttack_EndStart();
			break;
		case EEgseuState::WallCling:
			WallClingStart();
			break;
		case EEgseuState::WallCling_Loop:
			WallCling_LoopStart();
			break;
		case EEgseuState::WallKick:
			WallKickStart();
			break;
		case EEgseuState::Hit:
			HitStart();
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
	case EEgseuState::JumpAttack:
		JumpAttack(_DeltaTime);
		break;
	case EEgseuState::JumpAttack_Loop:
		JumpAttack_Loop(_DeltaTime);
		break;
	case EEgseuState::JumpAttack_End:
		JumpAttack_End(_DeltaTime);
		break;
	case EEgseuState::IdleAttack:
		IdleAttack(_DeltaTime);
		break;
	case EEgseuState::IdleAttack_Loop:
		IdleAttack_Loop(_DeltaTime);
		break;
	case EEgseuState::IdleAttack_End:
		IdleAttack_End(_DeltaTime);
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
	case EEgseuState::IdleRun_End:
		IdleRun_End(_DeltaTime);
		break;
	case EEgseuState::RunAttack:
		RunAttack(_DeltaTime);
		break;
	case EEgseuState::RunAttack_Loop:
		RunAttack_Loop(_DeltaTime);
		break;
	case EEgseuState::RunAttack_End:
		RunAttack_End(_DeltaTime);
		break;
	case EEgseuState::RunDash:
		RunDash(_DeltaTime);
		break;
	case EEgseuState::RunDash_Loop:
		RunDash_Loop(_DeltaTime);
		break;
	case EEgseuState::RunDash_End:
		RunDash_End(_DeltaTime);
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
	case EEgseuState::RunDashJumpAttack:
		RunDashJumpAttack(_DeltaTime);
		break;
	case EEgseuState::RunDashJumpAttack_Loop:
		RunDashJumpAttack_Loop(_DeltaTime);
		break;
	case EEgseuState::RunDashJumpAttack_End:
		RunDashJumpAttack_End(_DeltaTime);
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
	case EEgseuState::RunJumpAttack:
		RunJumpAttack(_DeltaTime);
		break;
	case EEgseuState::RunJumpAttack_Loop:
		RunJumpAttack_Loop(_DeltaTime);
		break;
	case EEgseuState::RunJumpAttack_End:
		RunJumpAttack_End(_DeltaTime);
		break;
	case EEgseuState::WallCling:
		WallCling(_DeltaTime);
		break;
	case EEgseuState::WallCling_Loop:
		WallCling_Loop(_DeltaTime);
		break;
	case EEgseuState::WallKick:
		WallKick(_DeltaTime);
		break;
	case EEgseuState::Hit:
		Hit(_DeltaTime);
		break;
	default :
		break;
	}
}

// Begin & Tick

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
	SummonVector = FVector::Down * 1000.0f;
	MoveUpdate(_DeltaTime);

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
		if (1.4f < SummonDelayTime)
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

#pragma region IdleStart
void AEgseu::IdleStart()
{
	JumpVector = FVector::Zero; // 벽 타고 내려왔을 대 점프 초기화.
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
		StateChange(EEgseuState::IdleAttack);
		return;
	}

	// 발싸!!!
	if (true == UEngineInput::IsUp('X'))
	{
		StateChange(EEgseuState::IdleAttack_End);
		return;
	}

	// 대쉬 Z
	if (true == UEngineInput::IsDown('Z'))
	{
		StateChange(EEgseuState::IdleDash);
		return;
	}

	MoveUpdate(_DeltaTime);
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
	PlayerRender->ChangeAnimation(GetAnimationName("Jump_Start"));
	DirCheck();
}

void AEgseu::IdleJump_LoopStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Jumping"));
	DirCheck();
}

void AEgseu::IdleJump_EndStart()
{
	JumpVector = FVector::Zero;
	PlayerRender->ChangeAnimation(GetAnimationName("Jump_End"));
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
		StateChange(EEgseuState::JumpAttack);
		return;
	}

	if (true == PlayerRender->IsCurAnimationEnd())
	{
		StateChange(EEgseuState::IdleJump_Loop);
		return;
	}
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
		StateChange(EEgseuState::JumpAttack_Loop);
		return;
	}

	// 벽에 닿음
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
		StateChange(EEgseuState::IdleJump_End);
		return;
	}
}

void AEgseu::IdleJump_End(float _DeltaTime)
{
	if (true == UEngineInput::IsDown('X'))
	{
		StateChange(EEgseuState::JumpAttack_End);
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

#pragma region JumpAttack
void AEgseu::JumpAttackStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Jump_Start_Attack"));
	//int x = PlayerRender->GetCurAnimationImageFrame();
	//PlayerRender->SetImageIndex(x + 1);
}

void AEgseu::JumpAttack_LoopStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Jump_Ing_Attack"));
}

void AEgseu::JumpAttack_EndStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Jump_End_Attack"));
}

void AEgseu::JumpAttack(float _DeltaTime)
{
	PlayerRender->ChangeAnimation(GetAnimationName("Jump_Start_Attack"), true, PlayerRender->GetCurAnimationFrame(), PlayerRender->GetCurAnimationTime());

	MoveUpdate(_DeltaTime);

	if (true == UEngineInput::IsUp('X') || true == UEngineInput::IsFree('X'))
	{
		ABuster* A_Buster = GetWorld()->SpawnActor<ABuster>();
		A_Buster->SetActorLocation(GetActorLocation()); // 상세 위치 조절 TODO
		std::string BusterName;
		if (DirState == EActorDir::Right)
		{
			A_Buster->SetDir(FVector::Right);
			BusterName = "Buster_Default_Right";
		}
		else if (DirState == EActorDir::Left)
		{
			A_Buster->SetDir(FVector::Left);
			BusterName = "Buster_Default_Left";
		}
		A_Buster->SetBusterState(EBusterState::DefaultCharge);
		A_Buster->SetBusterAnimation(BusterName);
	}
	
	StateChange(EEgseuState::IdleJump);
	return;
}
void AEgseu::JumpAttack_Loop(float _DeltaTime)
{
	PlayerRender->ChangeAnimation(GetAnimationName("Jump_Ing_Attack"), true, PlayerRender->GetCurAnimationFrame(), PlayerRender->GetCurAnimationTime());

	MoveUpdate(_DeltaTime);

	Color8Bit Color = UContentsGlobalData::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
	{
		StateChange(EEgseuState::IdleJump_End);
		return;
	}

	if (true == UEngineInput::IsUp('X') || true == UEngineInput::IsFree('X'))
	{
		ABuster* A_Buster = GetWorld()->SpawnActor<ABuster>();
		A_Buster->SetActorLocation(GetActorLocation()); // 상세 위치 조절 TODO
		std::string BusterName;
		if (DirState == EActorDir::Right)
		{
			A_Buster->SetDir(FVector::Right);
			BusterName = "Buster_Default_Right";
		}
		else if (DirState == EActorDir::Left)
		{
			A_Buster->SetDir(FVector::Left);
			BusterName = "Buster_Default_Left";
		}
		A_Buster->SetBusterState(EBusterState::DefaultCharge);
		A_Buster->SetBusterAnimation(BusterName);
		
		StateChange(EEgseuState::IdleJump_Loop);
		return;
	}
}
void AEgseu::JumpAttack_End(float _DeltaTime)
{
	//DirCheck();
	PlayerRender->ChangeAnimation(GetAnimationName("Jump_End_Attack"), true, PlayerRender->GetCurAnimationFrame(), PlayerRender->GetCurAnimationTime());

	MoveUpdate(_DeltaTime);

	if (true == UEngineInput::IsUp('X') || true == UEngineInput::IsFree('X'))
	{
		ABuster* A_Buster = GetWorld()->SpawnActor<ABuster>();
		A_Buster->SetActorLocation(GetActorLocation()); // 상세 위치 조절 TODO
		std::string BusterName;
		if (DirState == EActorDir::Right)
		{
			A_Buster->SetDir(FVector::Right);
			BusterName = "Buster_Default_Right";
		}
		else if (DirState == EActorDir::Left)
		{
			A_Buster->SetDir(FVector::Left);
			BusterName = "Buster_Default_Left";
		}
		A_Buster->SetBusterState(EBusterState::DefaultCharge);
		A_Buster->SetBusterAnimation(BusterName);

		StateChange(EEgseuState::IdleJump_End);
		return;
	}
}
#pragma endregion

#pragma region IdleAttack
void AEgseu::IdleAttackStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Idle_Attack_Start"));
}

void AEgseu::IdleAttack_LoopStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Idle_Attack_Loop"));
}

void AEgseu::IdleAttack_EndStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Idle_Attack_End"));
}

void AEgseu::IdleAttack(float _DeltaTime)
{
	if (BusterTickCount == 0)
	{
		ABuster* A_Buster = GetWorld()->SpawnActor<ABuster>();
		A_Buster->SetActorLocation(GetActorLocation()); // 상세 위치 조절 TODO
		std::string BusterName;
		if (DirState == EActorDir::Right)
		{
			A_Buster->SetDir(FVector::Right);
			BusterName = "Buster_Default_Right";
		}
		else if (DirState == EActorDir::Left)
		{
			A_Buster->SetDir(FVector::Left);
			BusterName = "Buster_Default_Left";
		}
		A_Buster->SetBusterState(EBusterState::DefaultCharge);
		A_Buster->SetBusterAnimation(BusterName);

		BusterTickCount++;
	}

	StateChange(EEgseuState::IdleAttack_Loop);
	BusterTickCount = 0;
	return;
}

void AEgseu::IdleAttack_Loop(float _DeltaTime)
{
	if (1.0f < BusterChargTime) // 차지 모션 대기 시간.
	{
		StateChange(EEgseuState::Idle);
		return;
	}

	if (true == UEngineInput::IsPress(VK_LEFT) ||
		true == UEngineInput::IsPress(VK_RIGHT))
	{
		StateChange(EEgseuState::IdleRun);
		return;
	}

	// 발싸!!
	if (true == UEngineInput::IsUp('X'))
	{
		StateChange(EEgseuState::IdleAttack_End);
		return;
	}
	if (true == UEngineInput::IsFree('X'))
	{
		StateChange(EEgseuState::Idle);
		return;
	}
}

void AEgseu::IdleAttack_End(float _DeltaTime)
{
	BusterDelayTime += _DeltaTime;

	if (1.0f <= BusterChargTime && BusterChargTime < 2.0f)
	{
		if (BusterTickCount == 0)
		{
			ABuster* A_Buster = GetWorld()->SpawnActor<ABuster>();
			A_Buster->SetActorLocation(GetActorLocation()); // 상세 위치 조절 TODO
			std::string BusterName;
			if (DirState == EActorDir::Right)
			{
				A_Buster->SetDir(FVector::Right);
				BusterName = "Buster_Middle_Right";
			}
			else if (DirState == EActorDir::Left)
			{
				A_Buster->SetDir(FVector::Left);
				BusterName = "Buster_Middle_Left";
			}
			A_Buster->SetBusterState(EBusterState::MiddleCharge);
			A_Buster->SetBusterAnimation(BusterName);
			BusterTickCount++;
		}
	}
	else if (2.0f <= BusterChargTime)
	{
		if (BusterTickCount == 0)
		{
			ABuster* A_Buster = GetWorld()->SpawnActor<ABuster>();
			A_Buster->SetActorLocation(GetActorLocation()); // 상세 위치 조절 TODO
			std::string BusterName;
			if (DirState == EActorDir::Right)
			{
				A_Buster->SetDir(FVector::Right);
				BusterName = "Buster_Pull_Right";
			}
			else if (DirState == EActorDir::Left)
			{
				A_Buster->SetDir(FVector::Left);
				BusterName = "Buster_Pull_Left";
			}
			A_Buster->SetBusterState(EBusterState::PullCharge);
			A_Buster->SetBusterAnimation(BusterName);
			BusterTickCount++;
		}
	}

	if (1.0f <= BusterDelayTime)
	{
		BusterDelayTime = 0.0f;
		StateChange(EEgseuState::Idle);
		return;
	}

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
	PlayerRender->ChangeAnimation(GetAnimationName("Dash_End"));
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

#pragma region DashAttack
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

#pragma region IdleRun
void AEgseu::IdleRunStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Run_Ready"));
	DirCheck();
}

void AEgseu::IdleRun_LoopStart()
{
	JumpVector = FVector::Zero; // 벽 타고 왔을 때 점프 Vector 초기화.

	if (ChangeAnimationFrame == 0)
	{
		PlayerRender->ChangeAnimation(GetAnimationName("Run"));
	}
	else
	{
		PlayerRender->ChangeAnimation(GetAnimationName("Run"), true, ChangeAnimationFrame, PlayerRender->GetCurAnimationTime());
		ChangeAnimationFrame = 0;
	}
	DirCheck();
}

void AEgseu::IdleRun_EndStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Idle"));
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

	if (true == PlayerRender->IsCurAnimationEnd())
	{
		StateChange(EEgseuState::IdleRun_Loop);
		return;
	}
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
		StateChange(EEgseuState::RunAttack);
		return;
	}

	// 발싸!!
	if (true == UEngineInput::IsUp('X'))
	{
		StateChange(EEgseuState::RunAttack_End);
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
		StateChange(EEgseuState::IdleRun_End);
		return;
	}
}

void AEgseu::IdleRun_End(float _DeltaTime)
{
	StateChange(EEgseuState::Idle);
	return;
}
#pragma endregion

#pragma region RunAttack
void AEgseu::RunAttackStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Run_Attack_Start"));
	DirCheck();
	{
		ABuster* A_Buster = GetWorld()->SpawnActor<ABuster>();
		A_Buster->SetActorLocation(GetActorLocation()); // 상세 위치 조절 TODO
		std::string BusterName;
		if (DirState == EActorDir::Right)
		{
			A_Buster->SetDir(FVector::Right);
			BusterName = "Buster_Default_Right";
		}
		else if (DirState == EActorDir::Left)
		{
			A_Buster->SetDir(FVector::Left);
			BusterName = "Buster_Default_Left";
		}
		A_Buster->SetBusterState(EBusterState::DefaultCharge);
		A_Buster->SetBusterAnimation(BusterName);
		BusterTickCount++;
	}
}

void AEgseu::RunAttack_LoopStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Run_Attack_Loop"));
	DirCheck();
}

void AEgseu::RunAttack_EndStart()
{

}

void AEgseu::RunAttack(float _DeltaTime) // 생각보다 많이 들어옴.
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

	if (true == UEngineInput::IsFree(VK_LEFT) &&
		true == UEngineInput::IsFree(VK_RIGHT) &&
		true == UEngineInput::IsFree(VK_UP) &&
		true == UEngineInput::IsFree(VK_DOWN))
	{
		StateChange(EEgseuState::Idle);
		return;
	}
	

	if (true == PlayerRender->IsCurAnimationEnd())
	{
		BusterTickCount = 0;
		StateChange(EEgseuState::RunAttack_Loop);
		return;
	}
}

void AEgseu::RunAttack_Loop(float _DeltaTime)
{
	AttackLoopTime += _DeltaTime;

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

	if (true == UEngineInput::IsFree(VK_LEFT) &&
		true == UEngineInput::IsFree(VK_RIGHT) &&
		true == UEngineInput::IsFree(VK_UP) &&
		true == UEngineInput::IsFree(VK_DOWN))
	{
		StateChange(EEgseuState::Idle);
		return;
	}


	if (1.0f <= AttackLoopTime)
	{
		AttackLoopTime = 0.0f;
		if (true == UEngineInput::IsPress(VK_RIGHT) || true == UEngineInput::IsPress(VK_LEFT))
		{
			ChangeAnimationFrame = PlayerRender->GetCurAnimationImageFrame();
			BusterTickCount = 0;
			StateChange(EEgseuState::IdleRun_Loop);
			return;
		}
		else
		{
			BusterTickCount = 0;
			StateChange(EEgseuState::Idle);
			return;
		}
	}

	if (true == UEngineInput::IsUp('X'))
	{
		BusterTickCount = 0;
		if (true == UEngineInput::IsPress(VK_RIGHT) || true == UEngineInput::IsPress(VK_LEFT))
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

void AEgseu::RunAttack_End(float _DeltaTime)
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

	if (1.0f <= BusterChargTime && BusterChargTime < 2.0f)
	{
		if (BusterTickCount == 0)
		{
			ABuster* A_Buster = GetWorld()->SpawnActor<ABuster>();
			A_Buster->SetActorLocation(GetActorLocation()); // 상세 위치 조절 TODO
			std::string BusterName;
			if (DirState == EActorDir::Right)
			{
				A_Buster->SetDir(FVector::Right);
				BusterName = "Buster_Middle_Right";
			}
			else if (DirState == EActorDir::Left)
			{
				A_Buster->SetDir(FVector::Left);
				BusterName = "Buster_Middle_Left";
			}
			A_Buster->SetBusterState(EBusterState::MiddleCharge);
			A_Buster->SetBusterAnimation(BusterName);
			BusterTickCount++;
		}
	}
	else if (2.0f <= BusterChargTime)
	{
		if (BusterTickCount == 0)
		{
			ABuster* A_Buster = GetWorld()->SpawnActor<ABuster>();
			A_Buster->SetActorLocation(GetActorLocation()); // 상세 위치 조절 TODO
			std::string BusterName;
			if (DirState == EActorDir::Right)
			{
				A_Buster->SetDir(FVector::Right);
				BusterName = "Buster_Pull_Right";
			}
			else if (DirState == EActorDir::Left)
			{
				A_Buster->SetDir(FVector::Left);
				BusterName = "Buster_Pull_Left";
			}
			A_Buster->SetBusterState(EBusterState::PullCharge);
			A_Buster->SetBusterAnimation(BusterName);
			BusterTickCount++;
		}
	}

	if (1.0f <= BusterDelayTime)
	{
		BusterDelayTime = 0.0f;
		if (true == UEngineInput::IsPress(VK_RIGHT) || true == UEngineInput::IsPress(VK_LEFT))
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
#pragma endregion

#pragma region RunDash
void AEgseu::RunDashStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Dash_Start"));
}

void AEgseu::RunDash_LoopStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Dash_Loop"));
}

void AEgseu::RunDash_EndStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Dash_End"));
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

	MoveUpdate(_DeltaTime);

	if (true == UEngineInput::IsUp('Z'))
	{
		StateChange(EEgseuState::IdleRun_Loop);
		return;
	}

	if (true == UEngineInput::IsPress('Z'))
	{
		StateChange(EEgseuState::RunDash_Loop);
		return;
	}
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
			StateChange(EEgseuState::RunDash_End);
			return;
		}
	}
}

void AEgseu::RunDash_End(float _DeltaTime)
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

#pragma region RunDashJump
void AEgseu::RunDashJumpStart()
{
	JumpVector = JumpPower;
	PlayerRender->ChangeAnimation(GetAnimationName("Jump_Start"));
	DirCheck();
}

void AEgseu::RunDashJump_LoopStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Jumping"));
	DirCheck();
}

void AEgseu::RunDashJump_EndStart()
{
	JumpVector = FVector::Zero;
	PlayerRender->ChangeAnimation(GetAnimationName("Jump_End"));
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

	MoveUpdate(_DeltaTime);

	if (true == PlayerRender->IsCurAnimationEnd())
	{
		StateChange(EEgseuState::RunDashJump_Loop);
		return;
	}
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

	MoveUpdate(_DeltaTime);

	// 공격

	// 땅에 닿음
	Color8Bit Color = UContentsGlobalData::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
	{
		DashVector = FVector::Zero;
		StateChange(EEgseuState::RunDashJump_End);
		return;
	}
}

void AEgseu::RunDashJump_End(float _DeltaTime)
{
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

#pragma region RunDashJumpAttack
void AEgseu::RunDashJumpAttackStart()
{
}

void AEgseu::RunDashJumpAttack_LoopStart()
{
}

void AEgseu::RunDashJumpAttack_EndStart()
{
}


void AEgseu::RunDashJumpAttack(float _DeltaTime)
{
}

void AEgseu::RunDashJumpAttack_Loop(float _DeltaTime)
{
}

void AEgseu::RunDashJumpAttack_End(float _DeltaTime)
{
}
#pragma endregion

#pragma region RunJump
void AEgseu::RunJumpStart()
{
	JumpVector = JumpPower;
	PlayerRender->ChangeAnimation(GetAnimationName("Jump_Start"));
	DirCheck();
}

void AEgseu::RunJump_LoopStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Jumping"));
	DirCheck();
}

void AEgseu::RunJump_EndStart()
{
	JumpVector = FVector::Zero;
	PlayerRender->ChangeAnimation(GetAnimationName("Jump_End"));
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

	MoveUpdate(_DeltaTime);

	// 점프 중 벽 체크
	bool WallChecl = CalWallCheck();
	if (true == WallChecl)
	{
		StateChange(EEgseuState::WallCling);
		return;
	}

	if (true == PlayerRender->IsCurAnimationEnd())
	{
		StateChange(EEgseuState::RunJump_Loop);
		return;
	}
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

	MoveUpdate(_DeltaTime);

	// 공격


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
		StateChange(EEgseuState::RunJump_End);
		return;
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

#pragma region RunJumpAttack
void AEgseu::RunJumpAttackStart()
{
}

void AEgseu::RunJumpAttack_LoopStart()
{
}

void AEgseu::RunJumpAttack_EndStart()
{
}


void AEgseu::RunJumpAttack(float _DeltaTime)
{
}

void AEgseu::RunJumpAttack_Loop(float _DeltaTime)
{
}

void AEgseu::RunJumpAttack_End(float _DeltaTime)
{
}
#pragma endregion

#pragma region WallCling
void AEgseu::WallClingStart()
{
	// 벽 잡는 애니메이션.
	PlayerRender->ChangeAnimation(GetAnimationName("WallCling_Start"));
	//DirCheck();
}

void AEgseu::WallCling(float _DeltaTime)
{
	MoveUpdate(_DeltaTime);

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
		LastMoveVector = FVector::Zero;
		GravityVector = FVector::Down * 10.0f;
		StateChange(EEgseuState::WallCling_Loop);
		return;
	}
}
#pragma endregion

#pragma region WallCling_Loop
void AEgseu::WallCling_LoopStart()
{
	JumpVector = FVector::Zero;
	PlayerRender->ChangeAnimation(GetAnimationName("WallCling_Loop"));
	//DirCheck();
}

void AEgseu::WallCling_Loop(float _DeltaTime)
{
	// 벽타고 미끌어지는 수치 조절 해야 함.

	MoveUpdate(_DeltaTime);

	// 벽을 잡고 있는 중에 공격
	// -----

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
		JumpVector = (JumpPower * 2) + (FVector::Left * MoveSpeed);
	}
	PlayerRender->ChangeAnimation(GetAnimationName("WallKick"));

	WallKickTime = 0.0f;
}

/// <summary>
/// 벽에서 다시 점프.
/// </summary>
/// <param name="_DeltaTime"></param>
void AEgseu::WallKick(float _DeltaTime)
{
	WallKickTime += _DeltaTime;
	MoveUpdate(_DeltaTime);
	if (WallKickTime < 0.2f) // 해당 시간 동안 아무것도 못함.(선 딜)
	{
		return;
	}
	
	// JumpVector 에 남아있던 좌우 방향 힘 초기화
	JumpVector = JumpVector - (FVector::Right * MoveSpeed);

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

#pragma region Hit
void AEgseu::HitStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Hit"));
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
			Hit_InvincibilityTime = 0.0f;
			StateChange(EEgseuState::IdleRun_Loop);
			return;
		}
		else
		{
			Hit_Count = 0;
			Hit_InvincibilityTime = 0.0f;
			StateChange(EEgseuState::Idle);
			return;
		}
	}
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
	CheckPos_2.Y -= PlayerRender->GetImage()->GetScale().iY() / 2; // 캐릭터 중앙.

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
	if (true == UEngineInput::IsPress('X'))
	{
		BusterChargTime += _DeltaTime;
	}
}


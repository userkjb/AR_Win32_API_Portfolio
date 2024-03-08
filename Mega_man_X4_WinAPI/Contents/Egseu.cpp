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
	// Debug
	if (Debug_Num != static_cast<int>(State))
	{
		UEngineDebug::OutPutDebugText(std::to_string(static_cast<int>(State)));
		Debug_Num = static_cast<int>(State);
	}
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
	PlayerRender = CreateImageRenderer(static_cast<int>(ERenderOrder::Player));
	PlayerRender->SetImage("x_Idle_Right.png");
	PlayerRender->AutoImageScale(2.0f);
	//PlayerRender->SetTransform({ {0,0}, {35 * 3, 80 * 3} });

	PlayerCollision = CreateCollision(ECollisionOrder::Player);
	PlayerCollision->SetPosition({ 0, -50 });
	PlayerCollision->SetScale({ 80, 100 });
	PlayerCollision->SetColType(ECollisionType::Rect);

	// ===== Animation ======================================================
	// Summon
	PlayerRender->CreateAnimation("Summon", "x_Start.png", 0, 0, 0.2f, true);
	PlayerRender->CreateAnimation("Summon_Loop", "x_Start.png", 1, 16, 0.13f, false);

	// Idle
	PlayerRender->CreateAnimation("Idle_Right", "X_Idle_Right.png", 0, 3, 0.3f, true);
	PlayerRender->CreateAnimation("Idle_Left", "X_Idle_Left.png", 0, 3, 0.3f, true);

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
	// x_Attack_Right �̹����� ���� �̹����� �ƴϿ��� �Ŀ� �ٲ�� ��.
	PlayerRender->CreateAnimation("Idle_Attack_Start_Right", "X_Idle_Attack_Right.png", 0, 2, 0.2f, false);
	PlayerRender->CreateAnimation("Idle_Attack_Start_Left", "X_Idle_Attack_Left.png", 0, 2, 0.2f, false);
	PlayerRender->CreateAnimation("Idle_Attack_Loop_Right", "X_Idle_Attack_Right.png", 0, 0, 0.5f, true);
	PlayerRender->CreateAnimation("Idle_Attack_Loop_Left", "X_Idle_Attack_Left.png", 0, 0, 0.5f, true);
	PlayerRender->CreateAnimation("Idle_Attack_End_Right", "X_Idle_Attack_End_Right.png", 0, 1, 0.1f, false);
	PlayerRender->CreateAnimation("Idle_Attack_End_Left", "X_Idle_Attack_End_Left.png", 0, 1, 0.1f, false);

	PlayerRender->CreateAnimation("Run_Attack_Start_Right", "Run_Attack_Right.png", 0, 1, 0.05f, false);
	PlayerRender->CreateAnimation("Run_Attack_Start_Left", "Run_Attack_Left.png", 0, 1, 0.05f, false);
	PlayerRender->CreateAnimation("Run_Attack_Loop_Right", "Run_Attack_Right.png", 2, 15, 0.1f, true);
	PlayerRender->CreateAnimation("Run_Attack_Loop_Left", "Run_Attack_Left.png", 2, 15, 0.1f, true);

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

	// ���� ����� �ٸ��ٸ�, 
	if (Dir != DirState)
	{
		// ���� �ִϸ��̼ǿ��� �ݴ�Ǵ� �ִϸ��̼����� �ٲ���� �Ѵ�.

		DirState = Dir;

		// Animation�� �ٲ���� �Ѵ�.
		// ���� �ٲ��ֱ� ���� GetAnimationName �Լ��� ����� �ְ� ����Ѵ�.
		std::string Name = GetAnimationName(CurAnimationName);

		// ChangeAnimation�� Animation ���߿� �ٲ�� ������ ���� Frame�� �˾ƾ� �Դ�.
		// GetCurAnimationFrame �Լ��� ����� ����Ѵ�.
		// ����� ������ Animation�� �ð��� �˾ƾ� �Ѵ�.
		// GetCurAnimationTime �Լ��� ����� ����Ѵ�.
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

	// ���¸� �޴´�. Ex) Idle
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

	// Actor�� ������ ��������.
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
	// ������ ������ ���� ������.
	StateChange(EEgseuState::Idle);
	return;
}
#pragma endregion

#pragma region Idle
void AEgseu::IdleStart()
{
	JumpVector = FVector::Zero; // �� Ÿ�� �������� �� ���� �ʱ�ȭ.
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
	// ������ �ִµ� ���� Ű�� ������ ��.
	if (true == UEngineInput::IsPress(VK_LEFT) ||
		true == UEngineInput::IsPress(VK_RIGHT))
	{
		StateChange(EEgseuState::IdleRun);
		return;
	}

	// ���� C
	if (true == UEngineInput::IsDown('C'))
	{
		StateChange(EEgseuState::IdleJump);
		return;
	}
	
	// ���� X
	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault);
		StateChange(EEgseuState::IdleAttack_Down);
		return;
	}

	// �߽�!!!
	if (true == UEngineInput::IsUp('X'))
	{
		if (BusterChargTime >= 1.0f)
		{
			BusterDelayTime = 0.0f;
			if (1.0f <= BusterChargTime && BusterChargTime < 2.0f)
			{
				BusterCreate(EBusterState::CreateMiddle);
			}
			else if (2.0f <= BusterChargTime)
			{
				BusterCreate(EBusterState::CreatePull);
			}
			StateChange(EEgseuState::IdleAttack_Up);
			return;
		}
	}

	// �뽬 Z
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
		BusterCreate(EBusterState::CreateDefault);
		StateChange(EEgseuState::JumpAttack_Down);
		return;
	}

	//if (true == UEngineInput::IsUp('X'))
	//{
	//	StateChange(EEgseuState::JumpAttack_Up);
	//	return;
	//}

	if (true == PlayerRender->IsCurAnimationEnd())
	{
		StateChange(EEgseuState::IdleJump_Loop);
		return;
	}
}

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

	// ����
	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault);
		StateChange(EEgseuState::JumpAttack_Down_Loop);
		return;
	}

	//// í�� ����!!
	//if (true == UEngineInput::IsUp('X'))
	//{
	//	StateChange(EEgseuState::JumpAttack_Up_Loop);
	//	return;
	//}

	// '��'�� ����
	bool WallChecl = CalWallCheck();
	if (true == WallChecl)
	{
		StateChange(EEgseuState::WallCling);
		return;
	}

	// '��'�� ����
	Color8Bit Color = UContentsGlobalData::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY() + 5, Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
	{
		JumpVector = FVector::Zero;
		StateChange(EEgseuState::IdleJump_End);
		return;
	}
}


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
		BusterCreate(EBusterState::CreateDefault);
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

#pragma region JumpAttack Down
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

	// �� ����?
	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault);
	}

	// 0.5�ʰ� ������.
	if (BusterDelayTime >= BusterDelayTimeMax)
	{
		StateChange(EEgseuState::IdleJump);
	}

	// �ٴ��̳�? -> ���� �ٴ�...
	Color8Bit Color = UContentsGlobalData::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
	{
		JumpVector = FVector::Zero;
		StateChange(EEgseuState::IdleJump_End);
		return;
	}
}

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
		BusterCreate(EBusterState::CreateDefault);
	}

	// Ư�� �ð����� �̺�Ʈ�� ������ �ٽ� IdleJump_Loop�� �̵�.
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

void AEgseu::JumpAttack_Down_EndStart()
{
	int CurFrame = PlayerRender->GetCurAnimationFrame();
	PlayerRender->ChangeAnimation(GetAnimationName("Jump_End_Attack"), false, CurFrame);
}

void AEgseu::JumpAttack_Down_End(float _DeltaTime)
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
		BusterCreate(EBusterState::CreateDefault);
	}

	if (BusterDelayTime >= BusterDelayTimeMax)
	{
		StateChange(EEgseuState::IdleJump_End);
		return;
	}

	if (true == PlayerRender->IsCurAnimationEnd())
	{
		StateChange(EEgseuState::IdleJump_End);
		return;
	}
}
#pragma endregion

#pragma region JumpAttack Up
void AEgseu::JumpAttack_UpStart()
{
}

void AEgseu::JumpAttack_Up(float _DeltaTime)
{
}

void AEgseu::JumpAttack_Up_LoopStart()
{
}

void AEgseu::JumpAttack_Up_Loop(float _DeltaTime)
{
}

void AEgseu::JumpAttack_Up_EndStart()
{
}

void AEgseu::JumpAttack_Up_End(float _DeltaTime)
{
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
		BusterCreate(EBusterState::CreateDefault);
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

void AEgseu::IdleAttack_Down_Loop(float _DeltaTime) // Down �̴� Press �̴� ���� ������ ���;� ��.
{
	BusterDelayTime += _DeltaTime;

	if (true == UEngineInput::IsPress(VK_LEFT) ||
		true == UEngineInput::IsPress(VK_RIGHT))
	{
		StateChange(EEgseuState::IdleRun_Loop);
		return;
	}

	// ��� �߿� X Ű�� ������,
	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault);
	}

	// 0.5�ʰ� �����ٸ�,
	if (BusterDelayTimeMax <= BusterDelayTime) // ���� ��� ��� �ð�.
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

	// 0.5�ʰ� �����ٸ�,
	if (BusterDelayTimeMax <= BusterDelayTime) // ���� ��� ��� �ð�.
	{
		StateChange(EEgseuState::IdleAttack_Up_End);
		return;
	}

	// ��� �߿� X Ű�� ������,
	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault);
	}
}

void AEgseu::IdleAttack_Up_EndStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Idle_Attack_End"));
	BusterDelayTime = 0.0f;
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

	// ���� (���߿� �߰�����. ���� ���� �ƴϴ�. �뽬������ �����ϸ� ���� �� �� �ִ�.)

	// ���߿� �뽬 ����.
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

	// ���� ��� �߰��ϴ� ������.

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

#pragma region Idle -> Run
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
		BusterCreate(EBusterState::CreateDefault);
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
			BusterCreate(EBusterState::CreateMiddle);
		}
		else if (2.0f <= BusterChargTime)
		{
			BusterCreate(EBusterState::CreatePull);
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
	JumpVector = FVector::Zero; // �� Ÿ�� ���� �� ���� Vector �ʱ�ȭ.
	
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

	// ����
	if (true == UEngineInput::IsDown('C'))
	{
		StateChange(EEgseuState::RunJump);
		return;
	}

	// ����
	if (true == UEngineInput::IsDown('X'))
	{
		BusterChargTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault);
		StateChange(EEgseuState::RunAttack_Down_Loop);
		return;
	}

	// �߽�!!
	if (true == UEngineInput::IsUp('X'))
	{
		if (BusterChargTime <= 0.8f)
		{
			return;
		}
		if (1.0f <= BusterChargTime && BusterChargTime < 2.0f)
		{
			BusterCreate(EBusterState::CreateMiddle);
		}
		else if (2.0f <= BusterChargTime)
		{
			BusterCreate(EBusterState::CreatePull);
		}
		BusterDelayTime = 0.0f;
		StateChange(EEgseuState::RunAttack_Up_Loop);
		return;
	}

	// �뽬
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
		BusterCreate(EBusterState::CreateDefault);
	}

	// �����̰� ������,
	if (BusterDelayTime >= BusterDelayTimeMax)
	{
		StateChange(EEgseuState::IdleRun);
		return;
	}

	if (true == PlayerRender->IsCurAnimationEnd())
	{
		StateChange(EEgseuState::IdleRun); // ������ ����� �� �ִ�.
		return;
	}
	
	// �������� ���߸� Idle��.
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

	// �� ����?
	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault);
	}

	if (true == UEngineInput::IsDown('C'))
	{
		BusterDelayTime = 0.0f;
		StateChange(EEgseuState::RunJump);
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

	// 0.5�ʰ� ������,
	if (BusterDelayTimeMax <= BusterDelayTime)
	{
		BusterDelayTime = 0.0f;
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
	if (BusterDelayTime == 0.0f) // Run Loop ���� ����.
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

	// 0.5�ʰ� ������,
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

	// ����
	if (true == UEngineInput::IsDown('C'))
	{
		DashTime = 0.0f;
		StateChange(EEgseuState::RunDashJump);
		return;
	}

	// ����

	// �뽬 ��!
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

	// �뽬 ���� �ð�
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

	// ���� ��� �߰��ϴ� ������.

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

	// ����

	// ���� ����
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

#pragma region RunDashJumpAttack Down
void AEgseu::RunDashJumpAttack_DownStart()
{
}
void AEgseu::RunDashJumpAttack_Down(float _DeltaTime)
{
}
void AEgseu::RunDashJumpAttack_Down_LoopStart()
{
}
void AEgseu::RunDashJumpAttack_Down_Loop(float _DeltaTime)
{
}
void AEgseu::RunDashJumpAttack_Down_EndStart()
{
}
void AEgseu::RunDashJumpAttack_Down_End(float _DeltaTime)
{
}
#pragma endregion
#pragma region RunDashJumpAttack Up
void AEgseu::RunDashJumpAttack_UpStart()
{
}

void AEgseu::RunDashJumpAttack_Up(float _DeltaTime)
{
}

void AEgseu::RunDashJumpAttack_Up_LoopStart()
{
}

void AEgseu::RunDashJumpAttack_Up_Loop(float _DeltaTime)
{
}

void AEgseu::RunDashJumpAttack_Up_EndStart()
{
}

void AEgseu::RunDashJumpAttack_Up_End(float _DeltaTime)
{
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

	MoveUpdate(_DeltaTime);

	// ����
	if (true == UEngineInput::IsDown('X'))
	{
		BusterCreate(EBusterState::CreateDefault);
		StateChange(EEgseuState::RunJumpAttack_Down);
		return;
	}

	// ���� ����
	if (true == UEngineInput::IsUp('X'))
	{
		if (BusterChargTime <= 0.8f)
		{
			return;
		}
		if (1.0f <= BusterChargTime && BusterChargTime < 2.0f)
		{
			BusterCreate(EBusterState::CreateMiddle);
		}
		else if (2.0f <= BusterChargTime)
		{
			BusterCreate(EBusterState::CreatePull);
		}
		BusterDelayTime = 0.0f;
		StateChange(EEgseuState::RunJumpAttack_Up);
		return;
	}


	// ���� �� �� üũ
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

	MoveUpdate(_DeltaTime);

	// ����
	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault);
		StateChange(EEgseuState::RunJumpAttack_Down_Loop);
		return;
	}

	// ���� ����
	if (true == UEngineInput::IsUp('X'))
	{
		if (BusterChargTime <= 0.8f)
		{
			return;
		}
		if (1.0f <= BusterChargTime && BusterChargTime < 2.0f)
		{
			BusterCreate(EBusterState::CreateMiddle);
		}
		else if (2.0f <= BusterChargTime)
		{
			BusterCreate(EBusterState::CreatePull);
		}
		BusterDelayTime = 0.0f;
		StateChange(EEgseuState::RunJumpAttack_Up_Loop);
		return;
	}


	// ���� �� �� üũ
	bool WallChecl = CalWallCheck();
	if (true == WallChecl)
	{
		StateChange(EEgseuState::WallCling);
		return;
	}

	// ���� ����
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
	PlayerRender->ChangeAnimation(GetAnimationName("Jump_End"));
}

void AEgseu::RunJump_End(float _DeltaTime)
{
	// ���� �� �� üũ
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
		BusterCreate(EBusterState::CreateDefault);
	}

	// ���� �� �� üũ
	bool WallChecl = CalWallCheck();
	if (true == WallChecl)
	{
		StateChange(EEgseuState::WallCling);
		return;
	}

	// 0.5�ʰ� ������(���ɼ� ����)
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
	if (BusterDelayTime == 0.0f) // x ������ �� ����.
	{
		int CurFrame = PlayerRender->GetCurAnimationFrame();
		PlayerRender->ChangeAnimation(GetAnimationName("Jump_Ing_Attack"), false, CurFrame);
	}
	else // ���� ���¿��� �̾ ����.
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

	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault);
	}

	if (BusterDelayTime >= BusterDelayTimeMax)
	{
		StateChange(EEgseuState::RunJump_Loop);
		return;
	}

	bool WallChecl = CalWallCheck();
	if (true == WallChecl)
	{
		StateChange(EEgseuState::WallCling);
		return;
	}

	// ���� ����
	Color8Bit Color = UContentsGlobalData::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
	{
		JumpVector = FVector::Zero;
		StateChange(EEgseuState::RunJumpAttack_Down_End);
		return;
	}
}

void AEgseu::RunJumpAttack_Down_EndStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Jump_End_Attack"));
}
void AEgseu::RunJumpAttack_Down_End(float _DeltaTime) // ���� ��� ����.
{
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
		BusterDelayTime = 0.0f;
		StateChange(EEgseuState::RunJump_End);
		return;
	}

	if (true == UEngineInput::IsDown('X'))
	{
		BusterDelayTime = 0.0f;
		BusterCreate(EBusterState::CreateDefault);
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

void AEgseu::RunJumpAttack_Up_LoopStart()
{
	if (BusterDelayTime == 0.0f) // x ���� �� ����.
	{
		int CurFrame = PlayerRender->GetCurAnimationFrame();
		PlayerRender->ChangeAnimation(GetAnimationName("Jump_Ing_Attack"), false, CurFrame);
	}
	else // ���� ���¿��� �̾ ����.
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
		BusterCreate(EBusterState::CreateDefault);
	}

	if (BusterDelayTime >= BusterDelayTimeMax)
	{
		StateChange(EEgseuState::RunJump_Loop);
		return;
	}

	bool WallChecl = CalWallCheck();
	if (true == WallChecl)
	{
		StateChange(EEgseuState::WallCling);
		return;
	}

	// ���� ����
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

void AEgseu::RunJumpAttack_Up_EndStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Jump_End_Attack"));
}

void AEgseu::RunJumpAttack_Up_End(float _DeltaTime)
{
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
		BusterDelayTime = 0.0f;
		StateChange(EEgseuState::RunJump_End);
		return;
	}
}
#pragma endregion

#pragma region WallCling
void AEgseu::WallClingStart()
{
	// �� ��� �ִϸ��̼�.
	PlayerRender->ChangeAnimation(GetAnimationName("WallCling_Start"));
	//DirCheck();
}

void AEgseu::WallCling(float _DeltaTime)
{
	MoveUpdate(_DeltaTime);

	// �� ��� �� �߿� ����
	if (true == UEngineInput::IsDown('C'))
	{
		StateChange(EEgseuState::WallKick);
		return;
	}
	
	// �� ��� �ִϸ��̼��� ������,
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
	// ��Ÿ�� �̲������� ��ġ ���� �ؾ� ��.

	MoveUpdate(_DeltaTime);

	// ���� ��� �ִ� �߿� ����
	// -----

	// ���� ��� �ִ� �߿� �ٽ� ����
	if (true == UEngineInput::IsDown('C'))
	{
		StateChange(EEgseuState::WallKick);
		return;
	}

	// ���� ������,
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
	// �߷� �� �ʱ�ȭ.(���ϸ� ��� �׿��� ������ �� �� ����)
	GravityVector = FVector::Zero;
	
	// �ݴ� �������� ����
	if (0 == static_cast<int>(DirState)) // Left
	{
		JumpVector = JumpPower + (FVector::Right * MoveSpeed);
	}
	else // Right
	{
		JumpVector = JumpPower + (FVector::Left * MoveSpeed);
	}
	PlayerRender->ChangeAnimation(GetAnimationName("WallKick"));

	WallKickTime = 0.0f;
}

/// <summary>
/// ������ �ٽ� ����.
/// </summary>
/// <param name="_DeltaTime"></param>
void AEgseu::WallKick(float _DeltaTime)
{
	WallKickTime += _DeltaTime;
	MoveUpdate(_DeltaTime);
	if (WallKickTime < 0.2f) // �ش� �ð� ���� �ƹ��͵� ����.(�� ��)
	{
		return;
	}
	
	// JumpVector �� �����ִ� �¿� ���� �� �ʱ�ȭ
	float JumpXValue = JumpVector.X;
	if (JumpXValue != 0.0f)
	{
		JumpVector.X = 0.0f;
	}
	//JumpVector = JumpVector - (FVector::Right * MoveSpeed);

	// �� ���� ���� ������.
	if (2 == PlayerRender->GetCurAnimationImageFrame())
	{
		// �뽬 ��ư�� ������ �ִٸ�,
		if (true == UEngineInput::IsPress('Z'))
		{
			StateChange(EEgseuState::RunDashJump_Loop);
			return;
		}
		// �뽬 ��ư�� ������ ���� �ʴٸ�,
		else if (true == UEngineInput::IsFree('Z'))
		{
			StateChange(EEgseuState::RunJump_Loop);
			return;
		}
		// ����Ű�� ������ ���� �ʴٸ�,
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

	// �����ð�.
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
	// Actor�� ������ ��������.
	FVector CheckPos = GetActorLocation();

	// �浹�� üũ�ϱ� ����
	// ����Ű�� ���� Actor�� �������� �ű��. 
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

	// ���� CheckPos�� ����ؼ� Map�� �浹 üũ�� �Ѵ�.
	Color8Bit Color = UContentsGlobalData::ColMapImage->GetColor(CheckPos.iX(), CheckPos.iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
	{
		RunVector = FVector::Zero;
		DashVector = FVector::Zero;
	}


	// Ű���� �Է��� ������ �ӵ��� Zero��.
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


// �浹.
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
/// ���� ���� üũ
/// </summary>
/// <returns></returns>
bool AEgseu::CalWallCheck()
{
	// Actor�� ������ ��������.
	FVector CheckPos_1 = GetActorLocation();
	FVector CheckPos_2 = GetActorLocation();

	// �浹�� üũ�ϱ� ����
	// ����Ű�� ���� Actor�� �������� �ű��. 
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
	CheckPos_2.Y -= PlayerRender->GetImage()->GetScale().iY() / 4; // ĳ���� �߾�.

	// ���� CheckPos�� ����ؼ� Map�� �浹 üũ�� �Ѵ�.
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

	// ���� �̹���
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

void AEgseu::BusterCreate(EBusterState _BusterState)
{
	ABuster* A_Buster = GetWorld()->SpawnActor<ABuster>();
	FVector BusterPos = GetActorLocation();
	A_Buster->SetActorLocation({ BusterPos.iX(), BusterPos.iY() - 64 }); // �� ��ġ ���� TODO
	
	std::string BusterName;
	if (DirState == EActorDir::Right)
	{
		A_Buster->SetDirState(EActorDir::Right);
	}
	else if (DirState == EActorDir::Left)
	{
		A_Buster->SetDirState(EActorDir::Left);
	}
	A_Buster->SetBusterState(_BusterState);
}


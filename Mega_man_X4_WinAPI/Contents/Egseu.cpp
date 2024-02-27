#include "Egseu.h"
#include <EngineCore/EngineCore.h> // Helper
#include "Buster.h"

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
}

void AEgseu::ChargeBeginPlay()
{
	MiddleChargeRender = CreateImageRenderer(static_cast<int>(ERenderOrder::Buster));
	MiddleChargeRender->SetImage("Charging_1.png");
	UWindowImage* ChargeImage_1 = MiddleChargeRender->GetImage();
	FVector ChargeScale_1 = ChargeImage_1->GetScale();
	MiddleChargeRender->SetTransform({ { 0, 0 }, {ChargeScale_1.iX() / 9, ChargeScale_1.iY()} });

	PullChargeRender = CreateImageRenderer(static_cast<int>(ERenderOrder::Buster));
	PullChargeRender->SetImage("Charging_2.png");
	UWindowImage* ChargeImage_2 = PullChargeRender->GetImage();
	FVector ChargeScale_2 = ChargeImage_2->GetScale();
	PullChargeRender->SetTransform({ { 0, 0 }, {ChargeScale_2.iX() / 4 , ChargeScale_2.iY()} });



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
	PlayerRender->SetTransform({ {0,0}, {35 * 3, 80 * 3} });

	PlayerCollision = CreateCollision(ECollisionOrder::Player);
	PlayerCollision->SetScale({ 35 * 3, 80 * 3 });
	PlayerCollision->SetColType(ECollisionType::Rect);

	// ===== Animation ======================================================
	// Summon
	PlayerRender->CreateAnimation("Summon_Right", "x_Start.png", 0, 0, 0.1f, true);
	PlayerRender->CreateAnimation("Summon_Loop_Right", "x_Start.png", 1, 16, 0.07f, false);

	// Idle
	PlayerRender->CreateAnimation("Idle_Right", "x_Idle_Right.png", { 0,1,2,3,4,3,2,1 }, 0.1f, true);
	PlayerRender->CreateAnimation("Idle_Left", "x_Idle_Left.png", { 0,1,2,3,4,3,2,1 }, 0.1f, true);

	// Run
	PlayerRender->CreateAnimation("Run_Right", "x_Move_Right.png", 2, 15, 0.05f, true);
	PlayerRender->CreateAnimation("Run_Left", "x_Move_Left.png", 2, 15, 0.05f, true);

	// Jump
	PlayerRender->CreateAnimation("Jump_Start_Right", "x_Jump_Right.png", 0, 7, 0.05f, false);
	PlayerRender->CreateAnimation("Jump_Start_Left", "x_Jump_Left.png", 0, 7, 0.05f, false);
	PlayerRender->CreateAnimation("Jumping_Right", "x_Jump_Right.png", 7, 7, 0.1f, false);
	PlayerRender->CreateAnimation("Jumping_Left", "x_Jump_Left.png", 7, 7, 0.1f, false);
	PlayerRender->CreateAnimation("JumpEnd_Right", "x_Jump_Right.png", 8, 10, 0.005f, false);
	PlayerRender->CreateAnimation("JumpEnd_Left", "x_Jump_Left.png", 8, 10, 0.005f, false);


	// Dash
	PlayerRender->CreateAnimation("Dash_Start_Right", "x_Dash_Right.png", 0, 1, 0.05f, false);
	PlayerRender->CreateAnimation("Dash_Start_Left", "x_Dash_Left.png", 0, 1, 0.05f, false);
	PlayerRender->CreateAnimation("Dash_Loop_Right", "x_Dash_Right.png", 2, 3, 0.05f, true);
	PlayerRender->CreateAnimation("Dash_Loop_Left", "x_Dash_Left.png", 2, 3, 0.05f, true);
	PlayerRender->CreateAnimation("Dash_End_Right", "x_Dash_Right.png", 4, 7, 0.05f, false);
	PlayerRender->CreateAnimation("Dash_End_Left", "x_Dash_Left.png", 4, 7, 0.05f, false);


	// =====================================================================

	PlayerRender->ChangeAnimation(GetAnimationName("Summon"));
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
		case EEgseuState::Idle_Loop:
			Idle_LoopStart();
			break;
		case EEgseuState::Idle_End:
			Idle_EndStart();
			break;
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
	case EEgseuState::Idle_Loop:
		Idle_Loop(_DeltaTime);
		break;
	case EEgseuState::Idle_End:
		Idle_End(_DeltaTime);
		break;
	default :
		break;
	}
}

// Begin

#pragma region Summon Begin
void AEgseu::SummonStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Summon"));
}

void AEgseu::Summon_LoopStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Summon_Loop"));
}

void AEgseu::Summon_EndStart()
{
	PlayerRender->ChangeAnimation(GetAnimationName("Idle"));
}
#pragma endregion

#pragma region Comment
void AEgseu::IdleStart()
{
}
void AEgseu::Idle_LoopStart()
{
}

void AEgseu::Idle_EndStart()
{
}
#pragma endregion


// Tick
#pragma region Summon Tick
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
	DelayTime += _DeltaTime;
	if (true == PlayerRender->IsCurAnimationEnd())
	{
		if (1.5f < DelayTime)
		{
			StateChange(EEgseuState::Summon_End);
			DelayTime = 0.0f;
			return;
		}
	}
}

void AEgseu::Summon_End(float _DeltaTime)
{
	StateChange(EEgseuState::Idle);
	return;
}
#pragma endregion

void AEgseu::Idle(float _DeltaTime)
{
}

void AEgseu::Idle_Loop(float _DeltaTime)
{
}

void AEgseu::Idle_End(float _DeltaTime)
{
}



//#pragma region Comment
//#pragma endregion

void AEgseu::CalMoveVector()
{
	// Actor의 기준점 가져오기.
	FVector CheckPos = GetActorLocation();

	// 충돌을 체크하기 위해
	// 방향키에 맞춰 Actor의 기준점을 옮긴다. 
	switch (DirState)
	{
	case EActorDir::Left:
		CheckPos.X -= 100;
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

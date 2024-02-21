#include "Player.h"
#include <EngineCore/EngineCore.h> // Helper
#include "Buster.h"

APlayer::APlayer()
{
}

APlayer::~APlayer()
{
}

void APlayer::BeginPlay()
{
	AActor::BeginPlay();

	ChargeBeginPlay();

	Renderer = CreateImageRenderer(static_cast<int>(ERenderOrder::Player));
	Renderer->SetImage("x_Idle_Right.png");
	//UWindowImage* Image = Renderer->GetImage();
	//FVector ImageScale = Image->GetScale();// 200 100
	Renderer->SetTransform({ {0,0}, {35 * 3, 80 * 3} }); 


	// =================================================================

	// Start Animation
	Renderer->CreateAnimation("Summon", "x_Start.png", 0, 0, 0.1f, true);
	Renderer->CreateAnimation("Summon_Loop", "x_Start.png", 1, 16, 0.07f, false); // 이미지 이상함 TODO

	// Idle
	Renderer->CreateAnimation("Idle_Right", "x_Idle_Right.png", { 0,1,2,3,4,3,2,1 }, 0.1f, true);
	Renderer->CreateAnimation("Idle_Left", "x_Idle_Left.png", { 0,1,2,3,4,3,2,1 }, 0.1f, true);

	// Run
	Renderer->CreateAnimation("Run_Right", "x_Move_Right.png", 2, 15, 0.05f, true);
	Renderer->CreateAnimation("Run_Left", "x_Move_Left.png", 2, 15, 0.05f, true);
	// Run And Attack
	Renderer->CreateAnimation("Run_And_Attack_Right", "x_Move_Attack_Right.png", 0, 30, 0.05f, true);
	Renderer->CreateAnimation("Run_And_Attack_Left", "x_Move_Attack_Left.png", 0, 30, 0.05f, true);

	// Jump
	Renderer->CreateAnimation("Jump_Start_Right", "x_Jump_Right.png", 0, 7, 0.05f, false);
	Renderer->CreateAnimation("Jump_Start_Left", "x_Jump_Left.png", 0, 7, 0.05f, false);
	Renderer->CreateAnimation("Jumping_Right", "x_Jump_Right.png", 7, 7, 0.1f, false);
	Renderer->CreateAnimation("Jumping_Left", "x_Jump_Left.png", 7, 7, 0.1f, false);
	Renderer->CreateAnimation("JumpEnd_Right", "x_Jump_Right.png", 8, 10, 0.005f, false);
	Renderer->CreateAnimation("JumpEnd_Left", "x_Jump_Left.png", 8, 10, 0.005f, false);

	// Attack
	Renderer->CreateAnimation("Idle_Attack_Right", "x_Idle_Attack_Right.png", 0, 5, 0.05f, false);
	Renderer->CreateAnimation("Idle_Attack_Left", "x_Idle_Attack_Left.png", 0, 5, 0.05f, false);
	Renderer->CreateAnimation("Idle_Attack_Wait_Right", "x_Idle_Attack_Right.png", 5, 5, 0.1f, false);
	Renderer->CreateAnimation("Idle_Attack_Wait_Left", "x_Idle_Attack_Left.png", 5, 5, 0.1f, false);
	Renderer->CreateAnimation("Idle_Attack_End_Right", "x_Idle_Attack_Right.png", 6, 7, 0.1f, false);
	Renderer->CreateAnimation("Idle_Attack_End_Left", "x_Idle_Attack_Left.png", 6, 7, 0.1f, false);

	// Dash
	Renderer->CreateAnimation("Dash_Start_Right", "x_Dash_Right.png", 0, 1, 0.05f, false);
	Renderer->CreateAnimation("Dash_Start_Left", "x_Dash_Left.png", 0, 1, 0.05f, false);
	Renderer->CreateAnimation("Dash_Loop_Right", "x_Dash_Right.png", 2, 3, 0.05f, true);
	Renderer->CreateAnimation("Dash_Loop_Left", "x_Dash_Left.png", 2, 3, 0.05f, true);
	Renderer->CreateAnimation("Dash_End_Right", "x_Dash_Right.png", 4, 7, 0.05f, false);
	Renderer->CreateAnimation("Dash_End_Left", "x_Dash_Left.png", 4, 7, 0.05f, false);

	Renderer->ChangeAnimation("Summon");

	StateChange(EPlayerState::Summon);
}

void APlayer::ChargeBeginPlay()
{
	ChargeRender_1 = CreateImageRenderer(static_cast<int>(ERenderOrder::Player));
	ChargeRender_1->SetImage("Charging_1.png");
	UWindowImage* ChargeImage_1 = ChargeRender_1->GetImage();
	FVector ChargeScale_1 = ChargeImage_1->GetScale();
	ChargeRender_1->SetTransform({ { 0, 0 }, {ChargeScale_1.iX() / 9, ChargeScale_1.iY()}});
	

	ChargeRender_2 = CreateImageRenderer(static_cast<int>(ERenderOrder::Player));
	ChargeRender_2->SetImage("Charging_2.png");
	UWindowImage* ChargeImage_2 = ChargeRender_2->GetImage();
	FVector ChargeScale_2 = ChargeImage_2->GetScale();
	ChargeRender_2->SetTransform({ { 0, 0 }, {ChargeScale_2.iX() / 4 , ChargeScale_2.iY()}});

	ChargeRender_1->CreateAnimation("Charge_1", "Charging_1.png", 0, 8, 0.05f, true);
	ChargeRender_2->CreateAnimation("Charge_2", "Charging_2.png", 0, 3, 0.05f, true);
	
	ChargeRender_1->ChangeAnimation("Charge_1");
	ChargeRender_2->ChangeAnimation("Charge_2");

	ChargeRender_1->ActiveOff();
	ChargeRender_2->ActiveOff();
}

void APlayer::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	StateUpdate(_DeltaTime);
}

void APlayer::DirCheck()
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
	if(Dir != DirState)
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
		Renderer->ChangeAnimation(Name, true, Renderer->GetCurAnimationFrame(), Renderer->GetCurAnimationTime());
	}
}

std::string APlayer::GetAnimationName(std::string _Name)
{
	std::string DirName = "";

	switch (DirState)
	{
	case EActorDir::Left :
		DirName = "_Left";
		break;
	case EActorDir::Right :
		DirName = "_Right";
		break;
	default :
		break;
	}

	// 상태를 받는다. Ex) Idle
	CurAnimationName = _Name;

	// Ex) Idle_Left
	return _Name + DirName;
}

void APlayer::StateChange(EPlayerState _State)
{
	// 현재 상태가 같지 않다 = 상태가 변했다.
	if (State != _State)
	{
		switch (_State)
		{
		case EPlayerState::Summon:
			SummonStart();
			break;
		case EPlayerState::SummonLoop:
			SummonLoopStart();
			break;
		case EPlayerState::SummonEnd:
			SummonEndStart();
			break;
		case EPlayerState::Idle :
			IdleStart();
			break;
		case EPlayerState::Run :
			RunStart();
			break;
		case EPlayerState::Jump :
			JumpStart();
			break;
		case EPlayerState::Sky :
			SkyStart();
			break;
		case EPlayerState::JumpEnd :
			JumpEndStart();
			break;
		case EPlayerState::IdleAttack:
			IdleAttackStart();
			break;
		case EPlayerState::IdleAttackWait:
			IdleAttackWaitStart();
			break;
		case EPlayerState::IdleAttackEnd:
			IdleAttackEndStart();
			break;
		case EPlayerState::DashStart:
			DashStart();
			break;
		case EPlayerState::DashLoop:
			DashLoopStart();
			break;
		case EPlayerState::DashEnd:
			DashEndStart();
			break;
		case EPlayerState::RunAndAttack:
			RunAndAttackStart();
			break;
		default :
			break;
		}
	}

	State = _State;
}

void APlayer::StateUpdate(float _DeltaTime) // Tick
{
	switch (State)
	{
	case EPlayerState::Summon:
		Summon(_DeltaTime);
		break;
	case EPlayerState::SummonLoop:
		SummonLoop(_DeltaTime);
		break;
	case EPlayerState::SummonEnd:
		SummonEnd(_DeltaTime);
		break;
	case EPlayerState::Idle :
		Idle(_DeltaTime);
		break;
	case EPlayerState::Run :
		Run(_DeltaTime);
		break;
	case EPlayerState::Jump :
		Jump(_DeltaTime);
		break;
	case EPlayerState::Sky :
		Sky(_DeltaTime);
		break;
	case EPlayerState::JumpEnd :
		JumpEnd(_DeltaTime);
		break;
	case EPlayerState::IdleAttack :
		IdleAttack(_DeltaTime);
		break;
	case EPlayerState::IdleAttackWait :
		IdleAttackWait(_DeltaTime);
		break;
	case EPlayerState::IdleAttackEnd :
		IdleAttackEnd(_DeltaTime);
		break;
	case EPlayerState::DashStart:
		Dash(_DeltaTime);
		break;
	case EPlayerState::DashLoop:
		DashLoop(_DeltaTime);
		break;
	case EPlayerState::DashEnd:
		DashEnd(_DeltaTime);
		break;
	case EPlayerState::RunAndAttack:
		RunAndAttack(_DeltaTime);
		break;
	default :
		break;
	}
}

// === 상태 시작 함수 ===

#pragma region 상태 시작 함수들. [...Start()]

void APlayer::SummonStart()
{
	Renderer->ChangeAnimation("Summon");
}

void APlayer::SummonLoopStart()
{
	Renderer->ChangeAnimation("Summon_Loop");
}

void APlayer::SummonEndStart()
{
	Renderer->ChangeAnimation(GetAnimationName("Idle"));
}

void APlayer::IdleStart()
{
	Renderer->ChangeAnimation(GetAnimationName("Idle"));
	DirCheck();
}

void APlayer::RunStart()
{
	Renderer->ChangeAnimation(GetAnimationName("Run"));
	DirCheck();
}

void APlayer::JumpStart()
{
	JumpVector = JumpPower;
	Renderer->ChangeAnimation(GetAnimationName("Jump_Start"));
	DirCheck();
}

void APlayer::SkyStart()
{
	Renderer->ChangeAnimation(GetAnimationName("Jumping"));
	DirCheck();
}

void APlayer::JumpEndStart()
{
	Renderer->ChangeAnimation(GetAnimationName("JumpEnd"));
}

void APlayer::IdleAttackStart()
{
	Renderer->ChangeAnimation(GetAnimationName("Idle_Attack"));
}

void APlayer::IdleAttackWaitStart()
{
	Renderer->ChangeAnimation(GetAnimationName("Idle_Attack_Wait"));
}

void APlayer::IdleAttackEndStart()
{
	Renderer->ChangeAnimation(GetAnimationName("Idle_Attack_End"));
}
void APlayer::DashStart()
{
	Renderer->ChangeAnimation(GetAnimationName("Dash_Start"));
}
void APlayer::DashLoopStart()
{
	Renderer->ChangeAnimation(GetAnimationName("Dash_Loop"));
}
void APlayer::DashEndStart()
{
	Renderer->ChangeAnimation(GetAnimationName("Dash_End"));
}

void APlayer::RunAndAttackStart()
{
	Renderer->ChangeAnimation(GetAnimationName("Run_And_Attack"));
	DirCheck();
}

#pragma endregion

// ==== 상태 함수 ====

#pragma region Summon Funcion
void APlayer::Summon(float _DeltaTime)
{
	// Actor의 기준점 가져오기.
	FVector CheckPos = this->GetActorLocation();
	Color8Bit Color = UContentsGlobalData::ColMapImage->GetColor(CheckPos.iX(), CheckPos.iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
	{
		StateChange(EPlayerState::SummonLoop);
		return;
	}
	
	AddActorLocation(FVector::Down * _DeltaTime * 500.0f);
}

void APlayer::SummonLoop(float _DeltaTime)
{
	DelayTime += _DeltaTime;
	if (true == Renderer->IsCurAnimationEnd())
	{
		if (1.5f < DelayTime)
		{
			StateChange(EPlayerState::SummonEnd);
			DelayTime = 0.0f;
			return;
		}
	}
}

void APlayer::SummonEnd(float _DeltaTime)
{
	StateChange(EPlayerState::Idle);
	return;
}
#pragma endregion

void APlayer::Idle(float _DeltaTime)
{
	// 가만히 있는데 뱡향 키가 눌렸을 때.
	if (true == UEngineInput::IsPress(VK_LEFT) ||
		true == UEngineInput::IsPress(VK_RIGHT))
	{
		StateChange(EPlayerState::Run);
		return;
	}
	//if (true == UEngineInput::IsUp('Z') || true == UEngineInput::IsFree('Z'))
	//{
	//	IsDash = false;
	//}

	// Jump
	if (true == UEngineInput::IsDown('C'))
	{
		StateChange(EPlayerState::Jump);
		return;
	}

	// Idle Attack
	if (true == UEngineInput::IsDown('X'))
	{
		StateChange(EPlayerState::IdleAttack);
		return;
	}

	// Dash
	//if (true == UEngineInput::IsDown('Z') && IsDash == false)
	if (true == UEngineInput::IsDown('Z'))
	{
		//IsDash = true;
		StateChange(EPlayerState::DashStart);
		return;
	}

	MoveUpdate(_DeltaTime);
}

void APlayer::Run(float _DeltaTime)
{
	// 움직이는데 어느 방향인지 모른다.
	DirCheck();

	// 움직임을 멈췄을 때,
	if (true == UEngineInput::IsFree(VK_LEFT) &&
		true == UEngineInput::IsFree(VK_RIGHT) &&
		true == UEngineInput::IsFree(VK_UP) &&
		true == UEngineInput::IsFree(VK_DOWN))
	{
		StateChange(EPlayerState::Idle);
		return;
	}

	// 키보드 입력
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		AddMoveVector(FVector::Left * MoveSpeed);
	}

	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		AddMoveVector(FVector::Right * MoveSpeed);
	}

	

	if (true == UEngineInput::IsDown('Z'))
	{
		StateChange(EPlayerState::DashStart);
		return;
	}

	// Jump
	if (true == UEngineInput::IsDown('C'))
	{
		StateChange(EPlayerState::Jump);
		return;
	}

	// Attack
	if (true == UEngineInput::IsDown('X'))
	{
		StateChange(EPlayerState::RunAndAttack);
		return;
	}

	MoveUpdate(_DeltaTime);
}

#pragma region Jump Function

void APlayer::Jump(float _DeltaTime)
{
	DirCheck();

	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		AddMoveVector(FVector::Left * MoveSpeed);
	}
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		AddMoveVector(FVector::Right * MoveSpeed);
	}
	// 공격 추가 예정

	MoveUpdate(_DeltaTime);

	if (true == Renderer->IsCurAnimationEnd())
	{
		StateChange(EPlayerState::Sky);
		return;
	}
}

void APlayer::Sky(float _DeltaTime)
{
	DirCheck();

	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		AddMoveVector(FVector::Left * MoveSpeed);
	}
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		AddMoveVector(FVector::Right * MoveSpeed);
	}
	// 공격 추가 예정


	MoveUpdate(_DeltaTime);

	Color8Bit Color = UContentsGlobalData::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
	{
		JumpVector = FVector::Zero;
		StateChange(EPlayerState::JumpEnd);
		return;
	}
}

void APlayer::JumpEnd(float _DeltaTime)
{
	DashVector = float4::Zero;
	if (true == Renderer->IsCurAnimationEnd())
	{
		StateChange(EPlayerState::Idle);
		return;
	}
}

#pragma endregion


#pragma region Attack Function

void APlayer::IdleAttack(float _DeltaTime)
{
	/*
	* 1. Buster 발싸.
	* 2. Buster 는 연속 생성 3개.
	* 3. float x = UEngineInput::GetPressTime('Z'); 여부에 따라 
	*/


	// Buster Actor 생성. -> Actor 자체는 그냥 Player가 만들어 질 때 만들기?
	
	//NewBuster = GetWorld()->SpawnActor<ABuster>();
	//NewBuster->SetActorLocation(GetActorLocation());

	//if (DirState == EActorDir::Right)
	//{
	//	NewBuster->SetDir(FVector::Right);
	//}
	//else
	//{
	//	NewBuster->SetDir(FVector::Left);
	//}
	//NewBuster->Destroy(2.0f); // test

	if (true == Renderer->IsCurAnimationEnd())
	{
		StateChange(EPlayerState::IdleAttackWait);
		return;
	}
}

void APlayer::IdleAttackWait(float _DeltaTime)
{
	AttackTime += UEngineInput::GetPressTime('X');

	if (true == UEngineInput::IsDown('X'))
	{
		StateChange(EPlayerState::IdleAttack);
		return;
	}
	else
	{
		//딜레이가 좀 있어야 함.
		StateChange(EPlayerState::IdleAttackEnd);
		return;
	}
}

void APlayer::IdleAttackEnd(float _DeltaTime)
{
	// Wait의 일정 시간이 지난 후...
	if (true == Renderer->IsCurAnimationEnd())
	{
		StateChange(EPlayerState::Idle);
		return;
	}
}
#pragma endregion


#pragma region Dash Function

void APlayer::Dash(float _DeltaTime)
{
	if (true == UEngineInput::IsFree('Z'))
	{
		StateChange(EPlayerState::DashEnd);
		return;
	}
	
	// Down 처리는 했는데 Press 상태이면 넘김.
	if (true == UEngineInput::IsPress('Z'))
	{
		DashTime = 0.0f;
		StateChange(EPlayerState::DashLoop);
		return;
	}
}

void APlayer::DashLoop(float _DeltaTime)
{
	DashTime += UEngineInput::GetPressTime('Z');

	// Idle to Dash
	if (true == UEngineInput::IsFree(VK_RIGHT) || true == UEngineInput::IsFree(VK_LEFT))
	{
		if (DirState == EActorDir::Right)
		{
			DashVector = FVector::Right * MoveSpeed * DashSpeed;
		}
		else if (DirState == EActorDir::Left)
		{
			DashVector = FVector::Left * MoveSpeed * DashSpeed;
		}
	}


	// Run to Dash
	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		DashVector = FVector::Right * MoveSpeed * DashSpeed;
	}
	else if (true == UEngineInput::IsPress(VK_LEFT))
	{
		DashVector = FVector::Left * MoveSpeed * DashSpeed;
	}

	MoveUpdate(_DeltaTime);

	// 점프
	if (true == UEngineInput::IsDown('C'))
	{
		StateChange(EPlayerState::Jump);
		return;
	}

	// 어택.






	if (0.5f <= DashTime || true == UEngineInput::IsFree('Z') || true == UEngineInput::IsUp('Z'))
	{
		DashTime = 0.0f;
		StateChange(EPlayerState::DashEnd);
		return;
	}
}

void APlayer::DashEnd(float _DeltaTime)
{
	DashTime = 0.0f;
	DashVector = float4::Zero;
	StateChange(EPlayerState::Idle);
	return;
}
#pragma endregion

void APlayer::RunAndAttack(float _DeltaTime)
{
	DirCheck();

	// 키보드 입력
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		AddMoveVector(FVector::Left * MoveSpeed);
	}

	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		AddMoveVector(FVector::Right * MoveSpeed);
	}

	// 방향키를 땠을 때,
	if (true == UEngineInput::IsFree(VK_LEFT) &&
		true == UEngineInput::IsFree(VK_RIGHT) &&
		true == UEngineInput::IsFree(VK_UP) &&
		true == UEngineInput::IsFree(VK_DOWN))
	{
		StateChange(EPlayerState::IdleAttack);
		return;
	}
	
	// 추가적인 x키 입력이 없을 때,

}


// ==== Vector ====

void APlayer::AddMoveVector(const FVector& _DirDelta)
{
	RunVector = _DirDelta;
}

void APlayer::CalLastMoveVector()
{
	LastMoveVector = FVector::Zero;
	LastMoveVector = LastMoveVector + RunVector;
	LastMoveVector = LastMoveVector + JumpVector;
	LastMoveVector = LastMoveVector + GravityVector;
	LastMoveVector = LastMoveVector + DashVector;
	LastMoveVector + JumpVector;
}

void APlayer::CalMoveVector()
{
	// Actor의 기준점 가져오기.
	FVector CheckPos = GetActorLocation();

	// 충돌을 체크하기 위해
	// 방향키에 맞춰 Actor의 기준점을 옮긴다. 
	switch (DirState)
	{
	case EActorDir::Left :
		CheckPos.X -= 100;
		break;
	case EActorDir::Right :
		CheckPos.X += 40;
		break;
	default :
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

void APlayer::CalGravityVector(float _DeltaTime)
{
	GravityVector += GravityAcc * _DeltaTime;

	Color8Bit Color = UContentsGlobalData::ColMapImage->GetColor(GetActorLocation().iX(), GetActorLocation().iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
	{
		GravityVector = FVector::Zero;
	}
}

void APlayer::MoveLastMoveVector(float _DeltaTime)
{
	// 카메라 추가.
	MoveCameraVector();	

	AActor::AddActorLocation(LastMoveVector * _DeltaTime);
}

void APlayer::MoveUpdate(float _DeltaTime)
{
	CalMoveVector();
	CalGravityVector(_DeltaTime);
	CalLastMoveVector();
	MoveLastMoveVector(_DeltaTime);
}

void APlayer::MoveCameraVector()
{
	FVector PlayerPos = this->GetActorLocation();
	FVector CameraPos = GetWorld()->GetCameraPos();
	
	CameraPos.X = PlayerPos.X - 400.0f;
	CameraPos.Y = PlayerPos.Y - 484.0f;

	if (0.0f >= CameraPos.X)
	{
		CameraPos.X = 0.0f;
	}

	if (0.0f >= CameraPos.Y)
	{
		CameraPos.Y = 0.0f;
	}
	
	GetWorld()->SetCameraPos(CameraPos);
}


// EngineDebug::OutPutDebugText("" + std::to_string());
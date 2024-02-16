#include "Player.h"
#include <EngineCore/EngineCore.h> // Helper

APlayer::APlayer()
{
}

APlayer::~APlayer()
{
}

void APlayer::BeginPlay()
{
	AActor::BeginPlay();

	Renderer = CreateImageRenderer(5);
	Renderer->SetImage("x_Idle_Right.png");
	UWindowImage* Image = Renderer->GetImage();
	FVector ImageScale = Image->GetScale();// 200 100
	Renderer->SetTransform({ {0,0}, {35 * 2, 80 * 2} });

	// Idle
	Renderer->CreateAnimation("Idle_Right", "x_Idle_Right.png", { 0,1,2,3,4,3,2,1 }, 0.1f, true);
	Renderer->CreateAnimation("Idle_Left", "x_Idle_Left.png", { 0,1,2,3,4,3,2,1 }, 0.1f, true);

	// Run
	Renderer->CreateAnimation("Run_Right", "x_Move_Right.png", 2, 15, 0.05f, true);
	Renderer->CreateAnimation("Run_Left", "x_Move_Left.png", 2, 15, 0.05f, true);

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


	Renderer->ChangeAnimation("Idle_Right");

	StateChange(EPlayerState::Idle);
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
	default :
		break;
	}
}

// === 상태 시작 함수 ===

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

// ==== 상태 함수 ====

void APlayer::Idle(float _DeltaTime)
{
	// 가만히 있는데 뱡향 키가 눌렸을 때.
	if (true == UEngineInput::IsPress(VK_LEFT) ||
		true == UEngineInput::IsPress(VK_RIGHT))
	{
		StateChange(EPlayerState::Run);
		return;
	}

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
	else if (true == UEngineInput::IsPress('X'))
	{
		StateChange(EPlayerState::IdleAttackWait);
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

	// Run 중에 Jump
	if (true == UEngineInput::IsDown('C'))
	{
		StateChange(EPlayerState::Jump);
		return;
	}
	// 공격 추가 예정

	MoveUpdate(_DeltaTime);
}

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
	if (true == Renderer->IsCurAnimationEnd())
	{
		StateChange(EPlayerState::Idle);
		return;
	}
}

void APlayer::IdleAttack(float _DeltaTime)
{
	
	// Buster Actor 생성.
	/* 예시.
		ABullet* NewBullet = GetWorld()->SpawnActor<ABullet>();
		NewBullet->SetActorLocation(GetActorLocation());
		NewBullet->SetDir(FVector::Right);
	*/


	if (true == Renderer->IsCurAnimationEnd())
	{
		StateChange(EPlayerState::IdleAttackWait);
		return;
	}
}

void APlayer::IdleAttackWait(float _DeltaTime)
{
	
	if (true == UEngineInput::IsPress('X'))
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
	if (true == Renderer->IsCurAnimationEnd())
	{
		StateChange(EPlayerState::Idle);
		return;
	}
}

void APlayer::AddMoveVector(const FVector& _DirDelta)
{
	MoveVector = _DirDelta;
}

void APlayer::CalLastMoveVector()
{
	LastMoveVector = FVector::Zero;
	LastMoveVector = LastMoveVector + MoveVector;
	LastMoveVector = LastMoveVector + JumpVector;
	LastMoveVector = LastMoveVector + GravityVector;
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
		CheckPos.X -= 30;
		break;
	case EActorDir::Right :
		CheckPos.X += 30;
		break;
	default :
		break;
	}
	CheckPos.Y -= 10;

	// 위의 CheckPos를 사용해서 Map의 충돌 체크를 한다.
	Color8Bit Color = UContentsGlobalData::ColMapImage->GetColor(CheckPos.iX(), CheckPos.iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
	{
		MoveVector = FVector::Zero;
	}


	// 키보드 입력이 없으면 속도를 Zero로.
	if (true == UEngineInput::IsFree(VK_LEFT) && true == UEngineInput::IsFree(VK_RIGHT) &&
		true == UEngineInput::IsFree(VK_UP) && true == UEngineInput::IsFree(VK_DOWN))
	{
		MoveVector = float4::Zero;
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
	GetWorld()->AddCameraPos(MoveVector * _DeltaTime);

	AActor::AddActorLocation(LastMoveVector * _DeltaTime);
}

void APlayer::MoveUpdate(float _DeltaTime)
{
	CalMoveVector();
	CalGravityVector(_DeltaTime);
	CalLastMoveVector();
	MoveLastMoveVector(_DeltaTime);
}

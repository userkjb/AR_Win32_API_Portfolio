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

	Renderer = CreateImageRenderer(5);
	Renderer->SetImage("x_Idle_Right.png");
	UWindowImage* Image = Renderer->GetImage();
	FVector ImageScale = Image->GetScale();// 200 100
	Renderer->SetTransform({ {0,0}, {35 * 3, 80 * 3} });

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

	// Dash
	Renderer->CreateAnimation("Dash_Start_Right", "x_Dash_Right.png", 0, 1, 1.05f, false);
	Renderer->CreateAnimation("Dash_Start_Left", "x_Dash_Left.png", 0, 1, 1.05f, false);
	Renderer->CreateAnimation("Dash_Loop_Right", "x_Dash_Right.png", 2, 3, 1.05f, true);
	Renderer->CreateAnimation("Dash_Loop_Left", "x_Dash_Left.png", 2, 3, 0.05f, true);
	Renderer->CreateAnimation("Dash_End_Right", "x_Dash_Right.png", 4, 7, 0.05f, false);
	Renderer->CreateAnimation("Dash_End_Left", "x_Dash_Left.png", 4, 7, 1.05f, false);

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

	// ���� ����� �ٸ��ٸ�,
	if(Dir != DirState)
	{
		DirState = Dir;

		// Animation�� �ٲ���� �Ѵ�.
		// ���� �ٲ��ֱ� ���� GetAnimationName �Լ��� ����� �ְ� ����Ѵ�.
		std::string Name = GetAnimationName(CurAnimationName);
		
		// ChangeAnimation�� Animation ���߿� �ٲ�� ������ ���� Frame�� �˾ƾ� �Դ�.
		// GetCurAnimationFrame �Լ��� ����� ����Ѵ�.
		// ����� ������ Animation�� �ð��� �˾ƾ� �Ѵ�.
		// GetCurAnimationTime �Լ��� ����� ����Ѵ�.
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

	// ���¸� �޴´�. Ex) Idle
	CurAnimationName = _Name;

	// Ex) Idle_Left
	return _Name + DirName;
}

void APlayer::StateChange(EPlayerState _State)
{
	// ���� ���°� ���� �ʴ� = ���°� ���ߴ�.
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
		case EPlayerState::DashStart:
			DashStart();
			break;
		case EPlayerState::DashLoop:
			DashLoopStart();
			break;
		case EPlayerState::DashEnd:
			DashEndStart();
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
	case EPlayerState::DashStart:
		Dash(_DeltaTime);
		break;
	case EPlayerState::DashLoop:
		DashLoop(_DeltaTime);
		break;
	case EPlayerState::DashEnd:
		DashEnd(_DeltaTime);
		break;
	default :
		break;
	}
}

// === ���� ���� �Լ� ===

#pragma region ���� ���� �Լ���. [...Start()]

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

#pragma endregion

// ==== ���� �Լ� ====

void APlayer::Idle(float _DeltaTime)
{
	// ������ �ִµ� ���� Ű�� ������ ��.
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
	if (true == UEngineInput::IsPress('X'))
	{
		StateChange(EPlayerState::IdleAttack);
		return;
	}

	// Dash
	if (true == UEngineInput::IsPress('Z') && DashTime == 0.0f)
	{
		StateChange(EPlayerState::DashStart);
		return;
	}

	MoveUpdate(_DeltaTime);
}

void APlayer::Run(float _DeltaTime)
{
	// �����̴µ� ��� �������� �𸥴�.
	DirCheck();

	// �������� ������ ��,
	if (true == UEngineInput::IsFree(VK_LEFT) &&
		true == UEngineInput::IsFree(VK_RIGHT) &&
		true == UEngineInput::IsFree(VK_UP) &&
		true == UEngineInput::IsFree(VK_DOWN))
	{
		StateChange(EPlayerState::Idle);
		return;
	}

	// Ű���� �Է�
	if (true == UEngineInput::IsPress(VK_LEFT))
	{
		AddMoveVector(FVector::Left * MoveSpeed);
	}

	if (true == UEngineInput::IsPress(VK_RIGHT))
	{
		AddMoveVector(FVector::Right * MoveSpeed);
	}

	// Run �߿� Jump
	if (true == UEngineInput::IsDown('C'))
	{
		StateChange(EPlayerState::Jump);
		return;
	}

	// ���� �߰� ����	
	//if (true == UEngineInput::IsPress('X'))
	//{
	//	StateChange(EPlayerState::AttackStart);
	//}

	// �뽬
	//if (true == UEngineInput::IsPress('Z'))
	//{
	//	StateChange(EPlayerState::DashStart);
	//}

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
	// ���� �߰� ����

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
	// ���� �߰� ����


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

#pragma endregion


#pragma region Attack Function

void APlayer::IdleAttack(float _DeltaTime)
{
	/*
	* 1. Buster �߽�.
	* 2. Buster �� ���� ���� 3��.
	* 3. float x = UEngineInput::GetPressTime('Z'); ���ο� ���� 
	*/


	// Buster Actor ����. -> Actor ��ü�� �׳� Player�� ����� �� �� �����?
	
	//ABuster* NewBuster = GetWorld()->SpawnActor<ABuster>();
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
	
	if (true == UEngineInput::IsDown('X'))
	{
		StateChange(EPlayerState::IdleAttack);
		return;
	}
	else
	{
		//�����̰� �� �־�� ��.
		StateChange(EPlayerState::IdleAttackEnd);
		return;
	}
}

void APlayer::IdleAttackEnd(float _DeltaTime)
{
	// Wait�� ���� �ð��� ���� ��...
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
	if (DirState == EActorDir::Right)
	{
		AddMoveVector(FVector::Right * MoveSpeed * DashSpeed);

		// �ݴ� ����Ű ����. TODO
	}
	if (DirState == EActorDir::Left)
	{
		AddMoveVector(FVector::Left * MoveSpeed * DashSpeed);

		// �ݴ� ����Ű ����. TODO
	}

	if (true == UEngineInput::IsFree('Z'))
	{
		StateChange(EPlayerState::DashEnd);
		return;
	}
	DashTime += UEngineInput::GetPressTime('Z');

	// ����

	// ����


	MoveUpdate(_DeltaTime);
	
	// [TODO]
	if (0.05f <= DashTime)
	{
		StateChange(EPlayerState::DashEnd);
		return;
	}
	if (0.1f <= DashTime)
	{
		StateChange(EPlayerState::DashLoop);
		return;
	}
}

void APlayer::DashLoop(float _DeltaTime)
{
	DashTime += UEngineInput::GetPressTime('Z');

	// ����.

	// ����.

	if (DashTime >= 1.0f || true == UEngineInput::IsFree('Z'))
	{
		StateChange(EPlayerState::DashEnd);
		return;
	}


	MoveUpdate(_DeltaTime);
}

void APlayer::DashEnd(float _DeltaTime)
{
	int a = 0;
	if (true == UEngineInput::IsFree('Z'))
	{
		DashTime = 0.0f;
		StateChange(EPlayerState::Idle);
		return;
	}

	MoveUpdate(_DeltaTime);
}
#pragma endregion


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
	LastMoveVector + JumpVector;
}

void APlayer::CalMoveVector()
{
	// Actor�� ������ ��������.
	FVector CheckPos = GetActorLocation();

	// �浹�� üũ�ϱ� ����
	// ����Ű�� ���� Actor�� �������� �ű��. 
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

	// ���� CheckPos�� ����ؼ� Map�� �浹 üũ�� �Ѵ�.
	Color8Bit Color = UContentsGlobalData::ColMapImage->GetColor(CheckPos.iX(), CheckPos.iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
	{
		RunVector = FVector::Zero;
	}


	// Ű���� �Է��� ������ �ӵ��� Zero��.
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
	// ī�޶� �߰�.
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
	FVector PlayerPosition = this->GetActorLocation();
	CameraVector.X = PlayerPosition.X - 100.0f;
	CameraVector.Y = PlayerPosition.Y - 484.0f;

	GetWorld()->SetCameraPos(CameraVector);
}


// EngineDebug::OutPutDebugText("" + std::to_string());
// float x = UEngineInput::GetPressTime('Z');
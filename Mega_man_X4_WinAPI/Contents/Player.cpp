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
	default :
		break;
	}
}

// === ���� ���� �Լ� ===

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

}

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

	MoveUpdate(_DeltaTime);
}

void APlayer::Jump(float _DeltaTime)
{
}

void APlayer::AddMoveVector(const FVector& _DirDelta)
{
	MoveVector = _DirDelta;
}

void APlayer::CalLastMoveVector()
{
	LastMoveVector = FVector::Zero;
	LastMoveVector = LastMoveVector + MoveVector;
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
		CheckPos.X -= 30;
		break;
	case EActorDir::Right :
		CheckPos.X += 30;
		break;
	default :
		break;
	}

	// 
	if (true == UEngineInput::IsFree(VK_LEFT) &&
		true == UEngineInput::IsFree(VK_RIGHT) &&
		true == UEngineInput::IsFree(VK_UP) &&
		true == UEngineInput::IsFree(VK_DOWN))
	{
		MoveVector = float4::Zero;
	}
}

void APlayer::MoveLastMoveVector(float _DeltaTime)
{
	// ī�޶� �߰�.
	GetWorld()->AddCameraPos(LastMoveVector * _DeltaTime);

	AActor::AddActorLocation(LastMoveVector * _DeltaTime);
}

void APlayer::MoveUpdate(float _DeltaTime)
{
	CalMoveVector();
	CalLastMoveVector(); // ��� Vector ������ ��ģ��.
	MoveLastMoveVector(_DeltaTime); // ��ģ Vector ���� �̿��� 
}

#include "Player.h"
#include <EngineCore/EngineCore.h> // Helper
//#include <EnginePlatform/EngineInput.h>

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
	Renderer->SetTransform({ {0,0}, {35*2, 46*2} });

	Renderer->CreateAnimation("Idle_Right", "x_Idle_Right.png", { 0,1,2,3,4,3,2,1 }, 0.1f, true);
	Renderer->CreateAnimation("Idle_Left", "x_Idle_Left.png", { 0,1,2,3,4,3,2,1 }, 0.1f, true);

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
		default :
			break;
		}
	}

	State = _State;
}

void APlayer::StateUpdate(float _DeltaTime)
{
	switch (State)
	{
	case EPlayerState::Idle :
		Idle(_DeltaTime);
		break;
	case EPlayerState::Run:
		Run(_DeltaTime);
		break;
	case EPlayerState::Jump:
		Jump(_DeltaTime);
		break;
	default :
		break;
	}
}

// === 상태 시작 함수 ===

void APlayer::IdleStart()
{
	Renderer->ChangeAnimation("Idle_Right");
}

void APlayer::RunStart()
{

}

void APlayer::JumpStart()
{

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


}

void APlayer::Run(float _DeltaTime)
{
	// 움직이는데 어느 방향인지 모른다.

}

void APlayer::Jump(float _DeltaTime)
{
}

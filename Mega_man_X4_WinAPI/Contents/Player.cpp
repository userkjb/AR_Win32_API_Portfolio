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
}

void APlayer::Tick(float _DeltaTime)
{
	HDC WindowDC = GEngine->MainWindow.GetWindowDC();

	// Move
	if (UEngineInput::IsDown(37) == true)
	{
		AddActorLocation(FVector::Left);
	}

	if (UEngineInput::IsDown(39) == true)
	{
		AddActorLocation(FVector::Right);
	}

	if (UEngineInput::IsDown(38) == true)
	{
		AddActorLocation(FVector::Up);
	}

	if (UEngineInput::IsDown(40) == true)
	{
		AddActorLocation(FVector::Down);
	}

	// Action
	if (UEngineInput::IsDown('Z') == true)
	{
		
	}

	if (UEngineInput::IsDown('X') == true)
	{
		
	}

	if (UEngineInput::IsDown('C') == true)
	{
		
	}

	FTransform Trans = GetTransform();
	Rectangle(WindowDC, Trans.iLeft(), Trans.iTop(), Trans.iRight(), Trans.iBottom());
}

#include "Player.h"
#include <EngineCore/EngineCore.h>
#include <EnginePlatform/EngineInput.h>

Player::Player()
{
}

Player::~Player()
{
}

void Player::BeginPlay()
{
	SetActorLocation({ 100, 100 });
	SetActorScale({ 100, 100 });
}

void Player::Tick(float _DeltaTime)
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

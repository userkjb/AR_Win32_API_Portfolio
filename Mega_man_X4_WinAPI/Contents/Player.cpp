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
	//FVector Pos = GetActorLocation();

	if (EngineInput::IsDown('A') == true)
	{
		AddActorLocation(FVector::Left);
	}

	if (EngineInput::IsDown('D') == true)
	{
		AddActorLocation(FVector::Right);
	}

	if (EngineInput::IsDown('W') == true)
	{
		AddActorLocation(FVector::Up);
	}

	if (EngineInput::IsDown('S') == true)
	{
		AddActorLocation(FVector::Down);
	}


	//SetActorLocation(Pos);
	FTransform Trans = GetTransform();
	Rectangle(WindowDC, Trans.iLeft(), Trans.iTop(), Trans.iRight(), Trans.iBottom());
}

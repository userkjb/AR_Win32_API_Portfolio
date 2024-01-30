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

	if (EngineInput::IsDown(37) == true)
	{
		AddActorLocation(FVector::Left);
	}

	if (EngineInput::IsDown(39) == true)
	{
		AddActorLocation(FVector::Right);
	}

	if (EngineInput::IsDown(38) == true)
	{
		AddActorLocation(FVector::Up);
	}

	if (EngineInput::IsDown(40) == true)
	{
		AddActorLocation(FVector::Down);
	}


	//SetActorLocation(Pos);
	FTransform Trans = GetTransform();
	Rectangle(WindowDC, Trans.iLeft(), Trans.iTop(), Trans.iRight(), Trans.iBottom());
}

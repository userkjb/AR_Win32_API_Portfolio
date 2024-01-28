#include "Player.h"
#include <EngineCore/EngineCore.h>

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
	FVector Pos = GetActorLocation();

	SetActorLocation(Pos);
	FTransform Trans = GetTransform();
	Rectangle(WindowDC, Trans.iLeft(), Trans.iTop(), Trans.iRight(), Trans.iBottom());
}

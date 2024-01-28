#include "IntroLevel.h"
#include "Player.h"

UIntroLevel::UIntroLevel()
{
}

UIntroLevel::~UIntroLevel()
{
}

void UIntroLevel::BeginPlay()
{
	this->SpawnActor<Player>();
}

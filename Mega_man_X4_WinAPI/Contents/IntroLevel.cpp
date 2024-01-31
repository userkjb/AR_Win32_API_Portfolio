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
	// Player
	this->SpawnActor<Player>();
	// Enemy

	// Boss 2개.

	// if Zero -> 아이리스

	// 벽 Object

}

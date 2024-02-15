#include "IntroLevel.h"
#include "IntroStageMap.h"
#include "Player.h"

UIntroLevel::UIntroLevel()
{
}

UIntroLevel::~UIntroLevel()
{
}

void UIntroLevel::BeginPlay()
{
	// Actor
	IntroStageMap = SpawnActor<AIntroStageMap>();
	IntroStageMap->SetMapImage("IntroStage_1.png");
	IntroStageMap->SetColMapImage("IntroStage_Col_1.png");


	// Player
	APlayer* NewPlayer = SpawnActor<APlayer>();
	NewPlayer->SetActorLocation({ 200, 200 }); // test

	// Enemy

	// Boss 2개.

	// if Zero -> 아이리스

	// 벽 Object

}

void UIntroLevel::Tick(float _DeltaTime)
{
}

void UIntroLevel::LevelStart(ULevel* _Level)
{
}

void UIntroLevel::LevelEnd(ULevel* _Level)
{
}

#include "TitleLevel.h"
#include "BackGroundMap.h"
#include "EngineCore/EngineCore.h"


UTitleLevel::UTitleLevel()
{
}

UTitleLevel::~UTitleLevel()
{
}

void UTitleLevel::BeginPlay()
{
	ULevel::BeginPlay();

	// Back Ground Image Actor
	BackGroundActor = SpawnActor<ABackGroundMap>();
	BackGroundActor->SetMapImage("TitleScreen.png");
	BackGroundActor->SetGameStartTextImage("GameStartText.png");
	BackGroundActor->SetMenuAnimation("Menu.png");
}

void UTitleLevel::Tick(float _DeltaTime)
{
	ULevel::Tick(_DeltaTime);
}

void UTitleLevel::LevelStart(ULevel* _Level)
{
	ULevel::LevelStart(_Level);
	int a = 0;
}

void UTitleLevel::LevelEnd(ULevel* _Level)
{
	ULevel::LevelEnd(_Level);
}

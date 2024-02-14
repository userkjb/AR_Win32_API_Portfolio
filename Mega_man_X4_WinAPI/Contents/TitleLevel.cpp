#include "TitleLevel.h"
#include "BackGroundMap.h"


UTitleLevel::UTitleLevel()
{
}

UTitleLevel::~UTitleLevel()
{
}

void UTitleLevel::BeginPlay()
{
	ULevel::BeginPlay();

	// Back Ground Image
	ABackGroundMap* Map = SpawnActor<ABackGroundMap>();
	Map->SetMapImage("TitleScreen.png");
	//Map->SetMenuImage("Menu.png");
	Map->SetGameStartTextImage("GameStartText.png");
}

void UTitleLevel::Tick(float _DeltaTime)
{
	ULevel::Tick(_DeltaTime);
}

void UTitleLevel::LevelStart(ULevel* _Level)
{
	ULevel::LevelStart(_Level);
}

void UTitleLevel::LevelEnd(ULevel* _Level)
{
	ULevel::LevelEnd(_Level);
}

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

	ABackGroundMap* Map = SpawnActor<ABackGroundMap>();
	Map->SetMapImage("Stage1Map.png");
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

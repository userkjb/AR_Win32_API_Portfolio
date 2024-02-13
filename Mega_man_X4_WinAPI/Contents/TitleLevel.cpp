#include "TitleLevel.h"

UTitleLevel::UTitleLevel()
{
}

UTitleLevel::~UTitleLevel()
{
}

void UTitleLevel::BeginPlay()
{
	ULevel::BeginPlay();


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

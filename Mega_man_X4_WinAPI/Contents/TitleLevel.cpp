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
	//BackGroundActor->SetMenuImage("Menu.png");
	//BackGroundActor->SetGameStartTextImage("GameStartText.png");

}

void UTitleLevel::Tick(float _DeltaTime)
{
	ULevel::Tick(_DeltaTime);

	if (UEngineInput::IsDown('P'))
	{
		if (true == BackGroundActor->IsActive())
		{
			BackGroundActor->SetActive(false);
		}
	}
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

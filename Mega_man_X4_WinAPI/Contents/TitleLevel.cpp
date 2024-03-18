#include "TitleLevel.h"
#include "BackGroundMap.h"
#include "EngineCore/EngineCore.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>


UTitleLevel::UTitleLevel()
{
}

UTitleLevel::~UTitleLevel()
{
}

void UTitleLevel::BeginPlay()
{
	ULevel::BeginPlay();

	// Resource 폴더 경로 설정.
	UEngineDirectory NewDir;
	NewDir.MoveParent();
	NewDir.Move("ContentsResources\\TitleLevel");
	//NewDir.MoveToSearchChild("ContentsResources\\TitleLevel");

	std::list<UEngineFile> NewList = NewDir.AllFile({ ".png", ".bmp" }, true);

	for (UEngineFile& File : NewList)
	{
		UEngineResourcesManager::GetInst().LoadImg(File.GetFullPath());
	}

	UEngineResourcesManager::GetInst().CuttingImage("Menu.png", 3, 1);


	//===================================================================================

	// Back Ground Image Actor
	BackGroundActor = SpawnActor<ABackGroundMap>();
	BackGroundActor->SetMapImage("TitleScreen.png");
	BackGroundActor->SetGameStartTextImage("GameStartText.png");
	BackGroundActor->SetMenuAnimation("Menu.png");
	BackGroundActor->SetStageChange(EBackGroundMapState::Title);
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

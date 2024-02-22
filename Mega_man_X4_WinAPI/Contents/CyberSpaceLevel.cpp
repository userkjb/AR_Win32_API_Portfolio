#include "CyberSpaceLevel.h"
#include "CyberSpaceMap.h"
#include "Player.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>

UCyberSpaceLevel::UCyberSpaceLevel()
{
}

UCyberSpaceLevel::~UCyberSpaceLevel()
{
}

void UCyberSpaceLevel::BeginPlay()
{
	ULevel::BeginPlay();

	UEngineDirectory NewDir;
	NewDir.MoveParent();
	NewDir.Move("ContentsResources\\CyberSpaceLevel");

	std::list<UEngineFile> NewList = NewDir.AllFile({ ".png", ".bmp" }, true);

	for (UEngineFile& File : NewList)
	{
		UEngineResourcesManager::GetInst().LoadImg(File.GetFullPath());
	}


	// Map
	CyberSpaceMap = SpawnActor<ACyberSpaceMap>();
	CyberSpaceMap->SetMapImage("CyberPeacock-Area1-1.png");
	CyberSpaceMap->SetColMapImage("CyberPeacock-Area1-1Col.png");
}

void UCyberSpaceLevel::Tick(float _DeltaTime)
{
	ULevel::Tick(_DeltaTime);
}

void UCyberSpaceLevel::LevelStart(ULevel* _Level)
{
	ULevel::LevelStart(_Level);
}

void UCyberSpaceLevel::LevelEnd(ULevel* _Level)
{
	ULevel::LevelEnd(_Level);
}

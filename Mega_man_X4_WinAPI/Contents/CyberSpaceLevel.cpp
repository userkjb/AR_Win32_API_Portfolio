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
	CyberSpaceMap->SetActorLocation({ 0, 0 });
	CyberSpaceMap->SetMapImage("CyberPeacock-Area1-1.png");
	CyberSpaceMap->SetColMapImage("CyberPeacock-Area1-1Col.png");

	APlayer* NewPlayer = SpawnActor<APlayer>();
	NewPlayer->SetActorLocation({ 400, 0 }); // 400 이 센터.
}

void UCyberSpaceLevel::Tick(float _DeltaTime)
{
	ULevel::Tick(_DeltaTime);

	FVector Pos = GetCameraPos();

	if (0.0f >= Pos.X)
	{
		Pos.X = 0.0f;
	}
	if (0.0f >= Pos.Y)
	{
		Pos.Y = 0.0f;
	}

	FVector ImageScale = CyberSpaceMap->GetImageScale();

	if (Pos.X >= ImageScale.X - GEngine->MainWindow.GetWindowScale().X)
	{
		Pos.X = ImageScale.X - GEngine->MainWindow.GetWindowScale().X;
	}

	SetCameraPos(Pos);
}

void UCyberSpaceLevel::LevelStart(ULevel* _Level)
{
	// 리소스 로드.
	// 액터 생성.
	ULevel::LevelStart(_Level);
}

void UCyberSpaceLevel::LevelEnd(ULevel* _Level)
{
	// 리소스 삭제.
	// 액터 삭제.
	ULevel::LevelEnd(_Level);
}

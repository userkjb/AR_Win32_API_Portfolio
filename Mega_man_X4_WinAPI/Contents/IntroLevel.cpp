#include "IntroLevel.h"
#include "IntroStageMap.h"
#include "Player.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>

UIntroLevel::UIntroLevel()
{
}

UIntroLevel::~UIntroLevel()
{
}

void UIntroLevel::BeginPlay()
{
	ULevel::BeginPlay();

	// Resource 폴더 경로 설정.
	UEngineDirectory NewDir;
	NewDir.MoveParent();
	NewDir.Move("ContentsResources\\IntroLevel");

	std::list<UEngineFile> NewList = NewDir.AllFile({ ".png", ".bmp" }, true);

	for (UEngineFile& File : NewList)
	{
		UEngineResourcesManager::GetInst().LoadImg(File.GetFullPath());
	}

	NewDir.MoveParent();
	NewDir.Move("GlobalLevel");
	
	NewList = NewDir.AllFile({ ".png", ".bmp" }, true);

	for (UEngineFile& File : NewList)
	{
		UEngineResourcesManager::GetInst().LoadImg(File.GetFullPath());
	}


	UEngineResourcesManager::GetInst().CuttingImage("x_Idle_Right.png", 5, 1);
	UEngineResourcesManager::GetInst().CuttingImage("x_Idle_Left.png", 5, 1);
	UEngineResourcesManager::GetInst().CuttingImage("x_Move_Right.png", 16, 1);
	UEngineResourcesManager::GetInst().CuttingImage("x_Move_Left.png", 16, 1);
	UEngineResourcesManager::GetInst().CuttingImage("x_Jump_Right.png", 11, 1);
	UEngineResourcesManager::GetInst().CuttingImage("x_Jump_Left.png", 11, 1);
	UEngineResourcesManager::GetInst().CuttingImage("x_Idle_Attack_Right.png", 8, 1);
	UEngineResourcesManager::GetInst().CuttingImage("x_Idle_Attack_Left.png", 8, 1);
	UEngineResourcesManager::GetInst().CuttingImage("x_Dash_Right.png", 8, 1);
	UEngineResourcesManager::GetInst().CuttingImage("x_Dash_Left.png", 8, 1);
	UEngineResourcesManager::GetInst().CuttingImage("x_Move_Attack_Right.png", 8, 4);
	UEngineResourcesManager::GetInst().CuttingImage("x_Move_Attack_Left.png", 8, 4);

	UEngineResourcesManager::GetInst().CuttingImage("x_Buster_Default_Right.png", 5, 1);

	UEngineResourcesManager::GetInst().CuttingImage("Spikeball_Left.png", 7, 3);

	UEngineResourcesManager::GetInst().CuttingImage("Grenademan_Left.png", 8, 5);
	UEngineResourcesManager::GetInst().CuttingImage("Grenademan_Bullet.png", 4, 1);
	UEngineResourcesManager::GetInst().CuttingImage("Grenademan_Bomb.png", 1, 1);
	

	// == Actor ==

	// Map
	IntroStageMap = SpawnActor<AIntroStageMap>();
	IntroStageMap->SetMapImage("IntroStage_1.png");
	IntroStageMap->SetColMapImage("IntroStage_Col_1.png");


	// Player
	APlayer* NewPlayer = SpawnActor<APlayer>();
	NewPlayer->SetActorLocation({ 200, 0 });

	// Enemy

	// Boss 2개.

	// if Zero -> 아이리스

	// 벽 Object

}

void UIntroLevel::Tick(float _DeltaTime)
{
	ULevel::Tick(_DeltaTime);
}

void UIntroLevel::LevelStart(ULevel* _Level)
{
	ULevel::LevelStart(_Level);
}

void UIntroLevel::LevelEnd(ULevel* _Level)
{
	ULevel::LevelEnd(_Level);
}

#include "CyberSpaceLevel.h"
#include "CyberSpaceMap.h"
#include "Player.h"
#include "Egseu.h"
#include "CyberPeacock.h"
#include "TriScan.h"
#include "SpikeMarl.h"
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

	//NewDir.MoveToSearchChild("ContentsResources\\CyberSpaceLevel");

	std::list<UEngineFile> NewList = NewDir.AllFile({ ".png", ".bmp" }, true);

	for (UEngineFile& File : NewList)
	{
		UEngineResourcesManager::GetInst().LoadImg(File.GetFullPath());
	}
}

void UCyberSpaceLevel::Tick(float _DeltaTime)
{
	ULevel::Tick(_DeltaTime);

	MoveCameraVector();
	ActorGeneration();
}

void UCyberSpaceLevel::LevelStart(ULevel* _Level)
{
	// 리소스 로드.
	// 액터 생성.
	ULevel::LevelStart(_Level);

	UEngineResourcesManager::GetInst().CuttingImage("Peacock_Intro.png", 8, 4);
	UEngineResourcesManager::GetInst().CuttingImage("Fight_Ready_Right.png", 1, 1);
	UEngineResourcesManager::GetInst().CuttingImage("Fight_Ready_Left.png", 6, 1);
	UEngineResourcesManager::GetInst().CuttingImage("Disappear_Appear_Right.png", 3, 1);
	UEngineResourcesManager::GetInst().CuttingImage("Disappear_Appear_Left.png", 3, 1);

	// Enemy
	UEngineResourcesManager::GetInst().CuttingImage("Triangle.png", 10, 1);
	UEngineResourcesManager::GetInst().CuttingImage("TriScan_Fragments.png", 1, 1);

	UEngineResourcesManager::GetInst().CuttingImage("SpikeBall_Right.png", 7, 3);
	UEngineResourcesManager::GetInst().CuttingImage("SpikeBall_Left.png", 7, 3);
	UEngineResourcesManager::GetInst().CuttingImage("SummonEnemy_L.png", 11, 1);
	UEngineResourcesManager::GetInst().CuttingImage("SummonEnemy_R.png", 11, 1);
	UEngineResourcesManager::GetInst().CuttingImage("SummonEnemy_Dummy.png", 3, 1);
	UEngineResourcesManager::GetInst().CuttingImage("SummonEnemy_End.png", 8, 1);

	// Map
	CyberSpaceMap = SpawnActor<ACyberSpaceMap>();
	CyberSpaceMap->SetActorLocation({ 0, 0 });
	CyberSpaceMap->SetMapImage("CyberPeacock-Area1-1.png");
	CyberSpaceMap->SetColMapImage("CyberPeacock-Area1-1Col.png");

	// Actor
	//NewPlayer = SpawnActor<APlayer>();
	//NewPlayer->SetActorLocation({ 200, 0 }); // 400 이 센터.

	// Player
	NewX = SpawnActor<AEgseu>();
	NewX->SetActorLocation({ 200, 90 });

	// Boss
	//ACyberPeacock* NewBoss = SpawnActor<ACyberPeacock>();
	//NewBoss->SetActorLocation({ 520, 500 });

	// Enemy
	TryScan_1 = SpawnActor<ATriScan>();
	TryScan_1->SetActorLocation({ 1160 , 135 });
	TryScan_1->SetTriScanState(ETriScanState::Idle);

	TryScan_2 = SpawnActor<ATriScan>();
	TryScan_2->SetActorLocation({ 1160 , 200 });
	TryScan_2->SetTriScanState(ETriScanState::Idle);

	SpikeMarl = SpawnActor<ASpikeMarl>();
	SpikeMarl->SetActorLocation({ 520, 400 });
	SpikeMarl->SetSpikeMarlState(ESpikeMarlState::SummonStart);
}

void UCyberSpaceLevel::LevelEnd(ULevel* _Level)
{
	// 리소스 삭제.
	// 액터 삭제.
	ULevel::LevelEnd(_Level);
}

void UCyberSpaceLevel::MoveCameraVector()
{
	FVector CameraPos = GetCameraPos();
	FVector PlayerPos = NewX->GetActorLocation();
	FVector ImageScale = CyberSpaceMap->GetImageScale();
	FVector WindowScale = GEngine->MainWindow.GetWindowScale();

	CameraPos.X = PlayerPos.X - WindowScale.hX();
	CameraPos.Y = PlayerPos.Y - 564.0f;

	if (0.0f >= CameraPos.X)
	{
		CameraPos.X = 0.0f;
	}
	if (CameraPos.X >= ImageScale.X - WindowScale.X)
	{
		CameraPos.X = ImageScale.X - WindowScale.X;
	}

	if (0.0f >= CameraPos.Y)
	{
		CameraPos.Y = 0.0f;
	}

	SetCameraPos(CameraPos);
}

void UCyberSpaceLevel::ActorGeneration()
{
	FVector PlayerPos = NewX->GetActorLocation();


}


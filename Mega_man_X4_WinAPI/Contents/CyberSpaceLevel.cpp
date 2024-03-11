#include "CyberSpaceLevel.h"
#include "CyberSpaceMap.h"
#include "Player.h"
#include "Egseu.h"
#include "CyberPeacock.h"
#include "TriScan.h"
#include "SpikeMarl.h"
#include "MiruTorearu.h"
//#include "PlayerFocus.h"
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

	//TestCameraMove();
	MoveCameraVector();
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

	UEngineResourcesManager::GetInst().CuttingImage("ElecballAtRest.png", 5, 1);
	UEngineResourcesManager::GetInst().CuttingImage("Rotating Elecball.png", 11, 1);
	UEngineResourcesManager::GetInst().CuttingImage("MiruToraeru_Death_Effect.png", 7, 1);
	UEngineResourcesManager::GetInst().CuttingImage("MiruToraeru_Death.png", 7, 2);
	UEngineResourcesManager::GetInst().CuttingImage("GetPlayer.png", 6, 1);

	// Focus
	UEngineResourcesManager::GetInst().CuttingImage("Focus.png", 3, 3);
	UEngineResourcesManager::GetInst().CuttingImage("HurryUp.png", 3, 1);

	// Exit
	UEngineResourcesManager::GetInst().CuttingImage("Rank.png", 3, 1);
	UEngineResourcesManager::GetInst().CuttingImage("GoToDoor.png", 14, 1);

	// Map
	CyberSpaceMap = SpawnActor<ACyberSpaceMap>(static_cast<int>(EActorType::Map));
	CyberSpaceMap->SetActorLocation({ 0, 0 });
	CyberSpaceMap->SetMapImage("CyberPeacock-Area1-1.png");
	CyberSpaceMap->SetColMapImage("CyberPeacock-Area1-1Col.png");
	CyberSpaceMap->SetMapExitImage("CyberPeacock-Area-EndObject.png");
	CyberSpaceMap->SetRankRenderImage("Rank.png");
	CyberSpaceMap->SetExitAniImage("GoToDoor.png");

	// Actor
	//NewPlayer = SpawnActor<APlayer>();
	//NewPlayer->SetActorLocation({ 200, 0 }); // 400 이 센터.

	// Player
	NewX = SpawnActor<AEgseu>(static_cast<int>(EActorType::Player));
	NewX->SetActorLocation({ 200, 90 });
	CyberSpaceMap->SetPlayer(NewX);

	// Boss
	//ACyberPeacock* NewBoss = SpawnActor<ACyberPeacock>();
	//NewBoss->SetActorLocation({ 520, 500 });

	// Enemy
	//TryScan_1 = SpawnActor<ATriScan>();
	//TryScan_1->SetActorLocation({ 1160 , 135 });
	//TryScan_1->SetTriScanState(ETriScanState::Idle);
	TryScan_1 = SpawnActor<ATriScan>(static_cast<int>(EActorType::Enemy));
	TryScan_1->SetActorLocation({ 520 , 500 });
	TryScan_1->SetTriScanState(ETriScanState::Idle);

	//TryScan_2 = SpawnActor<ATriScan>();
	//TryScan_2->SetActorLocation({ 1160 , 200 });
	//TryScan_2->SetTriScanState(ETriScanState::Idle);

	//SpikeMarl = SpawnActor<ASpikeMarl>();
	//SpikeMarl->SetActorLocation({ 520, 400 });
	//SpikeMarl->SetSpikeMarlState(ESpikeMarlState::SummonStart);

	//MiruTorearu_1 = SpawnActor<AMiruTorearu>();
	//MiruTorearu_1->SetActorLocation({ 520, 400 });
	//MiruTorearu_1->SetMiruTorearuState(EMiruTorearuState::StopCreate);

	//MiruTorearu_2 = SpawnActor<AMiruTorearu>();
	//MiruTorearu_2->SetActorLocation({ 520, 400 });
	//MiruTorearu_2->SetMiruDir(EActorDir::Left);
	//MiruTorearu_2->SetMiruTorearuState(EMiruTorearuState::RunCreate);
} // LevelStart

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

void UCyberSpaceLevel::TestCameraMove()
{
	FVector CameraPos = GetCameraPos();
	if (true == UEngineInput::IsDown(VK_RIGHT))
	{
		CameraPos.X += 1.0f;
	}

	if (true == UEngineInput::IsDown(VK_LEFT))
	{
		CameraPos.X -= 1.0f;
	}

	if (true == UEngineInput::IsDown(VK_UP))
	{
		CameraPos.Y += 1.0f;
	}

	if (true == UEngineInput::IsDown(VK_DOWN))
	{
		CameraPos.Y -= 1.0f;
	}

	SetCameraPos(CameraPos);
}

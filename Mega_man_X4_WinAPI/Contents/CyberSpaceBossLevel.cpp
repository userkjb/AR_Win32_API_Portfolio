#include "CyberSpaceBossLevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>

#include "CyberSpaceBossMap.h"
#include "Egseu.h"
#include "CyberPeacock.h"

UCyberSpaceBossLevel::UCyberSpaceBossLevel()
{
}

UCyberSpaceBossLevel::~UCyberSpaceBossLevel()
{
}

void UCyberSpaceBossLevel::BeginPlay()
{
	ULevel::BeginPlay();

	// CyberSpaceLevel 에서 다 읽어 들임.
	//UEngineDirectory NewDir;
	//NewDir.MoveParent();
	//NewDir.Move("ContentsResources\\CyberSpaceLevel");
	//std::list<UEngineFile> NewList = NewDir.AllFile({ ".png", ".bmp" }, true);
	//for (UEngineFile& File : NewList)
	//{
	//	UEngineResourcesManager::GetInst().LoadImg(File.GetFullPath());
	//}

	StateChange(EBossLevelState::None);
}

void UCyberSpaceBossLevel::Tick(float _DeltaTime)
{
	ULevel::Tick(_DeltaTime);

	StateUpdate(_DeltaTime);
}

void UCyberSpaceBossLevel::LevelStart(ULevel* _Level)
{
	ULevel::LevelStart(_Level);

	// Boss Image
	UEngineResourcesManager::GetInst().CuttingImage("Peacock_Intro.png", 6, 6);
	UEngineResourcesManager::GetInst().CuttingImage("Fight_Ready_Right.png", 7, 1);
	UEngineResourcesManager::GetInst().CuttingImage("Fight_Ready_Left.png", 7, 1);
	UEngineResourcesManager::GetInst().CuttingImage("Disappear_Appear_Right.png", 4, 1);
	UEngineResourcesManager::GetInst().CuttingImage("Disappear_Appear_Left.png", 4, 1);
	UEngineResourcesManager::GetInst().CuttingImage("FeatherAttack_Right.png", 5, 3);
	UEngineResourcesManager::GetInst().CuttingImage("FeatherAttack_Left.png", 5, 3);
	UEngineResourcesManager::GetInst().CuttingImage("TrackingShot_Right.png", 5, 4);
	UEngineResourcesManager::GetInst().CuttingImage("TrackingShot_Left.png", 5, 4);
	UEngineResourcesManager::GetInst().CuttingImage("RisingSlash_Right.png", 3, 3);
	UEngineResourcesManager::GetInst().CuttingImage("RisingSlash_Left.png", 3, 3);
	UEngineResourcesManager::GetInst().CuttingImage("missile.png", 16, 2); // 이미지 이상함. TODO

	// Map Door
	UEngineResourcesManager::GetInst().CuttingImage("mmx4-bossdoor.png", 5, 3);

	// Setting
	// Map
	CyberBossMap = SpawnActor<ACyberSpaceBossMap>(static_cast<int>(EActorType::Map));
	CyberBossMap->SetActorLocation({ 0, 0 });
	CyberBossMap->SetMapImage("CyberSpace_Stage_BossMap.png");
	CyberBossMap->SetMapColImage("CyberSpace_Stage_BossMapCol.png");
	CyberBossMap->SetDoorImage("mmx4-bossdoor.png");

	// Player
	Player = SpawnActor<AEgseu>(static_cast<int>(EActorType::Player));
	Player->SetActorLocation({ 200, 90 });

	// Boss
	CyberBoss = SpawnActor<ACyberPeacock>(static_cast<int>(EActorType::Boss));
	CyberBoss->SetActorLocation({ 2400, 330 });
	CyberBoss->SetActive(false);
}

void UCyberSpaceBossLevel::LevelEnd(ULevel* _Level)
{
}

void UCyberSpaceBossLevel::StateChange(EBossLevelState _State)
{
	if (LevelState != _State)
	{
		switch (_State)
		{
		case EBossLevelState::None:
			//NoneStart();
			break;
		case EBossLevelState::Front_Door:
			Front_DoorStart();
			break;
		case EBossLevelState::CheckPointRoom:
			CheckPointRoomStart();
			break;
		case EBossLevelState::Back_Door:
			Back_DoorStart();
			break;
		case EBossLevelState::BossRoom:
			BossRoomStart();
			break;

		default :
			break;
		}
	}

	LevelState = _State;
}

void UCyberSpaceBossLevel::StateUpdate(float _DeltaTime)
{
	switch (LevelState)
	{
	case EBossLevelState::None:
		None(_DeltaTime);
		break;
	case EBossLevelState::Front_Door:
		break;
	case EBossLevelState::CheckPointRoom:
		break;
	case EBossLevelState::Back_Door:
		break;
	case EBossLevelState::BossRoom:
		break;
	default:
		break;
	}
}

#pragma region None
//void UCyberSpaceBossLevel::NoneStart()
//{}

void UCyberSpaceBossLevel::None(float _DeltaTime)
{
	
}
#pragma endregion

#pragma region str
void UCyberSpaceBossLevel::Front_DoorStart()
{
}

void UCyberSpaceBossLevel::Front_Door(float _DeltaTime)
{
}
#pragma endregion

#pragma region str
void UCyberSpaceBossLevel::CheckPointRoomStart()
{
}

void UCyberSpaceBossLevel::CheckPointRoom(float _DeltaTime)
{
}
#pragma endregion

#pragma region str
void UCyberSpaceBossLevel::Back_DoorStart()
{
}

void UCyberSpaceBossLevel::Back_Door(float _DeltaTime)
{
}
#pragma endregion

#pragma region str
void UCyberSpaceBossLevel::BossRoomStart()
{
}

void UCyberSpaceBossLevel::BossRoom(float _DeltaTime)
{
}
#pragma endregion

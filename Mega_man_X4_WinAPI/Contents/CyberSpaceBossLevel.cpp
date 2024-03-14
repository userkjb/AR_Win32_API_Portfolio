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
	if (true == b_BossBattle)
	{
		BossRoomCameraVector();
	}
}

void UCyberSpaceBossLevel::LevelStart(ULevel* _Level)
{
	ULevel::LevelStart(_Level);

	// Boss Image
	UEngineResourcesManager::GetInst().CuttingImage("Peacock_Intro.png", 6, 6);
	//UEngineResourcesManager::GetInst().CuttingImage("Fight_Ready_Right.png", 7, 1);
	UEngineResourcesManager::GetInst().CuttingImage("Fight_Ready_Left.png", 7, 1);
	UEngineResourcesManager::GetInst().CuttingImage("Disappear_Appear_Right.png", 4, 1);
	UEngineResourcesManager::GetInst().CuttingImage("Disappear_Appear_Left.png", 4, 1);
	UEngineResourcesManager::GetInst().CuttingImage("FeatherAttack_Right.png", 5, 3);
	UEngineResourcesManager::GetInst().CuttingImage("FeatherAttack_Left.png", 5, 3);
	UEngineResourcesManager::GetInst().CuttingImage("TrackingShot_Right.png", 5, 4);
	UEngineResourcesManager::GetInst().CuttingImage("TrackingShot_Left.png", 5, 4);
	UEngineResourcesManager::GetInst().CuttingImage("RisingSlash_Right.png", 3, 3);
	UEngineResourcesManager::GetInst().CuttingImage("RisingSlash_Left.png", 3, 3);
	UEngineResourcesManager::GetInst().CuttingImage("Scope.png", 14, 1);
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
	CyberBoss->SetStateChange(ECyberPeacockState::None);
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
		case EBossLevelState::BossIntro:
			BossIntroStart();
			break;
		case EBossLevelState::BossText:
			BossTextStart();
			break;
		case EBossLevelState::BossBattleReadySound:
			BossBattleReadySoundStart();
			break;
		case EBossLevelState::BossReady:
			BossReadyStart();
			break;
		case EBossLevelState::BossBattle:
			BossBattleStart();
			break;
		case EBossLevelState::BossEnd:
			BossEndStart();
			break;
		case EBossLevelState::PlayerReverseSummon:
			PlayerReverseSummonStart();
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
		Front_Door(_DeltaTime);
		break;
	case EBossLevelState::CheckPointRoom:
		CheckPointRoom(_DeltaTime);
		break;
	case EBossLevelState::Back_Door:
		Back_Door(_DeltaTime);
		break;
	case EBossLevelState::BossRoom:
		BossRoom(_DeltaTime);
		break;
	case EBossLevelState::BossIntro:
		BossIntro(_DeltaTime);
		break;
	case EBossLevelState::BossText:
		BossText(_DeltaTime);
		break;
	case EBossLevelState::BossBattleReadySound:
		BossBattleReadySound(_DeltaTime);
		break;
	case EBossLevelState::BossReady:
		BossReady(_DeltaTime);
		break;
	case EBossLevelState::BossBattle:
		BossBattle(_DeltaTime);
		break;
	case EBossLevelState::BossEnd:
		BossEnd(_DeltaTime);
		break;
	case EBossLevelState::PlayerReverseSummon:
		PlayerReverseSummon(_DeltaTime);
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
	MoveCameraVector();
	if (true == CyberBossMap->GetFrontDoor())
	{
		StateChange(EBossLevelState::Front_Door);
		return;
	}
}
#pragma endregion

#pragma region Front_Door
void UCyberSpaceBossLevel::Front_DoorStart()
{
	PlayerRunVector = FVector::Zero;
	Player->SetStateChange(EEgseuState::BossRoomAutoRun);
}

void UCyberSpaceBossLevel::Front_Door(float _DeltaTime)
{
	if (true == CyberBossMap->GetFrontDoorOpen())
	{
		PlayerRunVector = FVector::Right * PlayerRunSpeed * _DeltaTime;
		Player->AddActorLocation(PlayerRunVector);
		if (false == CyberBossMap->GetFrontDoorCollision())
		{
			StateChange(EBossLevelState::CheckPointRoom);
			return;
		}
	}
	CheckPointCameraVector(_DeltaTime);
}
#pragma endregion

#pragma region CheckPointRoom
void UCyberSpaceBossLevel::CheckPointRoomStart()
{
	PlayerRunVector = FVector::Zero;
	Player->SetStateChange(EEgseuState::Idle);
}

void UCyberSpaceBossLevel::CheckPointRoom(float _DeltaTime)
{
	// 뒷 문 충돌 체크.
	if (true == CyberBossMap->GetBackFoor())
	{
		StateChange(EBossLevelState::Back_Door);
		return;
	}
}
#pragma endregion

#pragma region Back_Door
void UCyberSpaceBossLevel::Back_DoorStart()
{
	PlayerRunVector = FVector::Zero;
	Player->SetStateChange(EEgseuState::BossRoomAutoRun);
}

void UCyberSpaceBossLevel::Back_Door(float _DeltaTime)
{
	if (true == CyberBossMap->GetBackDoorOpen())
	{
		PlayerRunVector = FVector::Right * PlayerRunSpeed * _DeltaTime;
		Player->AddActorLocation(PlayerRunVector);

		if (false == CyberBossMap->GetBackDoorCollision())
		{
			StateChange(EBossLevelState::BossRoom);
			return;
		}
	}
	InBossRoomCameraVector(_DeltaTime);
}
#pragma endregion

#pragma region BossRoom
void UCyberSpaceBossLevel::BossRoomStart()
{
	b_BossBattle = true; // 보스전 카메라 적용.
	Player->SetStateChange(EEgseuState::Wait);
	// 워닝.
	WarningTime = 0.0f;
}

void UCyberSpaceBossLevel::BossRoom(float _DeltaTime)
{
	WarningTime += _DeltaTime;
	if (WarningTime >= 2.0f)
	{
		StateChange(EBossLevelState::BossIntro);
		return;
	}
}
#pragma endregion

#pragma region BossIntro
void UCyberSpaceBossLevel::BossIntroStart()
{
	CyberBoss->SetActive(true); // 스코프 나오는 이슈의 원인
	CyberBoss->SetStateChange(ECyberPeacockState::Intro);
}

void UCyberSpaceBossLevel::BossIntro(float _DeltaTime)
{
	if (CyberBoss->GetIntro()) // Cyber에서 Intro 애니메이션이 끝나면,
	{
		StateChange(EBossLevelState::BossText);
		return;
	}
}
#pragma endregion

#pragma region BossText
void UCyberSpaceBossLevel::BossTextStart()
{
	TextTime = 0.0f;
}

void UCyberSpaceBossLevel::BossText(float _DeltaTime)
{
	TextTime += _DeltaTime;
	// 지금은 그냥 넘김.
	if (TextTime >= 2.0f)
	{
		StateChange(EBossLevelState::BossBattleReadySound);
		return;
	}
}
#pragma endregion

#pragma region BossReady
void UCyberSpaceBossLevel::BossBattleReadySoundStart()
{
	// UI 출력.
	// 보스 싸움 준비 소리.
	SoundTime = 0.0f;
}

void UCyberSpaceBossLevel::BossBattleReadySound(float _DeltaTime)
{
	SoundTime += _DeltaTime;
	// 소리 끝나면,
	if(0.2f <= SoundTime)
	{
		StateChange(EBossLevelState::BossReady);
		return;
	}
}
#pragma endregion

#pragma region BossReady
void UCyberSpaceBossLevel::BossReadyStart()
{
	// 보스 파칭~!
	CyberBoss->SetStateChange(ECyberPeacockState::BattleReady);
	HpUIEvent = 0.0f;
}

void UCyberSpaceBossLevel::BossReady(float _DeltaTime)
{
	if (CyberBoss->GetBattleReady())
	{
		HpUIEvent += _DeltaTime;
		if (HpUIEvent >= 2.0f)
		{
			// UI 채력바 올라가고,
			StateChange(EBossLevelState::BossBattle);
			return;
		}
	}
}
#pragma endregion

#pragma region BossBattle
void UCyberSpaceBossLevel::BossBattleStart()
{
	Player->SetStateChange(EEgseuState::Idle);
	CyberBoss->SetStateChange(ECyberPeacockState::Disappear);
}

void UCyberSpaceBossLevel::BossBattle(float _DeltaTime)
{

}
#pragma endregion

#pragma region str
#pragma endregion
void UCyberSpaceBossLevel::BossEndStart()
{
}

void UCyberSpaceBossLevel::BossEnd(float _DeltaTime)
{
}

#pragma region str
#pragma endregion
void UCyberSpaceBossLevel::PlayerReverseSummonStart()
{
}

void UCyberSpaceBossLevel::PlayerReverseSummon(float _DeltaTime)
{
}



#pragma region Camera
void UCyberSpaceBossLevel::MoveCameraVector()
{
	FVector CameraPos = GetCameraPos();
	FVector PlayerPos = Player->GetActorLocation();
	FVector ImageScale = CyberBossMap->GetImageScale();
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

void UCyberSpaceBossLevel::CheckPointCameraVector(float _DeltaTime)
{
	FVector CameraPos = GetCameraPos();

	CameraPos = FVector::Right * FrontDoorCameraSpeed * _DeltaTime;
	if (CameraPos.iX() <= 1000)
	{
		AddCameraPos(CameraPos);
	}
	else
	{
		AddCameraPos(FVector::Zero);
	}
}

void UCyberSpaceBossLevel::InBossRoomCameraVector(float _DeltaTime)
{
	FVector CameraPos = GetCameraPos();

	CameraRunPos = FVector::Right * BackDoorCameraSpeed * _DeltaTime;
	if (CameraPos.iX() <= 1838)
	{
		AddCameraPos(CameraRunPos);
	}
	else
	{
		AddCameraPos(FVector::Zero);
	}
}

void UCyberSpaceBossLevel::BossRoomCameraVector()
{
	// Boss Room X : 1838 ~ 2042
	FVector CameraPos = GetCameraPos();
	FVector PlayerPos = Player->GetActorLocation();

	FVector WindowScale = GEngine->MainWindow.GetWindowScale();

	CameraPos.X = PlayerPos.X - WindowScale.hX();
	CameraPos.Y = PlayerPos.Y - 562;

	if (1838.0f >= CameraPos.X)
	{
		CameraPos.X = 1838.0f;
	}
	if (2042.0f <= CameraPos.X)
	{
		CameraPos.X = 2042.0f;
	}

	if (0.0f >= CameraPos.Y)
	{
		CameraPos.Y = 0.0f;
	}

	SetCameraPos(CameraPos);
}
#pragma endregion
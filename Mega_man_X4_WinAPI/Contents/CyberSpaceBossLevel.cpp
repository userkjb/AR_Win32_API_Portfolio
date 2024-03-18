#include "CyberSpaceBossLevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>

#include "CyberSpaceBossMap.h"
#include "CyberSpaceLevel.h"
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
	UEngineResourcesManager::GetInst().CuttingImage("Missile_Death.png", 3, 1);
	UEngineResourcesManager::GetInst().CuttingImage("Scope.png", 1, 1);
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
	CyberBossMap->SetWhiteImage("White_Image.png");

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
		case EBossLevelState::BossDefeated:
			BossDefeatedStart();
			break;
		case EBossLevelState::BossEnd:
			BossEndStart();
			break;
		case EBossLevelState::BossEndMiddle:
			BossEndMiddleStart();
			break;
		case EBossLevelState::PlayerVictory:
			PlayerVictoryStart();
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
	case EBossLevelState::BossDefeated:
		BossDefeated(_DeltaTime);
		break;
	case EBossLevelState::BossEnd:
		BossEnd(_DeltaTime);
		break;
	case EBossLevelState::BossEndMiddle:
		BossEndMiddle(_DeltaTime);
		break;
	case EBossLevelState::PlayerVictory:
		PlayerVictory(_DeltaTime);
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

	if (Player->GetPlayerState() == EEgseuState::None)
	{
		Player->SetStateChange(EEgseuState::Summon);
	}
}
#pragma endregion

#pragma region Front_Door
void UCyberSpaceBossLevel::Front_DoorStart()
{
	PlayerRunVector = FVector::Zero;
	Player->SetStateChange(EEgseuState::BossRoomAutoRun);
	UEngineSound::SoundPlay("OpenDoor.mp3");
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
	UEngineSound::SoundPlay("OpenDoor.mp3");
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

	UEngineSoundPlayer BGMSound = UCyberSpaceLevel::GetSound();
	BGMSound.Off();

	UEngineSound::SoundPlay("Warning.mp3");
}

void UCyberSpaceBossLevel::BossRoom(float _DeltaTime)
{
	WarningTime += _DeltaTime;
	if (WarningTime >= 5.0f)
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
	if (TextTime >= 1.0f)
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
	BossBattleReadySoundTime = 0.0f;
}

void UCyberSpaceBossLevel::BossBattleReadySound(float _DeltaTime)
{
	BossBattleReadySoundTime += _DeltaTime;
	// 소리 끝나면,
	if(0.2f <= BossBattleReadySoundTime)
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
		if (HpUIEvent >= 1.0f)
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
	BattelSound = UEngineSound::SoundPlay("BossBGM.mp3");
	BattelSound.Loop();
}

void UCyberSpaceBossLevel::BossBattle(float _DeltaTime)
{
	// 플레이어 피 체크.
	// 보스 피 체크.
	int PlayerHp = Player->GetPlayerHp();
	int BossHp = CyberBoss->GetCyberPeacockHp();
	
	if (PlayerHp <= 0)
	{

	}

	if (BossHp <= 0)
	{
		StateChange(EBossLevelState::BossDefeated);
		return;
	}
}
#pragma endregion

#pragma region BossDefeated
void UCyberSpaceBossLevel::BossDefeatedStart()
{
	CyberBoss->SetStateChange(ECyberPeacockState::Death); // 반짝 반짝
	Player->SetStateChange(EEgseuState::Wait);
	BossEndSoundTime = 0.0f;
	b_BattelSound = false;
}
void UCyberSpaceBossLevel::BossDefeated(float _DeltaTime)
{
	BossEndSoundTime += _DeltaTime;
	if (BossEndSoundTime >= 2.0f && b_BattelSound == false) // Death
	{
		b_BattelSound = true;
		BattelSound.Off();
	}

	if (true == CyberBoss->GetDeathAni()) // 반짝 반짝 끝남.
	{
		StateChange(EBossLevelState::BossEnd);
		return;
	}
}
#pragma endregion


#pragma region BossEnd
void UCyberSpaceBossLevel::BossEndStart()
{
	CyberBoss->SetStateChange(ECyberPeacockState::Explosion); // 그럼 터져야지!
	CyberBossMap->SetStateChange(ECyberBossMapState::White); // 맵은 흰색으로 점점 변한다.
}

void UCyberSpaceBossLevel::BossEnd(float _DeltaTime)
{
	if (true == CyberBossMap->GetMapWhite()) // 흰색으로 다 변했으면,
	{
		StateChange(EBossLevelState::BossEndMiddle);
		return;
	}
}
#pragma endregion

#pragma region BossEndMiddle
void UCyberSpaceBossLevel::BossEndMiddleStart()
{
	CyberBoss->SetStateChange(ECyberPeacockState::End); // 보스 랜더는 지우고 소리는 남아있음.
	CyberBossMap->SetStateChange(ECyberBossMapState::Restore); // 흰색을 다시 원상 복귀!
	UEngineSound::SoundPlay("BossExplosion_4.mp3");
}
void UCyberSpaceBossLevel::BossEndMiddle(float _DeltaTime)
{
	// 맵 복귀가 완료되면,
	if (true == CyberBossMap->GetRestoreMap())
	{
		StateChange(EBossLevelState::PlayerVictory);
		return;
	}
}
#pragma endregion


void UCyberSpaceBossLevel::PlayerVictoryStart()
{
	BossEndTime = 0.0f;
	UEngineSound::SoundPlay("Victory_X.mp3");
}
void UCyberSpaceBossLevel::PlayerVictory(float _DeltaTime)
{
	// 승리 배경 음악이 끝나면,
	BossEndTime += _DeltaTime;
	if (BossEndTime >= 6.0f)
	{
		// Player 승리 자세.
		StateChange(EBossLevelState::PlayerReverseSummon);
		return;
	}
}

#pragma region PlayerReverseSummon
void UCyberSpaceBossLevel::PlayerReverseSummonStart()
{
	Player->SetStateChange(EEgseuState::Victory);
	CyberBoss->SetStateChange(ECyberPeacockState::Destroy);
}

void UCyberSpaceBossLevel::PlayerReverseSummon(float _DeltaTime)
{
	// 승리 포즈 끝나면, Player에서 자동으로 귀환.
	// 귀환이 끝나면,
	if (true == Player->GetReSummonEnd())
	{
		return;
		GEngine->ChangeLevel("TitleLevel");
	}
}
#pragma endregion


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
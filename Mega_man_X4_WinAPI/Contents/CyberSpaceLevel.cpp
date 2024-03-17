#include "CyberSpaceLevel.h"
#include "CyberSpaceMap.h"
#include "Egseu.h"
#include "TriScan.h"
#include "SpikeMarl.h"
#include "MiruTorearu.h"
#include <EngineCore/EngineResourcesManager.h>
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>

UCyberSpaceLevel::UCyberSpaceLevel()
{
}

UCyberSpaceLevel::~UCyberSpaceLevel()
{
}

UEngineSoundPlayer UCyberSpaceLevel::BGMSound;

UEngineSoundPlayer UCyberSpaceLevel::GetSound()
{
	return BGMSound;
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

	BGMSound = UEngineSound::SoundPlay("X4-CYBER_KUJACKER_STAGE.mp3");
	BGMSound.Loop();

	Ready_Sound = UEngineSound::SoundPlay("Ready.mp3");
	Ready_Sound.Loop(0);
	Ready_Sound.Off();

	//UEngineSound::SoundPlay("X4-CYBER_KUJACKER_STAGE.mp3");
}

void UCyberSpaceLevel::Tick(float _DeltaTime)
{
	ULevel::Tick(_DeltaTime);

	//TestCameraMove();
	MoveCameraVector();

	if (true == CyberSpaceMap->GetStageEnd())
	{
		GEngine->ChangeLevel("CyberSpaceBossLevel");
	}

	StateUpdate(_DeltaTime);

	// Enemy 소환.
	EnemySpawn();

	// Change Level Test
	if (true == UEngineInput::IsDown('R'))
	{
		GEngine->ChangeLevel("CyberSpaceBossLevel");
	}
}

void UCyberSpaceLevel::LevelStart(ULevel* _Level)
{
	// 리소스 로드.
	// 액터 생성.
	ULevel::LevelStart(_Level);

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
	Player = SpawnActor<AEgseu>(static_cast<int>(EActorType::Player));
	Player->SetActorLocation({ 200, 0 });
	CyberSpaceMap->SetPlayer(Player);

} // LevelStart

void UCyberSpaceLevel::LevelEnd(ULevel* _Level)
{
	// 리소스 삭제.
	// 액터 삭제.
	ULevel::LevelEnd(_Level);

	//CyberSpaceMap->Destroy(0.0f);
	Player->Destroy(0.0f);
	//TryScan_1->Destroy(0.0f);
	//TryScan_2->Destroy(0.0f);
	//CyberSpaceMap = nullptr;
	Player = nullptr;
	//TryScan_1 = nullptr;
	//TryScan_2 = nullptr;
}

void UCyberSpaceLevel::StateChange(ECyberStageState _State)
{
	if (LevelState != _State)
	{
		switch (_State)
		{
		case ECyberStageState::None:
			break;
		case ECyberStageState::Ready:
			ReadyStart();
			break;
		case ECyberStageState::GameProgress:
			GameProgressStart();
			break;
		default :
			break;
		}
	}
	LevelState = _State;
}

void UCyberSpaceLevel::StateUpdate(float _DeltaTime)
{
	switch (LevelState)
	{
	case ECyberStageState::None:
		None(_DeltaTime);
		break;
	case ECyberStageState::Ready:
		Ready(_DeltaTime);
		break;
	case ECyberStageState::GameProgress:
		GameProgress(_DeltaTime);
		break;
	default:
		break;
	}
}


void UCyberSpaceLevel::None(float _DeltaTime)
{
	StateChange(ECyberStageState::Ready);
	return;
}

#pragma region Ready
void UCyberSpaceLevel::ReadyStart()
{
	SoundTime = 0.0f;
}

void UCyberSpaceLevel::Ready(float _DeltaTime)
{
	SoundTime += _DeltaTime;
	if (SoundTime >= 2.0f)
	{
		Ready_Sound.On();
		if (SoundTime >= 3.0f)
		{
			if (Player->GetPlayerState() == EEgseuState::None)
			{
				Player->SetStateChange(EEgseuState::Summon);
				StateChange(ECyberStageState::GameProgress);
				return;
			}
		}
	}
}
#pragma endregion

#pragma region GameProgress
void UCyberSpaceLevel::GameProgressStart()
{
	SoundTime = 0.0f;

}
void UCyberSpaceLevel::GameProgress(float _DeltaTime)
{

}
#pragma endregion



void UCyberSpaceLevel::MoveCameraVector()
{
	FVector CameraPos = GetCameraPos();
	FVector PlayerPos = Player->GetActorLocation();
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

void UCyberSpaceLevel::EnemySpawn()
{
	float PlayerPos = Player->GetActorLocation().X;
	if (520.0f <= PlayerPos && PlayerPos <= 530.0f)
	{
		if (TryScan_1 == nullptr && TryScan_2 == nullptr)
		{
			TryScan_1 = SpawnActor<ATriScan>(static_cast<int>(EActorType::Enemy));
			TryScan_1->SetActorLocation(TryScan_1_Pos);
			TryScan_1->SetTriScanState(ETriScanState::Idle);
			//TryScan_1->SetActorLocation({ 600 , 500 });

			TryScan_2 = SpawnActor<ATriScan>(static_cast<int>(EActorType::Enemy));
			TryScan_2->SetActorLocation(TryScan_2_Pos);
			TryScan_2->SetTriScanState(ETriScanState::Idle);
		}
	}

	if (760.0f <= PlayerPos && PlayerPos <= 770.0f)
	{
		if (MiruTorearu_1 == nullptr)
		{
			MiruTorearu_1 = SpawnActor<AMiruTorearu>(static_cast<int>(EActorType::Enemy));
			MiruTorearu_1->SetActorLocation({ MiruTorearu_1_Pos });
			MiruTorearu_1->SetMiruDir(EActorDir::Left);
			MiruTorearu_1->SetMiruTorearuState(EMiruTorearuState::RunCreate);
			MiruTorearu_1->SetPlayer(Player);
		}
	}

	if (1185.0f <= PlayerPos && PlayerPos <= 1190.0f)
	{
		if (MiruTorearu_2 == nullptr)
		{
			MiruTorearu_2 = SpawnActor<AMiruTorearu>(static_cast<int>(EActorType::Enemy));
			MiruTorearu_2->SetActorLocation({ MiruTorearu_2_Pos });
			MiruTorearu_2->SetMiruDir(EActorDir::Left);
			MiruTorearu_2->SetMiruTorearuState(EMiruTorearuState::RunCreate);
			MiruTorearu_2->SetPlayer(Player);
		}
	}

	if (1460.0f <= PlayerPos && PlayerPos <= 1465.0f)
	{
		// 2개
		if (MiruTorearu_3 == nullptr)
		{
			MiruTorearu_3 = SpawnActor<AMiruTorearu>(static_cast<int>(EActorType::Enemy));
			MiruTorearu_3->SetActorLocation({ MiruTorearu_3_Pos });
			MiruTorearu_3->SetMiruDir(EActorDir::Left);
			MiruTorearu_3->SetMiruTorearuState(EMiruTorearuState::RunCreate);
			MiruTorearu_3->SetPlayer(Player);
		}

		if (MiruTorearu_4 == nullptr)
		{
			MiruTorearu_4 = SpawnActor<AMiruTorearu>(static_cast<int>(EActorType::Enemy));
			MiruTorearu_4->SetActorLocation({ MiruTorearu_4_Pos });
			MiruTorearu_4->SetMiruDir(EActorDir::Left);
			MiruTorearu_4->SetMiruTorearuState(EMiruTorearuState::RunCreate);
			MiruTorearu_4->SetPlayer(Player);
		}
	}

	if (1650.0f <= PlayerPos && PlayerPos <= 1670.0f)
	{
		if (MiruTorearu_5 == nullptr)
		{
			MiruTorearu_5 = SpawnActor<AMiruTorearu>(static_cast<int>(EActorType::Enemy));
			MiruTorearu_5->SetActorLocation({ MiruTorearu_5_Pos });
			MiruTorearu_5->SetMiruDir(EActorDir::Left);
			MiruTorearu_5->SetMiruTorearuState(EMiruTorearuState::RunCreate);
			MiruTorearu_5->SetPlayer(Player);
		}

		if (MiruTorearu_6 == nullptr)
		{
			MiruTorearu_6 = SpawnActor<AMiruTorearu>(static_cast<int>(EActorType::Enemy));
			MiruTorearu_6->SetActorLocation({ MiruTorearu_6_Pos });
			MiruTorearu_6->SetMiruDir(EActorDir::Left);
			MiruTorearu_6->SetMiruTorearuState(EMiruTorearuState::RunCreate);
			MiruTorearu_6->SetPlayer(Player);
		}

		if (TryScan_3 == nullptr)
		{
			TryScan_3 = SpawnActor<ATriScan>(static_cast<int>(EActorType::Enemy));
			TryScan_3->SetActorLocation(TryScan_3_Pos);
			TryScan_3->SetTriScanState(ETriScanState::Idle);
		}

		if (TryScan_4 == nullptr)
		{
			TryScan_4 = SpawnActor<ATriScan>(static_cast<int>(EActorType::Enemy));
			TryScan_4->SetActorLocation(TryScan_4_Pos);
			TryScan_4->SetTriScanState(ETriScanState::Idle);
		}
	}

	if (2135.0f <= PlayerPos && PlayerPos <= 2140.0f)
	{
		if (MiruTorearu_7 == nullptr)
		{
			MiruTorearu_7 = SpawnActor<AMiruTorearu>(static_cast<int>(EActorType::Enemy));
			MiruTorearu_7->SetActorLocation({ MiruTorearu_7_Pos });
			MiruTorearu_7->SetMiruDir(EActorDir::Left);
			MiruTorearu_7->SetMiruTorearuState(EMiruTorearuState::RunCreate);
			MiruTorearu_7->SetPlayer(Player);
		}
	}

	if (2750.0f <= PlayerPos && PlayerPos <= 2755.0f)
	{
		if (MiruTorearu_8 == nullptr)
		{
			MiruTorearu_8 = SpawnActor<AMiruTorearu>(static_cast<int>(EActorType::Enemy));
			MiruTorearu_8->SetActorLocation({ MiruTorearu_8_Pos });
			MiruTorearu_8->SetMiruDir(EActorDir::Left);
			MiruTorearu_8->SetMiruTorearuState(EMiruTorearuState::RunCreate);
			MiruTorearu_8->SetPlayer(Player);
		}

		if (MiruTorearu_9 == nullptr)
		{
			MiruTorearu_9 = SpawnActor<AMiruTorearu>(static_cast<int>(EActorType::Enemy));
			MiruTorearu_9->SetActorLocation({ MiruTorearu_9_Pos });
			MiruTorearu_9->SetMiruDir(EActorDir::Left);
			MiruTorearu_9->SetMiruTorearuState(EMiruTorearuState::RunCreate);
			MiruTorearu_9->SetPlayer(Player);
		}
	}

	if (2950.0f <= PlayerPos && PlayerPos <= 3955.0f)
	{
		if (MiruTorearu_10 == nullptr)
		{
			MiruTorearu_10 = SpawnActor<AMiruTorearu>(static_cast<int>(EActorType::Enemy));
			MiruTorearu_10->SetActorLocation({ MiruTorearu_10_Pos });
			MiruTorearu_10->SetMiruDir(EActorDir::Left);
			MiruTorearu_10->SetMiruTorearuState(EMiruTorearuState::RunCreate);
			MiruTorearu_10->SetPlayer(Player);
		}
	}
	

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

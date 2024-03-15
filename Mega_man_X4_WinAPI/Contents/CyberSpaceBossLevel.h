#pragma once
#include <EngineCore/Level.h>
#include "ContentsGlobalData.h"

class ACyberSpaceBossMap;
class AEgseu;
class ACyberPeacock;


class UCyberSpaceBossLevel : public ULevel
{
public :
	// constrcuter destructer
	UCyberSpaceBossLevel();
	~UCyberSpaceBossLevel();

	// delete Function
	UCyberSpaceBossLevel(const UCyberSpaceBossLevel& _Other) = delete;
	UCyberSpaceBossLevel(UCyberSpaceBossLevel&& _Other) noexcept = delete;
	UCyberSpaceBossLevel& operator=(const UCyberSpaceBossLevel& _Other) = delete;
	UCyberSpaceBossLevel& operator=(UCyberSpaceBossLevel&& _Other) noexcept = delete;

protected :
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void LevelStart(ULevel* _Level) override;
	void LevelEnd(ULevel* _Level) override;

private :

	void StateChange(EBossLevelState _State);
	void StateUpdate(float _DeltaTime);

	//void NoneStart();
	void None(float _DeltaTime);
	void Front_DoorStart();
	void Front_Door(float _DeltaTime);
	void CheckPointRoomStart();
	void CheckPointRoom(float _DeltaTime);
	void Back_DoorStart();
	void Back_Door(float _DeltaTime);
	void BossRoomStart();
	void BossRoom(float _DeltaTime);
	void BossIntroStart();
	void BossIntro(float _DeltaTime);
	void BossTextStart();
	void BossText(float _DeltaTime);
	void BossBattleReadySoundStart();
	void BossBattleReadySound(float _DeltaTime);
	void BossReadyStart();
	void BossReady(float _DeltaTime);
	void BossBattleStart();
	void BossBattle(float _DeltaTime);
	void BossEndStart();
	void BossEnd(float _DeltaTime);
	void PlayerReverseSummonStart();
	void PlayerReverseSummon(float _DeltaTime);

	// Camera
	void MoveCameraVector(); // 일반적인 카메라.
	void CheckPointCameraVector(float _DeltaTime); // CheckPointRoom 들어갈 때 카메라.(다 들어가면 일반으로.)
	void InBossRoomCameraVector(float _DeltaTime); // 보스룸 들어갈 때 카메라.
	void BossRoomCameraVector();

	// Level State
	EBossLevelState LevelState = EBossLevelState::None;

	// Map
	ACyberSpaceBossMap* CyberBossMap = nullptr;

	// Player
	AEgseu* Player = nullptr;

	// Boss
	ACyberPeacock* CyberBoss = nullptr;

	FVector CameraRunPos = FVector::Zero;
	const float FrontDoorCameraSpeed = 170.0f;
	const float BackDoorCameraSpeed = 270.0f;
	FVector PlayerRunVector = FVector::Zero;
	const float PlayerRunSpeed = 100.0f;

	bool b_BossBattle = false;
	float WarningTime = 0.0f; // test
	float TextTime = 0.0f; // test
	float BossBattleReadySoundTime = 0.0f; // test
	float HpUIEvent = 0.0f; // test
	float BossEndTime = 0.0f;
};

/*
#pragma region str
#pragma endregion
*/
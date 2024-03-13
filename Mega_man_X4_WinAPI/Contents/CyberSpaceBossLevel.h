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


	// Camera
	void MoveCameraVector(bool _IsBoss);
	void CheckPointCameraVector();
	void BossRoomCameraVector();

	// Level State
	EBossLevelState LevelState = EBossLevelState::None;

	// Map
	ACyberSpaceBossMap* CyberBossMap = nullptr;

	// Player
	AEgseu* Player = nullptr;

	// Boss
	ACyberPeacock* CyberBoss = nullptr;
};

/*
#pragma region str
#pragma endregion
*/
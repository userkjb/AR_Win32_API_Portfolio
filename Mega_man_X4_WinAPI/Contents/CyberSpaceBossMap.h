#pragma once
#include <EngineCore/Actor.h>
#include "ContentsGlobalData.h"

class AEgseu;
class ACyberPeacock;

class ACyberSpaceBossMap : public AActor
{
public :
	// constrcuter destructer
	ACyberSpaceBossMap();
	~ACyberSpaceBossMap();

	// delete Function
	ACyberSpaceBossMap(const ACyberSpaceBossMap& _Other) = delete;
	ACyberSpaceBossMap(ACyberSpaceBossMap&& _Other) noexcept = delete;
	ACyberSpaceBossMap& operator=(const ACyberSpaceBossMap& _Other) = delete;
	ACyberSpaceBossMap& operator=(ACyberSpaceBossMap&& _Other) noexcept = delete;

	// Map
	void SetMapImage(std::string_view _MapImageName);

	// Map Col
	void SetMapColImage(std::string_view _MapColImageName);

	// Door_1
	// Door_2
	void SetDoorImage(std::string_view _DoorImageName);


	/// <summary>
	/// Debug Function
	/// </summary>
	void SwitchDebug();

	FVector GetImageScale() const
	{
		return ImageScale;
	}

	
	bool GetFrontDoorCollision() const
	{
		return IsFDCollision;
	}

	bool GetBackDoorCollision() const
	{
		return IsBDCollision;
	}

	bool GetFrontDoor() const
	{
		return FrontDoor;
	}

	bool GetBackFoor() const
	{
		return BackDoor;
	}
	
	bool GetFrontDoorOpen() const
	{
		return FrontDoorOpen;
	}

	bool GetBackDoorOpen() const
	{
		return BackDoorOpen;
	}

	inline bool GetMapWhite() const
	{
		return MapWhite;
	}

	inline bool GetRestoreMap() const
	{
		return MapRestore;
	}

	inline void SetStateChange(ECyberBossMapState _State)
	{
		StateChange(_State);
	}

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private :

	void CreateDoorAni();

	void StateChange(ECyberBossMapState _State);
	void StateUpdate(float _DeltaTime);

	void IdleStart();
	void Idle(float _DeltaTime);
	void SlowMoveStart();
	void SlowMove(float _DeltaTime);
	void CheckPointRoomStart();
	void CheckPointRoom(float _DeltaTime);
	void BossSlowMoveStart();
	void BossSlowMove(float _DeltaTime);
	void BossRoomStart();
	void BossRoom(float _DeltaTime);
	void WhiteStart();
	void White(float _DeltaTime);
	void RestoreStart();
	void Restore(float _DeltaTime);

	void CollisionCheck(float _DeltaTime);

	FVector ImageScale;
	FVector BossDoorPos_1 = { 1080, 480 }; // 문 위치
	FVector BossDoorPos_2 = { 1840, 480 }; // 문 위치

	ECyberBossMapState State = ECyberBossMapState::None;

	UImageRenderer* MapRenderer = nullptr;
	UImageRenderer* MapColRenderer = nullptr;

	UImageRenderer* BossDoor_1 = nullptr;
	UImageRenderer* BossDoor_2 = nullptr;
	UCollision* BossDoor_Coll_1 = nullptr;
	UCollision* BossDoor_Coll_2 = nullptr;

	AEgseu* Player = nullptr;
	AEgseu* ColPlayer = nullptr;
	FVector RunVector = FVector::Zero;
	const float RunSpeed = 100.0f;
	FVector StartCameraPos = FVector::Zero;
	const float CameraSpeed = 170.0f;
	FVector CameraRunPos = FVector::Zero;

	ACyberPeacock* CyberBoss = nullptr;

	bool IsFDCollision = false; // 앞 문 충돌 상태.
	bool IsBDCollision = false; // 뒷 문 충돌 상태.
	bool FrontDoor = false;
	bool BackDoor = false;
	bool FrontDoorOpen = false; // 앞 문 열림.
	bool BackDoorOpen = false; // 뒷 문 열림.
	//bool IsBoss = false;
	bool MapWhite = false;
	bool MapRestore = false;
	int CollisionCount_1 = 0;
	int CollisionCount_2 = 0;
};

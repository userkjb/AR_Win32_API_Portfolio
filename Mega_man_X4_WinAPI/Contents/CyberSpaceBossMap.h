#pragma once
#include <EngineCore/Actor.h>
#include "ContentsGlobalData.h"

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
	void SetDoorImage(std::string_view _DoorImageName);

	// Door_2

	/// <summary>
	/// Debug Function
	/// </summary>
	void SwitchDebug();

	FVector GetImageScale() const
	{
		return ImageScale;
	}

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private :

	void CreateDoorAni();

	void StateChange(EBossDoorState _State);
	void StateUpdate(float _DeltaTime);

	void CreateStart();
	void Create(float _DeltaTime);
	void IdleStart();
	void Idle(float _DeltaTime);
	void OpenStart();
	void Open(float _DeltaTime);
	void CloseStart();
	void Close(float _DeltaTime);


	void CollisionCheck(float _DeltaTime);

	FVector ImageScale;
	FVector BossDoorPos_1 = { 1080, 480 };
	FVector BossDoorPos_2 = { 1840, 480 };

	EBossDoorState State = EBossDoorState::None;

	UImageRenderer* MapRenderer = nullptr;
	UImageRenderer* MapColRenderer = nullptr;

	UImageRenderer* BossDoor_1 = nullptr;
	UImageRenderer* BossDoor_2 = nullptr;
	UCollision* BossDoor_Coll_1 = nullptr;
	UCollision* BossDoor_Coll_2 = nullptr;
};

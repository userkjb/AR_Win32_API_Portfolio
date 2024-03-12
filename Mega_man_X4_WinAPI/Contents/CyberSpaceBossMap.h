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

	FVector ImageScale;
	FVector BossDoorPos_1 = { 1080, 480 };
	FVector BossDoorPos_2 = { 1840, 480 };

	UImageRenderer* MapRenderer = nullptr;
	UImageRenderer* MapColRenderer = nullptr;
	UImageRenderer* BossDoor_1 = nullptr;
	UImageRenderer* BossDoor_2 = nullptr;
};

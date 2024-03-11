#pragma once
#include <EngineCore/Actor.h>
#include "ContentsGlobalData.h"

class AEgseu;
class APlayerFocus;

class ACyberSpaceMap : public AActor
{
public :
	// constrcuter destructer
	ACyberSpaceMap();
	~ACyberSpaceMap();

	// delete Function
	ACyberSpaceMap(const ACyberSpaceMap& _Other) = delete;
	ACyberSpaceMap(ACyberSpaceMap&& _Other) noexcept = delete;
	ACyberSpaceMap& operator=(const ACyberSpaceMap& _Other) = delete;
	ACyberSpaceMap& operator=(ACyberSpaceMap&& _Other) noexcept = delete;

	// Map
	void SetMapImage(std::string_view _MapImageName);
	// Col Map
	void SetColMapImage(std::string_view _MapImageName);
	// Map Object
	void SetMapExitImage(std::string_view _MapImageName);

	void SetPlayer(AEgseu* _Player)
	{
		Player = _Player;
	}

	void SwitchDebug();

	FVector GetImageScale() const
	{
		return ImageScale;
	}

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private :
	void StateChange(ECyberSpaceMapState _State);
	void StateUpdate(float _DeltaTime);

	void PlayerFocus_StartBegin();
	void PlayerFocus_Start(float _DeltaTime);
	void PlayerFocus_LoopBegin();
	void PlayerFocus_Loop(float _DeltaTime);
	void PlayerFocus_EndBegin();
	void PlayerFocus_End(float _DeltaTime);
	
	void PlayerPosEvent(float _DeltaTime);

	ECyberSpaceMapState State = ECyberSpaceMapState::None;
	float FocusCreateTime = 0.0f;
	float FocusTime = 0.0f;
	APlayerFocus* Focus = nullptr;
	AEgseu* Player = nullptr;
	EFocusState FocusState = EFocusState::None;
	EEgseuState PreState = EEgseuState::None;

	UImageRenderer* MapRenderer = nullptr;	// Map 이미지
	UImageRenderer* ColRenderer = nullptr;	// ColMap 이미지
	UImageRenderer* MapExit = nullptr;
	FVector ImageScale;
};


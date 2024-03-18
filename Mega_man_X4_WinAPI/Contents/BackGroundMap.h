#pragma once
#include <EngineCore/Actor.h>
#include "ContentsGlobalData.h"

class ABackGroundMap : public AActor
{
public :
	// constrcuter destructer
	ABackGroundMap();
	~ABackGroundMap();

	// delete Function
	ABackGroundMap(const ABackGroundMap& _Other) = delete;
	ABackGroundMap(ABackGroundMap&& _Other) noexcept = delete;
	ABackGroundMap& operator=(const ABackGroundMap& _Other) = delete;
	ABackGroundMap& operator=(ABackGroundMap&& _Other) noexcept = delete;

	/// <summary>
	/// Back Ground Map 설정.
	/// </summary>
	/// <param name="_MapImageName"></param>
	void SetMapImage(std::string_view _MapImageName);

	/// <summary>
	/// Menu Animation 설정
	/// </summary>
	/// <param name="_LogoImageName"></param>
	void SetMenuAnimation(std::string_view _LogoImageName);
	
	/// <summary>
	/// Game Start 안내 Image 설정.
	/// </summary>
	/// <param name="_ImageName"></param>
	void SetGameStartTextImage(std::string_view _ImageName);

	inline void SetStageChange(EBackGroundMapState _State)
	{
		StateChange(_State);
	}

protected :
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private :

	void StateChange(EBackGroundMapState _State);
	void StateUpdate(float _DeltaTime);

	void TitleStart();
	void Title(float _DeltaTime);
	void GameMenuStart();
	void GameMenu(float _DeltaTime);

	UImageRenderer* TitleRenderer = nullptr;
	UImageRenderer* GameStartTextRenderer = nullptr;
	UImageRenderer* MenuRenderer = nullptr;

	EBackGroundMapState State = EBackGroundMapState::None;

	bool EnterKey = false;
	int KeyCount = 1;
};


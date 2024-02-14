#pragma once
#include <EngineCore/Actor.h>

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
	/// Map ¼³Á¤.
	/// </summary>
	/// <param name="_MapImageName"></param>
	void SetMapImage(std::string_view _MapImageName);

	void SetMenuImage(std::string_view _LogoImageName);
	
	void SetGameStartTextImage(std::string_view _ImageName);

protected :
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void MenuChange();

private :
	UImageRenderer* TitleRenderer = nullptr;
	UImageRenderer* GameStartTextRenderer = nullptr;
	UImageRenderer* MenuRenderer = nullptr;

	bool EnterKey = false;
};


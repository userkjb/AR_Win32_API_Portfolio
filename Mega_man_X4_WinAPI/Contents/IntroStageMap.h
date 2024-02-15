#pragma once
#include <EngineCore/Actor.h>

class AIntroStageMap : public AActor
{
public:
	// constrcuter destructer
	AIntroStageMap();
	~AIntroStageMap();

	// delete Function
	AIntroStageMap(const AIntroStageMap& _Other) = delete;
	AIntroStageMap(AIntroStageMap&& _Other) noexcept = delete;
	AIntroStageMap& operator=(const AIntroStageMap& _Other) = delete;
	AIntroStageMap& operator=(AIntroStageMap&& _Other) noexcept = delete;

	/// <summary>
	/// Intro Stage 1 의 Map 설정.
	/// </summary>
	/// <param name="_MapImageName"></param>
	void SetMapImage(std::string_view _MapImageName);
	/// <summary>
	/// Intro Stage 1의 Map의 Col
	/// </summary>
	/// <param name="_MapImageName"></param>
	void SetColMapImage(std::string_view _MapImageName);

	void SwitchDebug();

protected :
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private :
	UImageRenderer* Renderer = nullptr;		// Map 이미지
	UImageRenderer* ColRenderer = nullptr;	// ColMap 이미지
};


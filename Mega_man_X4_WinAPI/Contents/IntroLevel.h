#pragma once
#include <EngineCore/Level.h>

class AIntroStageMap;

class UIntroLevel : public ULevel
{
public :
	// constrcuter destructer
	UIntroLevel();
	~UIntroLevel();

	// delete Function
	UIntroLevel(const UIntroLevel& _Other) = delete;
	UIntroLevel(UIntroLevel&& _Other) noexcept = delete;
	UIntroLevel& operator=(const UIntroLevel& _Other) = delete;
	UIntroLevel& operator=(UIntroLevel&& _Other) noexcept = delete;

protected :
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void LevelStart(ULevel* _Level) override;
	void LevelEnd(ULevel* _Level) override;

private :
	AIntroStageMap* IntroStageMap = nullptr;
};


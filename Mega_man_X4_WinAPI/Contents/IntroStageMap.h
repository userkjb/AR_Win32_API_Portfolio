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

protected :

private :

};


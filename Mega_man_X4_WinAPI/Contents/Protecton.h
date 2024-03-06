#pragma once
#include <EngineCore/Actor.h>
#include "ContentsGlobalData.h"

class ABuster;

class AProtecton : public AActor
{
public:
	// constrcuter destructer
	AProtecton();
	~AProtecton();

	// delete Function
	AProtecton(const AProtecton& _Other) = delete;
	AProtecton(AProtecton&& _Other) noexcept = delete;
	AProtecton& operator=(const AProtecton& _Other) = delete;
	AProtecton& operator=(AProtecton&& _Other) noexcept = delete;

protected :
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private :

};


#pragma once
#include <EngineCore/Actor.h>

class ABuster : public AActor
{
public :
	// constrcuter destructer
	ABuster();
	~ABuster();

	// delete Function
	ABuster(const ABuster& _Other) = delete;
	ABuster(ABuster&& _Other) noexcept = delete;
	ABuster& operator=(const ABuster& _Other) = delete;
	ABuster& operator=(ABuster&& _Other) noexcept = delete;

	void SetDir(FVector _Dir)
	{
		Dir = _Dir;
	}

protected :
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private :
	FVector Dir = FVector::Zero;
	float BusterSpeed = 1.0f;
};


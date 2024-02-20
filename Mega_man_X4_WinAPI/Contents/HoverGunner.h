#pragma once
#include <EngineCore/Actor.h>

class AHoverGunner : public AActor
{
public:
	// constrcuter destructer
	AHoverGunner();
	~AHoverGunner();

	// delete Function
	AHoverGunner(const AHoverGunner& _Other) = delete;
	AHoverGunner(AHoverGunner&& _Other) noexcept = delete;
	AHoverGunner& operator=(const AHoverGunner& _Other) = delete;
	AHoverGunner& operator=(AHoverGunner&& _Other) noexcept = delete;
};


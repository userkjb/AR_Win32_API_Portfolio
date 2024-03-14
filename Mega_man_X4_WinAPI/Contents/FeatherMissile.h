#pragma once
#include <EngineCore/Actor.h>
#include "ContentsGlobalData.h"

class AFeatherMissile : public AActor
{
public :
	// constrcuter destructer
	AFeatherMissile();
	~AFeatherMissile();

	// delete Function
	AFeatherMissile(const AFeatherMissile& _Other) = delete;
	AFeatherMissile(AFeatherMissile&& _Other) noexcept = delete;
	AFeatherMissile& operator=(const AFeatherMissile& _Other) = delete;
	AFeatherMissile& operator=(AFeatherMissile&& _Other) noexcept = delete;

protected :
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private :
	void StateChange(ECyberPeacockMissileState _State);
	void StateUpdate(float _DeltaTime);


	UImageRenderer* MissileRenderer = nullptr;
	UCollision* MissileCollision = nullptr;

	ECyberPeacockMissileState MillileState = ECyberPeacockMissileState::None;

	FVector MissileVector = FVector::Zero;
	const float Speed = 1000.0f;
};


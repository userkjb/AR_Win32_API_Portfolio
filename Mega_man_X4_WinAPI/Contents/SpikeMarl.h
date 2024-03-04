#pragma once
#include <EngineCore/Actor.h>
#include "ContentsGlobalData.h"

class ASpikeMarl : public AActor
{
public :
	// constrcuter destructer
	ASpikeMarl();
	~ASpikeMarl();

	// delete Function
	ASpikeMarl(const ASpikeMarl& _Other) = delete;
	ASpikeMarl(ASpikeMarl&& _Other) noexcept = delete;
	ASpikeMarl& operator=(const ASpikeMarl& _Other) = delete;
	ASpikeMarl& operator=(ASpikeMarl&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private :

	void StateChange(ESpikeMarlState _State);
	void StateUpdate(float _DeltaTime);

	ESpikeMarlState State = ESpikeMarlState::None;
	int Hp = 6;
};


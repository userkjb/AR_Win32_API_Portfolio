#include "SpikeMarl.h"

ASpikeMarl::ASpikeMarl()
{
}

ASpikeMarl::~ASpikeMarl()
{
}

void ASpikeMarl::BeginPlay()
{
	AActor::BeginPlay();

	//StateChange(ESpikeMarlState::None);
}

void ASpikeMarl::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	StateUpdate(_DeltaTime);
}

void ASpikeMarl::StateChange(ESpikeMarlState _State)
{
	if (State != _State)
	{
		switch (_State)
		{
		case ESpikeMarlState::Idle:
			break;
		default:
			break;
		}
	}
}

void ASpikeMarl::StateUpdate(float _DeltaTime)
{
	switch (State)
	{
	case ESpikeMarlState::Idle:
		break;
	default:
		break;
	}
}

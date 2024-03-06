#include "MiruTorearu.h"

AMiruTorearu::AMiruTorearu()
{
}

AMiruTorearu::~AMiruTorearu()
{
}

void AMiruTorearu::BeginPlay()
{
	AActor::BeginPlay();

	BeginRender();
}

void AMiruTorearu::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
}

void AMiruTorearu::BeginRender()
{

}

void AMiruTorearu::StateChange(EMiruTorearuState _State)
{
	if (State != _State)
	{
		switch (_State)
		{
		case EMiruTorearuState::None:
			break;
		case EMiruTorearuState::Stop:
			break;
		case EMiruTorearuState::Run:
			break;
		case EMiruTorearuState::Attack:
			break;
		case EMiruTorearuState::DeathStart:
			break;
		case EMiruTorearuState::DeathLoop:
			break;
		case EMiruTorearuState::DeathEnd:
			break;
		default:
			break;
		}
	}
	State = _State;
}

void AMiruTorearu::StateUpdate(float _DeltaTime)
{
	switch (State)
	{
	case EMiruTorearuState::None:
		break;
	case EMiruTorearuState::Stop:
		break;
	case EMiruTorearuState::Run:
		break;
	case EMiruTorearuState::Attack:
		break;
	case EMiruTorearuState::DeathStart:
		break;
	case EMiruTorearuState::DeathLoop:
		break;
	case EMiruTorearuState::DeathEnd:
		break;
	default:
		break;
	}
}

#pragma region None
void AMiruTorearu::NoneStart()
{
}

void AMiruTorearu::None(float _DeltaTime)
{
}
#pragma endregion

#pragma region Stop
void AMiruTorearu::StopStart()
{
}

void AMiruTorearu::Stop(float _DeltaTime)
{
}
#pragma endregion

#pragma region Run
void AMiruTorearu::RunStart()
{
}

void AMiruTorearu::Run(float _DeltaTime)
{
}
#pragma endregion

#pragma region Attack
void AMiruTorearu::AttackStart()
{
}

void AMiruTorearu::Attack(float _DeltaTime)
{
}
#pragma endregion

#pragma region DeathStart
void AMiruTorearu::DeathStartStart()
{
}

void AMiruTorearu::DeathStart(float _DeltaTime)
{
}
#pragma endregion

#pragma region DeathLoop
void AMiruTorearu::DeathLoopStart()
{
}

void AMiruTorearu::DeathLoop(float _DeltaTime)
{
}
#pragma endregion

#pragma region DeathEnd
void AMiruTorearu::DeathEndStart()
{
}

void AMiruTorearu::DeathEnd(float _DeltaTime)
{
}
#pragma endregion

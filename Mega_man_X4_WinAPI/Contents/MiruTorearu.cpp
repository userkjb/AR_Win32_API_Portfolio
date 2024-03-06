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
			NoneStart();
			break;
		case EMiruTorearuState::Stop:
			StopStart();
			break;
		case EMiruTorearuState::Run:
			RunStart();
			break;
		case EMiruTorearuState::Attack:
			AttackStart();
			break;
		case EMiruTorearuState::DeathStart:
			DeathStartStart();
			break;
		case EMiruTorearuState::DeathLoop:
			DeathLoopStart();
			break;
		case EMiruTorearuState::DeathEnd:
			DeathEndStart();
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
		None(_DeltaTime);
		break;
	case EMiruTorearuState::Stop:
		Stop(_DeltaTime);
		break;
	case EMiruTorearuState::Run:
		Run(_DeltaTime);
		break;
	case EMiruTorearuState::Attack:
		Attack(_DeltaTime);
		break;
	case EMiruTorearuState::DeathStart:
		DeathStart(_DeltaTime);
		break;
	case EMiruTorearuState::DeathLoop:
		DeathLoop(_DeltaTime);
		break;
	case EMiruTorearuState::DeathEnd:
		DeathEnd(_DeltaTime);
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

#include "PlayerFocus.h"

APlayerFocus::APlayerFocus()
{
}

APlayerFocus::~APlayerFocus()
{
}

void APlayerFocus::BeginPlay()
{
	AActor::BeginPlay();

	ForcusComponent();

	StateChange(EFocusState::None);
}

void APlayerFocus::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	StateUpdate(_DeltaTime);
}

void APlayerFocus::ForcusComponent()
{
	FVector ActorPos = GetActorLocation();

	//RightUpRender = 
	//LeftUpRender = 
	//LeftDownRender = 
	//HurryUpRender = 
	//FocusRender = 
}

void APlayerFocus::StateChange(EFocusState _State)
{
	if (State != _State)
	{
		switch (_State)
		{
		case EFocusState::None:
			//NoneStart();
			break;
		case EFocusState::CallCreate:
			//CallCreateStart();
			break;
		case EFocusState::Create:
			CreateStart();
			break;
		case EFocusState::Run:
			RunStart();
			break;
		case EFocusState::Death:
			DeathStart();
			break;
		default :
			break;
		}
	}
	State = _State;
}

void APlayerFocus::StateUpdate(float _DeltaTime)
{
	switch (State)
	{
	case EFocusState::None:
		//None(_DeltaTime);
		break;
	case EFocusState::CallCreate:
		CallCreate(_DeltaTime);
		break;
	case EFocusState::Create:
		Create(_DeltaTime);
		break;
	case EFocusState::Run:
		Run(_DeltaTime);
		break;
	case EFocusState::Death:
		Death(_DeltaTime);
		break;
	default:
		break;
	}
}

#pragma region CallCreate
void APlayerFocus::CallCreate(float _DeltaTime)
{
	int a = 0;
}
#pragma endregion

#pragma region Create
void APlayerFocus::CreateStart()
{
}

void APlayerFocus::Create(float _DeltaTime)
{
}
#pragma endregion

#pragma region Run
void APlayerFocus::RunStart()
{
}

void APlayerFocus::Run(float _DeltaTime)
{
}
#pragma endregion

#pragma region Death
void APlayerFocus::DeathStart()
{
}

void APlayerFocus::Death(float _DeltaTime)
{
}
#pragma endregion

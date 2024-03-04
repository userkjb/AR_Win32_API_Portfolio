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
		break;
	default:
		break;
	}
}

#include "Level.h"
#include "Actor.h"

ULevel::ULevel()
{
}

ULevel::~ULevel()
{
}

void ULevel::ActorInit(AActor* _NewActor)
{
	_NewActor->SetWorld(this);
	_NewActor->BeginPlay();
}

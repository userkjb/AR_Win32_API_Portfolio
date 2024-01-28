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

void ULevel::LevelTick(float _DeltaTime)
{
	for (std::pair<const int, std::list<AActor*>>& OrderListPair : AllActor)
	{
		std::list<AActor*>& ActorList = OrderListPair.second;
		for (AActor* Actor : ActorList)
		{
			if (false == Actor->IsActive())
			{
				continue;
			}

			Actor->DestroyUpdate(_DeltaTime);
			Actor->Tick(_DeltaTime);
		}
	}
}

#pragma once
#include "NameObject.h"
#include <list>
#include <map>

class AActor;
class EngineCore;

class ULevel : public UNameObject
{
	friend EngineCore;
public :
	// constructer destructer
	ULevel();
	~ULevel();

	// delete Function
	ULevel(const ULevel& _Other) = delete;
	ULevel(ULevel&& _Other) noexcept = delete;
	ULevel& operator=(const ULevel) = delete;
	ULevel& operator=(ULevel&& _Other) noexcept = delete;

	virtual void BeginPlay() {};
	virtual void Tick(float _DeltaTime) {};

	template<typename ActorType>
	ActorType* SpawnActor(int _Order = 0)
	{
		ActorType* NewActor = new ActorType();
		ActorInit(NewActor);
		AllActor[_Order].push_back(NewActor);
		return NewActor;
	}

protected :

private :
	void ActorInit(AActor* _NewActor);
	void LevelTick(float _DeltaTime);

	std::map<int, std::list<AActor*>> AllActor;
};


#pragma once
#include <EngineBase/NameObject.h>
#include <list>
#include <map>

class AActor;
class UEngineCore;

class ULevel : public UNameObject
{
	friend UEngineCore;
	friend UImageRenderer;

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

	/// <summary>
	/// Level �� ������ �� ����.
	/// ���� Level �� ���´�.
	/// </summary>
	/// <param name="_PrevLevel"></param>
	virtual void LevelStart(ULevel* _PrevLevel) {};
	/// <summary>
	/// ���� ����ǰ� �ִ� Level�� ����Ǹ� ����.
	/// </summary>
	/// <param name="_NextLevel"></param>
	virtual void LevelEnd(ULevel* _NextLevel) {};

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
	void LevelRender(float _DeltaTime);
	void LevelRelease(float _DeltaTime);

	std::map<int, std::list<AActor*>> AllActor;
	std::map<int, std::list<UImageRenderer*>> Renderers; // ImageRenderer���� Image���� �־��ش�.
};


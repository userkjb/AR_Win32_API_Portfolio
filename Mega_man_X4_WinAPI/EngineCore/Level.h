#pragma once
#include <EngineBase/NameObject.h>
#include <EngineBase/EngineMath.h> // FVector
#include <list>
#include <map>

class AActor;
class UEngineCore;
class UImageRenderer;
class UCollision;

class ULevel : public UNameObject
{
	friend UEngineCore;
	friend UImageRenderer;
	friend UCollision;

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

	template<typename ActorType, typename EnumType>
	ActorType* SpawnActor(EnumType _Order)
	{
		return SpawnActor<ActorType>(static_cast<int>(_Order));
	}

	template<typename ActorType>
	ActorType* SpawnActor(int _Order = 0)
	{
		ActorType* NewActor = new ActorType();
		ActorInit(NewActor);
		AllActor[_Order].push_back(NewActor);
		return NewActor;
	}

	FVector GetCameraPos()
	{
		return CameraPos;
	}

	void SetCameraPos(FVector _CameraPos)
	{
		CameraPos = _CameraPos;
	}

	void AddCameraPos(FVector _CameraPos)
	{
		CameraPos += _CameraPos;
	}

	/// <summary>
	/// ��� �ð��� �����Ѵ�.
	/// </summary>
	/// <param name="_Scale"></param>
	void SetAllTimeScale(float _Scale)
	{
		for (std::pair<const int, float>& TimeScale : TimeScale)
		{
			TimeScale.second = _Scale;
		}
	}

	// �̰Ŵ� ������ Ÿ�ӽ������� ���� �ؾ߸� �����ϴ�.
	template<typename EnumType>
	void SetOtherTimeScale(EnumType _Value, float _Scale)
	{
		SetOtherTimeScale(static_cast<int>(_Value), _Scale);
	}

	void SetOtherTimeScale(int _Value, float _Scale)
	{
		for (std::pair<const int, float>& TimeScale : TimeScale)
		{
			if (TimeScale.first == _Value)
			{
				continue;
			}

			TimeScale.second = _Scale;
		}
	}

	/// <summary>
	/// 
	/// </summary>
	/// <typeparam name="EnumType"></typeparam>
	/// <param name="_Value"></param>
	/// <param name="_Scale"></param>
	template<typename EnumType>
	void SetTimeScale(EnumType _Value, float _Scale)
	{
		SetTimeScale(static_cast<int>(_Value), _Scale);
	}

	void SetTimeScale(int _Value, float _Scale)
	{
		TimeScale[_Value] = _Scale;
	}

protected :

private :
	void ActorInit(AActor* _NewActor);
	void LevelTick(float _DeltaTime);
	void LevelRender(float _DeltaTime);
	/// <summary>
	/// ������� Actor, Renderer, Collision �� ����� = �޸𸮴� ��� �ִ�. Actor���� ������� �Ѵ�.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void LevelRelease(float _DeltaTime);

	std::map<int, std::list<AActor*>> AllActor;
	std::map<int, std::list<UImageRenderer*>> Renderers; // ImageRenderer���� Image���� �־��ش�.
	std::map<int, std::list<UCollision*>> Collisions;
	std::map<int, float> TimeScale;

	FVector CameraPos = FVector::Zero;
};


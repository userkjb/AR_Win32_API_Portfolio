#pragma once

#include <EngineBase/Transform.h>
#include <EngineBase/NameObject.h>
#include "TickObject.h"
#include "Level.h"
#include "ImageRenderer.h"
#include "Collision.h"

//class UImageRenderer;
class ULevel;
class UActorComponent;
class UImageRenderer;
class UCollision;

class AActor : public UNameObject, public UTickObject
{
	friend ULevel;

public :
	// constructer destructer
	AActor();
	~AActor();

	// delete Function
	AActor(const AActor& _Other) = delete;
	AActor(AActor&& _Other) noexcept = delete;
	AActor& operator=(const AActor) = delete;
	AActor& operator=(AActor&& _Other) noexcept = delete;

	FVector GetActorLocation()
	{
		return Transform.GetPosition();
	}

	void SetActorLocation(FVector _Value)
	{
		Transform.SetPosition(_Value);
	}

	void AddActorLocation(FVector _Value)
	{
		Transform.AddPosition(_Value);
	}

	void SetActorScale(FVector _Value)
	{
		Transform.SetScale(_Value);
	}

	FTransform GetTransform()
	{
		return Transform;
	}

	ULevel* GetWorld()
	{
		return World;
	}

	/// <summary>
	/// Renderer Component ����.
	/// </summary>
	/// <typeparam name="EnumType"></typeparam>
	/// <param name="Order"></param>
	/// <returns></returns>
	template<typename EnumType>
	UImageRenderer* CreateImageRenderer(int _Order = 0)
	{
		return CreateImageRenderer(static_cast<int>(_Order));
	}


	template<typename EnumType>
	UCollision* CreateCollision(EnumType _Order = 0)
	{
		return CreateCollision(static_cast<int>(_Order));
	}

	UImageRenderer* CreateImageRenderer(int _Order = 0);
	UCollision* CreateCollision(int _Order = 0);


	/// <summary>
	/// UTickObject
	/// </summary>
	/// <param name="_DestroyTime"></param>
	void Destroy(float _DestroyTime) override;
	/// <summary>
	/// UTickObject
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void DestroyUpdate(float _DeltaTime) override;

	/// <summary>
	/// Active�� ������ �� �ִ�.
	/// </summary>
	/// <param name="_Active"></param>
	/// <param name="_ActiveTime"></param>
	void SetActive(bool _Active, float _ActiveTime = 0.0f) override;
	/// <summary>
	/// 
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void ActiveUpdate(float _DeltaTime) override;

	/// <summary>
	/// �ڽĵ��� ���������� Ȯ��.(Render, Collision)
	/// </summary>
	void CheckReleaseChild();

	/// <summary>
	/// �ش� Actor�� ���� ��� Render���� Off �Ѵ�.
	/// </summary>
	void AllRenderersActiveOff();
	/// <summary>
	/// �ش� Actor�� ���� ��� Render���� On �Ѵ�.
	/// </summary>
	void AllRenderersActiveOn();

protected :
	/// <summary>
	/// UTickObject
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void Tick(float _DeltaTime) override;

private :
	/// <summary>
	/// Render �� �̹����� ����.
	/// </summary>
	std::list<UImageRenderer*> Renderers;

	/// <summary>
	/// Collision ������ ����.
	/// </summary>
	std::list<UCollision*> Collisions;

	ULevel* World = nullptr;
	FTransform Transform = FTransform();

	void SetWorld(ULevel* _Value)
	{
		World = _Value;
	}
};


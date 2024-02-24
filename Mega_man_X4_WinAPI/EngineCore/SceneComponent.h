#pragma once
#include <EngineBase/Transform.h> // Pose, Scale
#include "ActorComponent.h"

/// <summary>
/// ���� ���� Actor�� �������� �ڽŸ��� ��ġ, ũ�⸦ ���� Component
/// </summary>
class USceneComponent : public UActorComponent
{
public :
	// constrcuter destructer
	USceneComponent();
	~USceneComponent();

	// delete Function
	USceneComponent(const USceneComponent& _Other) = delete;
	USceneComponent(USceneComponent&& _Other) noexcept = delete;
	USceneComponent& operator=(const USceneComponent& _Other) = delete;
	USceneComponent& operator=(USceneComponent&& _Other) noexcept = delete;

	void SetPosition(const FVector& _Value)
	{
		Transform.SetPosition(_Value);
	}

	void SetScale(const FVector& _Value)
	{
		Transform.SetScale(_Value);
	}

	void SetTransform(const FTransform& _Value)
	{
		Transform = _Value;
	}

	FTransform GetTransform()
	{
		return Transform;
	}

	/// <summary>
	/// Position�� �����Ѵ�.
	/// </summary>
	/// <returns></returns>
	FVector GetPosition() const
	{
		return Transform.GetPosition();
	}

	/// <summary>
	/// Actor�� ������� ��ġ,ũ�⸦ �����´�.
	/// </summary>
	/// <returns></returns>
	FTransform GetActorBaseTransform();

private :
	FTransform Transform;
};


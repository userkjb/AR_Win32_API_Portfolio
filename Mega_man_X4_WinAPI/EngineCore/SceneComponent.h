#pragma once
#include <EngineBase/Transform.h> // Pose, Scale
#include "ActorComponent.h"

/// <summary>
/// 내가 속한 Actor를 기준으로 자신만의 위치, 크기를 갖는 Component
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
	/// Position만 리턴한다.
	/// </summary>
	/// <returns></returns>
	FVector GetPosition() const
	{
		return Transform.GetPosition();
	}

	/// <summary>
	/// Actor를 기반으로 위치,크기를 가져온다.
	/// </summary>
	/// <returns></returns>
	FTransform GetActorBaseTransform();

private :
	FTransform Transform;
};


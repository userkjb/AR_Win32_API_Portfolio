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


private :

};


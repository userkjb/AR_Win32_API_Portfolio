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


private :

};


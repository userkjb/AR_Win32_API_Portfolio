#pragma once
#include "SceneComponent.h"

class UCollision : public USceneComponent
{
	// constrcuter destructer
	UCollision();
	~UCollision();

	// delete Function
	UCollision(const UCollision& _Other) = delete;
	UCollision(UCollision&& _Other) noexcept = delete;
	UCollision& operator=(const UCollision& _Other) = delete;
	UCollision& operator=(UCollision&& _Other) noexcept = delete;

	void SetColType(ECollisionType _Type)
	{
		ColType = _Type;
	}

	/// <summary>
	/// ���� ���� Level �� Actor ����.
	/// </summary>
	/// <param name="_Order"></param>
	void SetOrder(int _Order) override;

protected :
	void BeginPlay() override;

private :
	ECollisionType ColType = ECollisionType::Rect;
};
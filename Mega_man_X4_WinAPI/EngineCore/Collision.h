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

	/// <summary>
	/// �پ��� ������ Collision�� üũ�ؾ� �ϱ� ������ template����.
	/// </summary>
	/// <typeparam name="EnumType"></typeparam>
	/// <param name="_Order"></param>
	/// <param name="_Result"></param>
	/// <returns></returns>
	template<typename EnumType>
	bool CollisionCheck(EnumType _Order, std::vector<UCollision*>& _Result)
	{
		return CollisionCheck(static_cast<int>(_Order), _Result);
	}

	bool CollisionCheck(int _Order, std::vector<UCollision*>& _Result);

protected :
	void BeginPlay() override;

private :
	ECollisionType ColType = ECollisionType::Rect;
};
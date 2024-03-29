#pragma once
#include "SceneComponent.h"
#include <vector>

class UCollision : public USceneComponent
{
public :
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
	/// 내가 속한 Level 과 Actor 설정.
	/// </summary>
	/// <param name="_Order"></param>
	void SetOrder(int _Order) override;

	/// <summary>
	/// 다양한 형태의 Collision을 체크해야 하기 때문에 template으로.
	/// </summary>
	/// <typeparam name="EnumType"></typeparam>
	/// <param name="_Order"></param>
	/// <param name="_Result"></param>
	/// <returns></returns>
	template<typename EnumType>
	bool CollisionCheck(EnumType _Order, std::vector<UCollision*>& _Result, FVector _NextPos = FVector::Zero)
	{
		return CollisionCheck(static_cast<int>(_Order), _Result, _NextPos);
	}

	bool CollisionCheck(int _Order, std::vector<UCollision*>& _Result, FVector _NextPos = FVector::Zero);
	void DebugRender(FVector _CameraPos);

protected :
	void BeginPlay() override;

private :
	ECollisionType ColType = ECollisionType::Rect;
};
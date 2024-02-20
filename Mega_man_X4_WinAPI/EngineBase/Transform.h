#pragma once
#include "EngineMath.h"

enum class ECollisionType
{
	Point,
	CirCle,
	Rect,
	Max,
};

/// <summary>
/// 위치, 크기 class
/// </summary>
class FTransform
{
private :
	// Manager를 사용하지 않고 전역으로 사용하기 위한 static
	static bool (*CollisionFunction[static_cast<int>(ECollisionType::Max)][static_cast<int>(ECollisionType::Max)])(const FTransform& _Left, const FTransform& _Right);
public :
	FTransform();
	FTransform(const FVector& _Pos, const FVector& _Scale)
		: Position(_Pos), Scale(_Scale){}
	~FTransform();

	void SetScale(FVector _Value)
	{
		Scale = _Value;
	}
	FVector GetScale() const
	{
		return Scale;
	}
	void SetPosition(FVector _Value)
	{
		Position = _Value;
	}
	void AddPosition(FVector _Value)
	{
		Position += _Value;
	}
	FVector GetPosition()
	{
		return Position;
	}

	float Left() const
	{
		return Position.X - Scale.hX();
	}
	float Right() const
	{
		return Position.X + Scale.hX();
	}
	float Top() const
	{
		return Position.Y - Scale.hY();
	}
	float Bottom() const
	{
		return Position.Y + Scale.hY();
	}

	int iLeft() const
	{
		return static_cast<int>(Left());
	}
	int iRight() const
	{
		return static_cast<int>(Right());
	}
	int iTop() const
	{
		return static_cast<int>(Top());
	}
	int iBottom() const
	{
		return static_cast<int>(Bottom());
	}


	// 충돌 여부. - 위치와 크기를 가지고 있기 때문에
	bool Collision(ECollisionType _ThisType, ECollisionType _OtherType, const FTransform& _Other);

private :
	FVector Position;
	FVector Scale;
};
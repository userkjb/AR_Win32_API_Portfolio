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
/// ��ġ, ũ�� class
/// </summary>
class FTransform
{
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

private :
	FVector Position;
	FVector Scale;
};
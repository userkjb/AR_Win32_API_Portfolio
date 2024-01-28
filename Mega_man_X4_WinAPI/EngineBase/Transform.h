#pragma once
#include "EngineMath.h"

class FTransform
{
public :
	FTransform();
	~FTransform();

	void SetScale(FVector _Value)
	{
		Scale = _Value;
	}
	FVector GetScale()
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

private :
	FVector Scale;
	FVector Position;
};
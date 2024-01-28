#pragma once
#include "EngineMath.h"

class FTransform
{
public :
	FTransform();
	~FTransform();


private :
	FVector Scale;
	FVector Position;
};
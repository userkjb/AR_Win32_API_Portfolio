#pragma once

#include "NameObject.h"
#include "TickObject.h"

class AActor : public UNameObject, public UTickObject
{
public :
	// constructer destructer
	AActor();
	~AActor();

	// delete Function
	AActor(const AActor& _Other) = delete;
	AActor(AActor&& _Other) noexcept = delete;
	AActor& operator=(const AActor) = delete;
	AActor& operator=(AActor&& _Other) noexcept = delete;

protected :

private :

};


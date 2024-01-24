#pragma once
#include "NameObject.h"

class ULevel : public UNameObject
{
public :
	// constructer destructer
	ULevel();
	~ULevel();

	// delete Function
	ULevel(const ULevel& _Other) = delete;
	ULevel(ULevel&& _Other) noexcept = delete;
	ULevel& operator=(const ULevel) = delete;
	ULevel& operator=(ULevel&& _Other) noexcept = delete;

protected :

private :

};


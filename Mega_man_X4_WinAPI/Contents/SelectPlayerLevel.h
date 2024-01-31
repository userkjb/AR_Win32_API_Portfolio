#pragma once
#include <EngineCore/Level.h>

class USelectPlayerLevel : public ULevel
{
public :
	USelectPlayerLevel();
	~USelectPlayerLevel();

	// delete Function
	USelectPlayerLevel(const USelectPlayerLevel& _Other) = delete;
	USelectPlayerLevel(USelectPlayerLevel&& _Other) noexcept = delete;
	USelectPlayerLevel& operator=(const USelectPlayerLevel) = delete;
	USelectPlayerLevel& operator=(USelectPlayerLevel&& _Other) noexcept = delete;

protected :

private :

};


#pragma once
// ´õ¹Ì Level
#include <EngineCore/Level.h>

class UTitleLevel : public ULevel
{
public :
	// constructer destructer
	UTitleLevel();
	~UTitleLevel();

	// delete Function
	UTitleLevel(const UTitleLevel& _Other) = delete;
	UTitleLevel(UTitleLevel&& _Other) noexcept = delete;
	UTitleLevel& operator=(const UTitleLevel) = delete;
	UTitleLevel& operator=(UTitleLevel&& _Other) noexcept = delete;

protected :

private :

};


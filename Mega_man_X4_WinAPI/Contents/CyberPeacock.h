#pragma once
#include <EngineCore/Actor.h>

class ACyberPeacock : public AActor
{
public :
	// constrcuter destructer
	ACyberPeacock();
	~ACyberPeacock();

	// delete Function
	ACyberPeacock(const ACyberPeacock& _Other) = delete;
	ACyberPeacock(ACyberPeacock&& _Other) noexcept = delete;
	ACyberPeacock& operator=(const ACyberPeacock& _Other) = delete;
	ACyberPeacock& operator=(ACyberPeacock&& _Other) noexcept = delete;
};

// Hp = 48


/* Attack
* contact - 6
* Feather Attack - 9
* Rising Slash - 6
* Tracking Shot - 6
*/
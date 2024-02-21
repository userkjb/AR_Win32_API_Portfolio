#pragma once
#include <EngineCore/Actor.h>

/// <summary>
/// 사이버 공작.
/// </summary>
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

protected :

private :
	int Hp = 48;
};


/* Attack
* contact - 6
* Feather Attack - 9
* Rising Slash - 6
* Tracking Shot - 6
*/
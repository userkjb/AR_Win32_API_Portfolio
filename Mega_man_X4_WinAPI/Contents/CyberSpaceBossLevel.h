#pragma once
#include <EngineCore/Level.h>

class ACyberSpaceBossMap;
class AEgseu;
class ACyberPeacock;


class UCyberSpaceBossLevel : public ULevel
{
public :
	// constrcuter destructer
	UCyberSpaceBossLevel();
	~UCyberSpaceBossLevel();

	// delete Function
	UCyberSpaceBossLevel(const UCyberSpaceBossLevel& _Other) = delete;
	UCyberSpaceBossLevel(UCyberSpaceBossLevel&& _Other) noexcept = delete;
	UCyberSpaceBossLevel& operator=(const UCyberSpaceBossLevel& _Other) = delete;
	UCyberSpaceBossLevel& operator=(UCyberSpaceBossLevel&& _Other) noexcept = delete;

protected :
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void LevelStart(ULevel* _Level) override;
	void LevelEnd(ULevel* _Level) override;

private :

	//void MoveCameraVector();

	// Map
	ACyberSpaceBossMap* CyberBossMap = nullptr;

	// Player
	AEgseu* Player = nullptr;

	// Boss
	ACyberPeacock* Boss = nullptr;
};


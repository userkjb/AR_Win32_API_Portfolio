#pragma once
#include <EngineCore/Level.h>

class ACyberSpaceMap;

class UCyberSpaceLevel : public ULevel
{
public :
	// constrcuter destructer
	UCyberSpaceLevel();
	~UCyberSpaceLevel();

	// delete Function
	UCyberSpaceLevel(const UCyberSpaceLevel& _Other) = delete;
	UCyberSpaceLevel(UCyberSpaceLevel&& _Other) noexcept = delete;
	UCyberSpaceLevel& operator=(const UCyberSpaceLevel& _Other) = delete;
	UCyberSpaceLevel& operator=(UCyberSpaceLevel&& _Other) noexcept = delete;

protected :
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void LevelStart(ULevel* _Level) override;
	void LevelEnd(ULevel* _Level) override;

private :
	ACyberSpaceMap* CyberSpaceMap = nullptr;
};


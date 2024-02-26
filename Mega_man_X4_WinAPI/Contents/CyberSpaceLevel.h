#pragma once
#include <EngineCore/Level.h>

class ACyberSpaceMap;
class APlayer;

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
	/// <summary>
	/// 카메라 움직임에 대한 함수.
	/// </summary>
	void MoveCameraVector();

	ACyberSpaceMap* CyberSpaceMap = nullptr;
	APlayer* NewPlayer = nullptr;
};


#pragma once
#include <EngineCore/Level.h>

class ACyberSpaceMap;
//class APlayer;
class AEgseu;
class ATriScan;
class ASpikeMarl;
class AMiruTorearu;

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

	void TestCameraMove();

	// Map
	ACyberSpaceMap* CyberSpaceMap = nullptr;
	//APlayer* NewPlayer = nullptr;

	// Player
	AEgseu* NewX = nullptr;

	// Enemy 
	ATriScan* TryScan_1 = nullptr;
	ATriScan* TryScan_2 = nullptr;
	ASpikeMarl* SpikeMarl = nullptr;
	AMiruTorearu* MiruTorearu_1 = nullptr;
	AMiruTorearu* MiruTorearu_2 = nullptr;
};


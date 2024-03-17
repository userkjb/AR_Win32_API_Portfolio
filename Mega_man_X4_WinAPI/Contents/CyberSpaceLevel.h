#pragma once
#include <EngineCore/Level.h>
#include <EnginePlatform/EngineSound.h>
#include <vector>
#include "ContentsGlobalData.h"

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

	static UEngineSoundPlayer GetSound();

protected :
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void LevelStart(ULevel* _Level) override;
	void LevelEnd(ULevel* _Level) override;

private :
	void StateChange(ECyberStageState _State);
	void StateUpdate(float _DeltaTime);

	
	void None(float _DeltaTime);
	void ReadyStart();
	void Ready(float _DeltaTime);
	void GameProgressStart();
	void GameProgress(float _DeltaTime);


	/// <summary>
	/// 카메라 움직임에 대한 함수.
	/// </summary>
	void MoveCameraVector();

	void EnemySpawn();
	//void EnemySpawn(bool _Test);

	void TestCameraMove();

	// State
	ECyberStageState LevelState = ECyberStageState::None;


	// Time
	float SoundTime = 0.0f;



	// Map
	ACyberSpaceMap* CyberSpaceMap = nullptr;
	//APlayer* NewPlayer = nullptr;

	// Player
	AEgseu* Player = nullptr;


	//UEngineSoundPlayer BGMSound;
	static UEngineSoundPlayer BGMSound;
	UEngineSoundPlayer Ready_Sound;

	ATriScan* TryScan_1 = nullptr;
	ATriScan* TryScan_2 = nullptr;
	ATriScan* TryScan_3 = nullptr;
	ATriScan* TryScan_4 = nullptr;

	//std::vector<AMiruTorearu*> MiruTorearu{ nullptr };

	AMiruTorearu* MiruTorearu_1 = nullptr;
	AMiruTorearu* MiruTorearu_2 = nullptr;
	AMiruTorearu* MiruTorearu_3 = nullptr;
	AMiruTorearu* MiruTorearu_4 = nullptr;
	AMiruTorearu* MiruTorearu_5 = nullptr;
	AMiruTorearu* MiruTorearu_6 = nullptr;
	AMiruTorearu* MiruTorearu_7 = nullptr;
	AMiruTorearu* MiruTorearu_8 = nullptr;
	AMiruTorearu* MiruTorearu_9 = nullptr;
	AMiruTorearu* MiruTorearu_10 = nullptr;

	ASpikeMarl* SpikeMarl = nullptr;

	// ------------------------------------------------

	FVector TryScan_1_Pos = { 1160 , 135 };
	FVector TryScan_2_Pos = { 1160 , 200 };

	FVector MiruTorearu_1_Pos = { 1370, 240 };
	FVector MiruTorearu_2_Pos = { 1700, 500 };
	FVector MiruTorearu_3_Pos = { 1900, 200 };
	FVector MiruTorearu_4_Pos = { 1900, 520 };
	FVector MiruTorearu_5_Pos = { 2100, 370 };
	FVector MiruTorearu_6_Pos = { 2100, 530 };
	FVector MiruTorearu_7_Pos = { 2655, 400 };
	FVector MiruTorearu_8_Pos = { 3250, 400 };
	FVector MiruTorearu_9_Pos = { 3250, 510 };
	FVector MiruTorearu_10_Pos = { 3950, 200 };

	FVector TryScan_3_Pos = { 2155, 450 };
	FVector TryScan_4_Pos = { 2155, 500 };
	
};


#pragma once
#include <EngineCore/Actor.h>
#include "ContentsGlobalData.h"
#include "Egseu.h"

class AEgseu;

class AFeatherMissile : public AActor
{
public :
	// constrcuter destructer
	AFeatherMissile();
	~AFeatherMissile();

	// delete Function
	AFeatherMissile(const AFeatherMissile& _Other) = delete;
	AFeatherMissile(AFeatherMissile&& _Other) noexcept = delete;
	AFeatherMissile& operator=(const AFeatherMissile& _Other) = delete;
	AFeatherMissile& operator=(AFeatherMissile&& _Other) noexcept = delete;

	/// <summary>
	/// 발싸 방향 설정.
	/// </summary>
	/// <param name="_Dir">오른쪽? 왼쪽?</param>
	void SetMissileStartDir(EActorDir _Dir)
	{
		MissileStartDir = _Dir;
	}

protected :
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private :
	void StateChange(ECyberPeacockMissileState _State);
	void StateUpdate(float _DeltaTime);

	void NoneStart();
	void None(float _DeltaTime);
	void CreateStart();
	void Create(float _DeltaTime);
	void Run_UpStart();
	void Run_Up(float _DeltaTime);
	void Run_UpToRightStart();
	void Run_UpToRight(float _DeltaTime);
	void Run_UpToLeftStart();
	void Run_UpToLeft(float _DeltaTime);
	void Run_RightStart();
	void Run_Right(float _DeltaTime);
	void Run_RightToUpStart();
	void Run_RightToUp(float _DeltaTime);
	void Run_RightToDownStart();
	void Run_RightToDown(float _DeltaTime);
	void Run_LeftStart();
	void Run_Left(float _DeltaTime);
	void Run_LeftToUpStart();
	void Run_LeftToUp(float _DeltaTime);
	void Run_LeftToDownStart();
	void Run_LeftToDown(float _DeltaTime);
	void Run_DownStart();
	void Run_Down(float _DeltaTime);
	void Run_DownToRightStart();
	void Run_DownToRight(float _DeltaTime);
	void Run_DownToLeftStart();
	void Run_DownToLeft(float _DeltaTime);
	void DeathStart();
	void Death(float _DeltaTime);

	void CollisionCheck();

	UImageRenderer* MissileRenderer = nullptr;
	UCollision* MissileCollision = nullptr;

	ECyberPeacockMissileState MissileState = ECyberPeacockMissileState::None;
	EActorDir MissileStartDir = EActorDir::Left;

	float LifeTime = 0.0f;
	FVector MissileVector = FVector::Zero;
	const float Speed = 1000.0f;

	AEgseu* Player = nullptr;
};


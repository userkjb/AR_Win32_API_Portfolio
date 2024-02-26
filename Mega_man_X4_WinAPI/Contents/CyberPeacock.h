#pragma once
#include <EngineCore/Actor.h>
#include "ContentsGlobalData.h"

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
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void DirCheck();

	void StateChange(EBossState _State);

	void StateUpdate(float _DeltaTime);

	std::string GetAnimationName(std::string _Name);

	// 상태 시작 함수들
	void IntroStart();
	void ReadyStart();
	void FeatherAttackStart();
	void RisingSlashStart();
	void TrackingShotStart();
	void DeathStart();

	// 상태 Tick 함수들
	void Intro(float _DeltaTime);
	void Ready(float _DeltaTime);
	void FeatherAttack(float _DeltaTime);
	void RisingSlash(float _DeltaTime);
	void TrackingShot(float _DeltaTime);
	void Death(float _DeltaTime);


private :

	void CollisionCheck();

	float BossLifeTime = 0.0f;
	int Hp = 48;

	int FeatherAttackDamage = 9;
	int RisingSlashDamage = 6;
	int TrackingShotDamage = 6;

	UImageRenderer* PeacockRenderer = nullptr;
	UCollision* PeacockCollision = nullptr;

	EActorDir DirState = EActorDir::Left;

	EBossState State = EBossState::None;
	ERenderOrder Order = ERenderOrder::None;
};

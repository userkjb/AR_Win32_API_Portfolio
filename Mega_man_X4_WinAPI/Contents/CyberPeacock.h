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

	void StateChange(ECyberPeacockState _State);

	void StateUpdate(float _DeltaTime);

	std::string GetAnimationName(std::string _Name);

	// 상태 시작 함수들
	void IntroStart();
	void IntroEndStart();
	void DisappearStart();
	void AppearStart();
	void FeatherAttackStart();
	void RisingSlashStart();
	void TrackingShotStart();
	void DeathStart();

	// 상태 Tick 함수들
	void Intro(float _DeltaTime);
	void IntroEnd(float _DeltaTime);
	void Disappear(float _DeltaTime);
	void Appear(float _DeltaTime);
	void FeatherAttack(float _DeltaTime);
	void RisingSlash(float _DeltaTime);
	void TrackingShot(float _DeltaTime);
	void Death(float _DeltaTime);


private :

	void CollisionCheck();

	float BossPatternTime = 0.0f;
	int Hp = 48;
	int HitCount = 0;
	int PatternNumber = 0;

	int BodyDamage = 5;
	int FeatherAttackDamage = 9;
	int RisingSlashDamage = 6;
	int TrackingShotDamage = 6;

	UImageRenderer* PeacockRenderer = nullptr;
	UCollision* PeacockCollision = nullptr;

	EActorDir DirState = EActorDir::Left;

	ECyberPeacockState State = ECyberPeacockState::None;
	ERenderOrder Order = ERenderOrder::None;
};

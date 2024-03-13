#pragma once
#include <EngineCore/Actor.h>
#include "ContentsGlobalData.h"


class AEgseu;
class ACyberSpaceBossMap;

/// <summary>
/// 사이버 공작.
/// </summary>
class ACyberPeacock : public AActor
{
	friend ACyberSpaceBossMap;
public :
	// constrcuter destructer
	ACyberPeacock();
	~ACyberPeacock();

	// delete Function
	ACyberPeacock(const ACyberPeacock& _Other) = delete;
	ACyberPeacock(ACyberPeacock&& _Other) noexcept = delete;
	ACyberPeacock& operator=(const ACyberPeacock& _Other) = delete;
	ACyberPeacock& operator=(ACyberPeacock&& _Other) noexcept = delete;

	static ACyberPeacock* GetMainBoss();

	inline int GetBodyDamage() const
	{
		return BodyDamage;
	}

	/// <summary>
	/// Level에서 접근.
	/// </summary>
	/// <param name="_State"></param>
	void SetStateChange(ECyberPeacockState _State)
	{
		StateChange(_State);
	}

	ECyberPeacockState GetCyberPeacockState() const
	{
		return State;
	}

protected :
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void DirCheck();

	void StateChange(ECyberPeacockState _State);

	void StateUpdate(float _DeltaTime);

	std::string GetAnimationName(std::string _Name);

	// 상태 함수들
	void WaitStart();
	void Wait(float _DeltaTime);
	void IntroStart();
	void Intro(float _DeltaTime);
	void IntroEndStart();
	void IntroEnd(float _DeltaTime);
	void DisappearStart();
	/// <summary>
	/// 사라지다.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void Disappear(float _DeltaTime);
	void AppearStart();
	/// <summary>
	/// 생기다.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void Appear(float _DeltaTime);

	void FeatherAttackStart();
	void FeatherAttack(float _DeltaTime);
	void RisingSlashStart();
	void RisingSlash(float _DeltaTime);
	void TrackingShotStart();
	void TrackingShot(float _DeltaTime);
	void DeathStart();
	void Death(float _DeltaTime);


private :
	// 밖에서 사용할 Player를 static 으로 뺌.
	static ACyberPeacock* MainBoss;

	void CollisionCheck();

	int RandValue = 0;

	std::string CurAnimationName = "None";

	float BossPatternTime = 0.0f;
	int Hp = 48;
	int HitCount = 0;

	int BodyDamage = 5;
	int FeatherAttackDamage = 9;
	int RisingSlashDamage = 6;
	int TrackingShotDamage = 6;

	UImageRenderer* PeacockRenderer = nullptr;
	UCollision* PeacockCollision = nullptr;

	EActorDir DirState = EActorDir::Left;

	ECyberPeacockState State = ECyberPeacockState::None;
	ERenderOrder Order = ERenderOrder::None;

	AEgseu* Player = nullptr;
};

#pragma once
#include <EngineCore/Actor.h>
#include "ContentsGlobalData.h"


class AEgseu;
class ACyberSpaceBossMap;

/// <summary>
/// ���̹� ����.
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
	/// Level���� ����.
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

	// ���� �Լ���
	void WaitStart();
	void Wait(float _DeltaTime);
	void IntroStart();
	void Intro(float _DeltaTime);
	void IntroEndStart();
	void IntroEnd(float _DeltaTime);
	void DisappearStart();
	/// <summary>
	/// �������.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void Disappear(float _DeltaTime);
	void AppearStart();
	/// <summary>
	/// �����.
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
	// �ۿ��� ����� Player�� static ���� ��.
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

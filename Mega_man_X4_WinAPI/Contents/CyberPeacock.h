#pragma once
#include <EngineCore/Actor.h>
#include "ContentsGlobalData.h"


class AEgseu;
class ABuster;
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

	inline int GetFeatherAttackDamage() const
	{
		return FeatherAttackDamage;
	}

	inline int GetRisingSlashDamage() const
	{
		return RisingSlashDamage;
	}

	inline bool GetIntro() const
	{
		return b_Intro;
	}

	inline bool GetBattleReady() const
	{
		return b_BattleReady;
	}

	inline int GetMissileCreateCount() const
	{
		return MissileCreateCount;
	}

	inline int GetCyberPeacockHp() const
	{
		return Hp;
	}

	inline bool GetDeathAni() const
	{
		return b_DeathAni;
	}

	/// <summary>
	/// Level���� ����.
	/// </summary>
	/// <param name="_State"></param>
	void SetStateChange(ECyberPeacockState _State)
	{
		StateChange(_State);
	}

	UImageRenderer* GetTrackingShotScope() const
	{
		return TrackingShotScope;
	}

	ECyberPeacockState GetCyberPeacockState() const
	{
		return State;
	}

protected :
	void BeginPlay() override;
	void Renderer();
	void Tick(float _DeltaTime) override;

	void StateChange(ECyberPeacockState _State);

	void StateUpdate(float _DeltaTime);

	std::string GetAnimationName(std::string _Name);
	std::string GetPlayerOppositeAnimationName(std::string _Name);

	// ���� �Լ���
	void NoneStart();
	void None(float _DeltaTime);
	void WaitStart();
	void Wait(float _DeltaTime);
	void IntroStart();
	void Intro(float _DeltaTime);
	void BattleReadyStart();
	void BattleReady(float _DeltaTime);
	void DisappearStart(); // �����.
	void Disappear(float _DeltaTime); // �����.
	void AppearStart(); // ��Ÿ��.
	void Appear(float _DeltaTime); // ��Ÿ��.

	void FeatherAttackStart();
	void FeatherAttack(float _DeltaTime);
	void RisingSlashStart();
	void RisingSlash(float _DeltaTime);
	void RisingSlash_LoopStart();
	void RisingSlash_Loop(float _DeltaTime);
	void TrackingShotStart();
	void TrackingShot(float _DeltaTime);
	void TrackingShot_LoopStart();
	void TrackingShot_Loop(float _DeltaTime);

	void DeathStart();
	void Death(float _DeltaTime);
	void ExplosionStart();
	void Explosion(float _DeltaTime);
	void EndStart();
	void End(float _DeltaTime);


private :
	// �ۿ��� ����� Player�� static ���� ��.
	static ACyberPeacock* MainBoss;

	void CollisionCheck();

	void CreateMissile(int _Count);

	void CalCollision();
	//void HpCheck();
	////////////////////////////////////////////

	bool b_Intro = false; // ��Ʈ�� �ִϸ��̼� ����.
	bool b_BattleReady = false; // ��Ī~! �ִϸ��̼� ����.
	bool b_DeathAni = false;
	bool b_BusterCol = false;

	int RandValue = 0; // ���� ���� ���� ����.

	std::string CurAnimationName = "None";

	float BossPatternTime = 0.0f;
	float DeathTime = 0.0f;
	float AppearTime = 0.0f;
	int Hp = 48;	
	int MissileCreateCount = 0; // ~7
	int BodyDamage = 5;
	int FeatherAttackDamage = 9;
	int RisingSlashDamage = 6;

	FVector RisingSlashVector = FVector::Zero;
	const float RisingSlashSpeed = 500.0f;
	float RisingSlashTargetPos = 0.0f;
	//FVector MissileStartPos = FVector::Zero;

	UImageRenderer* PeacockRenderer = nullptr; // ����
	UCollision* PeacockCollision = nullptr; // ���� ���� Collision
	UImageRenderer* TrackingShotScope = nullptr; // ������
	UCollision* FeatherAttackCollision = nullptr; // ���� ��ų Collision
	UCollision* RisingSlashCollision = nullptr; // ���� ��ų Collision
	

	UImageRenderer* TestImage = nullptr;
	FVector BossImageCenterPos = { 6.0f, -117.0f };
	bool b_test = false;
	void TestFunction(bool _test);
	std::vector<FVector> MissileCount;
	float MissileFireDelay = 0.0f;
	//FVector Count_0 = { 70, 0 };
	//FVector Count_1 = { 71, -42  };
	//FVector Count_2 = { 52, -71  };
	//FVector Count_3 = { 22, -86  };
	//FVector Count_4 = { -32, -86 };
	//FVector Count_5 = { -69, -75 };
	//FVector Count_6 = { -89, -40 };
	//FVector Count_7 = { -94, 2  };

	EActorDir CyberPeacockDir = EActorDir::Left;
	EActorDir TrackingShotDir = EActorDir::Left; // TrackingShot ������ �� ����

	ECyberPeacockState State = ECyberPeacockState::None;
	ECyberPeacockState PreState = ECyberPeacockState::None;
	ERenderOrder Order = ERenderOrder::None;

	AEgseu* Player = nullptr;
	ABuster* PrevBuster = nullptr;
	ABuster* UsingBuster = nullptr;
};
// X : 1838 ~ 2042
// 
// 1838 ~ 1940 || 1940 ~ 2042

// 1917 ~ 2681
//  382    382
// 2299   2299
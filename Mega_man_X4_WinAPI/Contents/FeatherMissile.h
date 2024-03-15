#pragma once
#include <EngineCore/Actor.h>
#include "ContentsGlobalData.h"
//#include "Egseu.h"

class AEgseu;
class ACyberPeacock;

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
	//void SetMissileStartDir(EActorDir _Dir)
	//{
	//	MissileStartDir = _Dir;
	//}

	void SetMissileState(ECyberPeacockMissileState _State)
	{
		MissileState = _State;
	}

	inline int GetDamage() const
	{
		return Damage;
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
	void RunStart();
	void Run(float _DeltaTime);
	void DeathStart();
	void Death(float _DeltaTime);

	void CollisionCheck();

	UImageRenderer* MissileRenderer = nullptr;
	UCollision* MissileCollision = nullptr;

	ECyberPeacockMissileState MissileState = ECyberPeacockMissileState::None;
	//EActorDir MissileStartDir = EActorDir::Left;

	float LifeTime = 0.0f;
	FVector MissileVector = FVector::Zero;
	const float Speed = 500.0f;
	float MissileStartDelay = 0.0f;

	int Damage = 6;

	AEgseu* Player = nullptr;
	ACyberPeacock* Boss = nullptr;
};


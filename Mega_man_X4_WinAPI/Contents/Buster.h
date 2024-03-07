#pragma once
#include <EngineCore/Actor.h>
#include "ContentsGlobalData.h"


class ABuster : public AActor
{
public :
	// constrcuter destructer
	ABuster();
	~ABuster();

	// delete Function
	ABuster(const ABuster& _Other) = delete;
	ABuster(ABuster&& _Other) noexcept = delete;
	ABuster& operator=(const ABuster& _Other) = delete;
	ABuster& operator=(ABuster&& _Other) noexcept = delete;

	
	void SetDirState(EActorDir _Dir)
	{
		DirState = _Dir;
	}

	void SetBusterState(EBusterState _State)
	{
		E_BusterState = _State;
	}

	EBusterState GetBusterState() const
	{
		return E_BusterState;
	}

	//void SetBusterAnimation(std::string_view _Name);

	// Level 에서 대미지 변경 해야 함.
	int GetDefaultBusterDamage()
	{
		return DefaultBusterDamage;
	}
	void SetDefaultBusterDamage(int _Damage)
	{
		DefaultBusterDamage = _Damage;
	}
	void SetMiddleBusterDamage(int _Damage)
	{
		MiddleBusterDamage = _Damage;
	}
	int GetMiddleBusterDamage()
	{
		return MiddleBusterDamage;
	}
	void SetPullBusterDamage(int _Damage)
	{
		PullBusterDamage = _Damage;
	}
	int GetPullBusterDamage()
	{
		return PullBusterDamage;
	}
	
protected :
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void StateChange(EBusterState _State);

	void StateUpdate(float _DeltaTime);

	// Buster 상태.
	EBusterState E_BusterState = EBusterState::CreateBuster;

	// Render 순서.
	ERenderOrder Order = ERenderOrder::None;	

private :

	//FVector Dir = FVector::Zero;
	//AActor* Buster = this;
	EActorDir DirState = EActorDir::Right;
	
	std::string GetRorL() const;
	void DefaultBegin(float _DeltaTime);
	void MiddleBegin(float _DeltaTime);
	void PullBegin(float _DeltaTime);

	void DefaultChargeStart();
	void DefaultBuster(float _DeltaTime);
	void MiddleChargeStart();
	void MiddleCharge(float _DeltaTime);
	void PullChargeStart();
	void PullCharge(float _DeltaTime);
	void BusterCrashStart();
	void BusterCrash(float _DeltaTime);
	void BusterEndStart();
	void BusterEnd(float _DeltaTime);


	//=============================================

	void CollisionCheck();

	UImageRenderer* Renderer = nullptr;
	UCollision* BusterCollision = nullptr;
	UImageRenderer* DeleteBusterRender = nullptr;
	const float BusterSpeed = 500.0f; // 발사 속도.
	FVector BusterVector = FVector::Zero;
	FVector LastMoveVector = FVector::Zero;
	int DefaultBusterDamage = 1; // Buster 공격력.
	int MiddleBusterDamage = 2;
	int PullBusterDamage = 3;

	float BusterLifeTime = 0.0f; // Buster 생명 주기.
	const float BusterLife = 2.0f;
};


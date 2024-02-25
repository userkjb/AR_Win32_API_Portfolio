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

	
	void SetDir(FVector _Dir)
	{
		Dir = _Dir;
	}

	void SetBusterState(EBusterState _State)
	{
		E_BusterState = _State;
	}

	void SetBusterAnimation(std::string_view _Name);

protected :
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void StateChange(EBusterState _State);

	void StateUpdate(float _DeltaTime);

	// 상태 시작 함수. Begin
	void BusterCrashStart();
	void BusterEndStart();


	// Buster 상태.
	EBusterState E_BusterState = EBusterState::CreateBuster;

	// Render 순서.
	ERenderOrder Order = ERenderOrder::None;	

	// Level 에서 대미지 변경 해야 함.
	void SetDefaultBusterDamage(int _Damage)
	{
		DefaultBusterDamage = _Damage;
	}
	void SetMiddleBusterDamage(int _Damage)
	{
		MiddleBusterDamage = _Damage;
	}
	void SetPullBusterDamage(int _Damage)
	{
		PullBusterDamage = _Damage;
	}



private :

	FVector Dir = FVector::Zero;

	// 상태 진행 함수.
	void DefaultBuster(float _DeltaTime);
	void MiddleCharge(float _DeltaTime);
	void PullCharge(float _DeltaTime);
	void BusterCrash(float _DeltaTime);
	void BusterEnd(float _DeltaTime);

	//=============================================

	UImageRenderer* Renderer = nullptr;
	UCollision* BusterCollision = nullptr;
	const float BusterSpeed = 0.0075f; // 발사 속도.
	int DefaultBusterDamage = 1; // Buster 공격력.
	int MiddleBusterDamage = 2;
	int PullBusterDamage = 3;

	float BusterLifeTime = 0.0f; // Buster 생명 주기.
	const float BusterLife = 2.0f;
};


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
		BusterState = _State;
	}

protected :
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void StateChange(EBusterState _State);

	void StateUpdate(float _DeltaTime);

	// 상태 시작 함수. Begin
	void DefaultBusterStart();
	void MiddleChargeStart();
	void PullchargeStart();
	void BusterCrashStart();
	void BusterEndStart();


	// Buster 상태.
	EBusterState BusterState = EBusterState::DefaultCharge;

	// Render 순서.
	ERenderOrder Order = ERenderOrder::None;

private :

	// 상태 진행 함수.
	void DefaultBuster(float _DeltaTime);
	void MiddleCharge(float _DeltaTime);
	void PullCharge(float _DeltaTime);
	void BusterCrash(float _DeltaTime);
	void BusterEnd(float _DeltaTime);



	//=============================================

	UImageRenderer* Renderer = nullptr;
	UCollision* BusterCollision = nullptr;
	FVector Dir = FVector::Zero; // 날아가는 방향
	const float BusterSpeed = 1.0f;	 // 발사 속도

	float BusterLifeTime = 0.0f;
};


#pragma once
#include <EngineCore/Actor.h>
#include "ContentsGlobalData.h"

class ABuster;

class AEgseu : public AActor
{
public :
	// constrcuter destructer
	AEgseu();
	~AEgseu();

	// delete Function
	AEgseu(const AEgseu& _Other) = delete;
	AEgseu(AEgseu&& _Other) noexcept = delete;
	AEgseu& operator=(const AEgseu& _Other) = delete;
	AEgseu& operator=(AEgseu&& _Other) noexcept = delete;

	static AEgseu* GetMainPlayer();

	EActorDir GetActorDir() const
	{
		return DirState;
	}

protected :
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private :
	void DirCheck();
	std::string GetAnimationName(std::string _Name);
	void StateChange(EEgseuState _State);
	void StateUpdate(float _DeltaTime);

	// Start

	// Tick

	EEgseuState State = EEgseuState::None;
	ERenderOrder Order = ERenderOrder::None;
	EActorDir DirState = EActorDir::Right;

	std::string CurAnimationName = "None";

	UImageRenderer* Renderer = nullptr;
	UCollision* PlayerCollision = nullptr;
	UImageRenderer* Busterflash = nullptr; // 버스터 총구 섬광.
	UImageRenderer* MiddleChargeRender = nullptr; // 중간 차지
	UImageRenderer* PullChargeRender = nullptr; // 풀 차지

	// Vector
	FVector RunVector = FVector::Zero;
	FVector DashVector = FVector::Zero;
	// 중력 가속도(500.0f)
	FVector GravityAcc = FVector::Down * 1000.0f;
	// 중력
	FVector GravityVector = FVector::Zero;
	// 점프 파워
	FVector JumpPower = FVector::Up * 450.0f;
	FVector JumpVector = FVector::Zero;
	// 소환 속도
	FVector SummonVector = FVector::Zero;
	// 모든 Vector들을 함한 Vector
	FVector LastMoveVector = FVector::Zero;

	//
	static AEgseu* MainPlayer;

	// 
	float MoveSpeed = 500.0f;
	float DashSpeed = 2.0f;
	float DashTime = 0.0f;
	float ChargTime = 0.0f;
	float AttackTime = 0.0f;
	float DelayTime = 0.0f;
	float ReadToBusterTime = 0.0f; // Buster 준비 시간.
	bool IsBuster = false; // Buster 쐈음?

	int TickCount = 0; // Buster가 1번 생성되도록.

	int MaxHp = 0;
	int Hp = 0;
};


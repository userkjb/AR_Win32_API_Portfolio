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
	UImageRenderer* Busterflash = nullptr; // ������ �ѱ� ����.
	UImageRenderer* MiddleChargeRender = nullptr; // �߰� ����
	UImageRenderer* PullChargeRender = nullptr; // Ǯ ����

	// Vector
	FVector RunVector = FVector::Zero;
	FVector DashVector = FVector::Zero;
	// �߷� ���ӵ�(500.0f)
	FVector GravityAcc = FVector::Down * 1000.0f;
	// �߷�
	FVector GravityVector = FVector::Zero;
	// ���� �Ŀ�
	FVector JumpPower = FVector::Up * 450.0f;
	FVector JumpVector = FVector::Zero;
	// ��ȯ �ӵ�
	FVector SummonVector = FVector::Zero;
	// ��� Vector���� ���� Vector
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
	float ReadToBusterTime = 0.0f; // Buster �غ� �ð�.
	bool IsBuster = false; // Buster ����?

	int TickCount = 0; // Buster�� 1�� �����ǵ���.

	int MaxHp = 0;
	int Hp = 0;
};


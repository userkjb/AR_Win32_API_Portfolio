#pragma once
#include <EngineCore/Actor.h>
#include "ContentsGlobalData.h"

class APlayer : public AActor
{
public:
	// constrcuter destructer
	APlayer();
	~APlayer();

	// delete Function
	APlayer(const APlayer& _Other) = delete;
	APlayer(APlayer&& _Other) noexcept = delete;
	APlayer& operator=(const APlayer& _Other) = delete;
	APlayer& operator=(APlayer&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
		
	// ������ ���� �߿��ϹǷ� ���� ����.

	/// <summary>
	/// <para>Player(Actor)�� ������ �����Ѵ�.</para>
	/// <para>������ �ٸ��� �ش� ���⿡ �´� Animation���� �ٲ۴�.</para>
	/// </summary>
	void DirCheck();

	/// <summary>
	/// ���� �������� Animation�� �̸��� �����´�.
	/// </summary>
	/// <param name="_Name"></param>
	/// <returns></returns>
	std::string GetAnimationName(std::string _Name);

	/// <summary>
	/// Player ���� ���� �Լ�.
	/// </summary>
	/// <param name="_State"></param>
	void StateChange(EPlayerState _State);
	/// <summary>
	/// ���¿� ���� Update(Tick)
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void StateUpdate(float _DeltaTime);

	// ���¿� ���� ���� �Լ���
	void IdleStart();
	void RunStart();
	void JumpStart();
	void SkyStart();
	void JumpEndStart();
	void IdleAttackStart();
	void IdleAttackWaitStart();
	void IdleAttackEndStart();

	// ���� �Լ���
	
	/// <summary>
	/// ������ ���� ��� ó�� �Լ�.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void Idle(float _DeltaTime);

	/// <summary>
	/// �����̰� ���� �� ó�� �Լ�.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void Run(float _DeltaTime);

	/// <summary>
	/// ���� ��Ȳ�� �� ó�� �Լ�
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void Jump(float _DeltaTime);

	/// <summary>
	/// ���� ������ ��� ó�� �Լ�.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void Sky(float _DeltaTime);

	/// <summary>
	/// ���� ó�� �Լ�.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void JumpEnd(float _DeltaTime);

	/// <summary>
	/// ���� �Լ�.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void IdleAttack(float _DeltaTime);

	/// <summary>
	/// ���� �� ��� �Լ�.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void IdleAttackWait(float _DeltaTime);

	/// <summary>
	/// ���� ���� �Լ�.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void IdleAttackEnd(float _DeltaTime);

	// ==========================================

	// Player �⺻ ����.
	EPlayerState State = EPlayerState::None;
	// Player ���� == Actor ����
	EActorDir DirState = EActorDir::Right;
	// ���� �ִϸ��̼��� �̸�. -> �ִϸ��̼� �̸��� �Ű� ��� ��.
	std::string CurAnimationName = "None";

private :

	/// <summary>
	/// Player�� �����̴� �Լ�.(����� �ٸ� ������.)
	/// </summary>
	/// <param name="_DirDelta"></param>
	void AddMoveVector(const FVector& _DirDelta);

	// === �̵� �Լ��� ===

	/// <summary>
	/// ��� Vector���� ���Ѵ�.
	/// </summary>
	void CalLastMoveVector();

	/// <summary>
	/// �̵� ��� �Լ�.
	/// </summary>
	void CalMoveVector();

	/// <summary>
	/// �߷� ��� �Լ�.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void CalGravityVector(float _DeltaTime);

	/// <summary>
	/// ������ Vector(LastMoveVector)�� �̿��ؼ� ī�޶�� ĳ���͸� �����δ�.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void MoveLastMoveVector(float _DeltaTime);

	/// <summary>
	/// �� �̵� �Լ����� ��� ���� �Լ�.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void MoveUpdate(float _DeltaTime);

	// ==========================================

	UImageRenderer* Renderer = nullptr;

	// ���� �ӷ�
	FVector MoveVector = FVector::Zero;	

	// �߷� ���ӵ�(500.0f)
	FVector GravityAcc = FVector::Down * 1000.0f;
	// �߷�
	FVector GravityVector = FVector::Zero;

	// ���� �Ŀ�
	FVector JumpPower = FVector::Up * 450.0f;
	FVector JumpVector = FVector::Zero;

	// ��� Vector���� ���� Vector
	FVector LastMoveVector = FVector::Zero;

	// test ==========================================

	// 100
	float MoveSpeed = 500.0f;
};
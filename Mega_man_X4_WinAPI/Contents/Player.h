#pragma once
#include <EngineCore/Actor.h>
#include "ContentsGlobalData.h"

class ABuster;

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
	void SummonStart();
	void SummonLoopStart();
	void SummonEndStart();
	void IdleStart();
	void RunStart();
	void RunAndAttackStart();
	void RunAndChargeStart();
	void JumpStart();
	void SkyStart();
	void JumpEndStart();
	void AttackStart();
	void AttackWaitStart();
	void AttackEndStart();
	void DashStart();
	void DashLoopStart();
	void DashEndStart();

	// ���� �Լ���

	/// <summary>
	/// ��ȯ ���� �Լ�.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void Summon(float _DeltaTime);

	/// <summary>
	/// ��ȯ ���� ���� ���� �Լ�.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void SummonLoop(float _DeltaTime);

	/// <summary>
	/// ��ȯ ���� ���� �Լ�.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void SummonEnd(float _DeltaTime);
	
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

	void RunAndAttack(float _DeltaTime);

	void RunAndCharge(float _DeltaTime);

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
	void Attack(float _DeltaTime);

	/// <summary>
	/// ���� �� ��� �Լ�.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void AttackWait(float _DeltaTime);

	/// <summary>
	/// ���� ���� �Լ�.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void AttackEnd(float _DeltaTime);

	/// <summary>
	/// DastStart Function
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void Dash(float _DeltaTime);

	/// <summary>
	/// �뽬 ���� ���� �Լ�
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void DashLoop(float _DeltaTime);

	/// <summary>
	/// �뽬 ���� ���� �Լ�.(���� �־�� ��)
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void DashEnd(float _DeltaTime);


	// ==========================================

	// Player �⺻ ����.
	EPlayerState State = EPlayerState::None;
	// Renderer ����.
	ERenderOrder Order = ERenderOrder::None;

	// Player ���� == Actor ����
	EActorDir DirState = EActorDir::Right;
	// ���� �ִϸ��̼��� �̸�. -> �ִϸ��̼� �̸��� �Ű� ��� ��.
	std::string CurAnimationName = "None";

private :

	/// <summary>
	/// ���� BeginPlay �Լ��� �ʹ� ������� Charge(����) ���� �������� ���� ��.
	/// </summary>
	void ChargeBeginPlay();

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

	/// <summary>
	/// ī�޶� �����ӿ� ���� �Լ�.
	/// </summary>
	void MoveCameraVector();

	// ==========================================

	UImageRenderer* Renderer = nullptr;
	UImageRenderer* Busterflash = nullptr; // ������ �ѱ� ����.
	UImageRenderer* MiddleChargeRender = nullptr;
	UImageRenderer* PullChargeRender = nullptr;

	// Run Vector
	FVector RunVector = FVector::Zero;

	// �뽬
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

	// Camera Vector
	FVector CameraCenterPos = { 400, 300 };

	// test ==========================================
	
	float MoveSpeed = 500.0f;
	float DashSpeed = 2.0f;
	float DashTime = 0.0f;
	float ChargTime = 0.0f;
	float AttackTime = 0.0f;
	float DelayTime = 0.0f;
	int TickCount = 0;
};

/*
* Data
* https://www.deviantart.com/sesshowmall/art/Mega-Man-X4-Height-Chart-781104089
*/
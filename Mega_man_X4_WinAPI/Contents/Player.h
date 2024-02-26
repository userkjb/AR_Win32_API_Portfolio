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
		
	// 방향은 가장 중요하므로 가장 위에.

	/// <summary>
	/// <para>Player(Actor)의 방향을 설정한다.</para>
	/// <para>방향이 다르면 해당 방향에 맞는 Animation으로 바꾼다.</para>
	/// </summary>
	void DirCheck();

	/// <summary>
	/// 현재 실행중인 Animation의 이름을 가져온다.
	/// </summary>
	/// <param name="_Name"></param>
	/// <returns></returns>
	std::string GetAnimationName(std::string _Name);

	/// <summary>
	/// Player 상태 설정 함수.
	/// </summary>
	/// <param name="_State"></param>
	void StateChange(EPlayerState _State);

	/// <summary>
	/// 상태에 따른 Update(Tick)
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void StateUpdate(float _DeltaTime);

	// 상태에 대한 시작 함수들
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

	// 상태 함수들

	/// <summary>
	/// 소환 상태 함수.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void Summon(float _DeltaTime);

	/// <summary>
	/// 소환 진행 중인 상태 함수.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void SummonLoop(float _DeltaTime);

	/// <summary>
	/// 소환 종료 상태 함수.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void SummonEnd(float _DeltaTime);
	
	/// <summary>
	/// 가만히 있을 경우 처리 함수.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void Idle(float _DeltaTime);

	/// <summary>
	/// 움직이고 있을 때 처리 함수.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void Run(float _DeltaTime);

	void RunAndAttack(float _DeltaTime);

	void RunAndCharge(float _DeltaTime);

	/// <summary>
	/// 점프 상황일 때 처리 함수
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void Jump(float _DeltaTime);

	/// <summary>
	/// 공중 상태인 경우 처리 함수.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void Sky(float _DeltaTime);

	/// <summary>
	/// 착지 처리 함수.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void JumpEnd(float _DeltaTime);

	/// <summary>
	/// 공격 함수.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void Attack(float _DeltaTime);

	/// <summary>
	/// 공격 후 대기 함수.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void AttackWait(float _DeltaTime);

	/// <summary>
	/// 공격 종료 함수.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void AttackEnd(float _DeltaTime);

	/// <summary>
	/// DastStart Function
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void Dash(float _DeltaTime);

	/// <summary>
	/// 대쉬 중인 상태 함수
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void DashLoop(float _DeltaTime);

	/// <summary>
	/// 대쉬 종료 상태 함수.(감속 있어야 함)
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void DashEnd(float _DeltaTime);


	// ==========================================

	// Player 기본 상태.
	EPlayerState State = EPlayerState::None;
	// Renderer 순서.
	ERenderOrder Order = ERenderOrder::None;

	// Player 방향 == Actor 방향
	EActorDir DirState = EActorDir::Right;
	// 현재 애니메이션의 이름. -> 애니메이션 이름도 신경 써야 함.
	std::string CurAnimationName = "None";

private :

	/// <summary>
	/// 기존 BeginPlay 함수가 너무 길어져서 Charge(차지) 관련 설정들을 따로 뺌.
	/// </summary>
	void ChargeBeginPlay();

	/// <summary>
	/// Player를 움직이는 함수.(계산은 다른 곳에서.)
	/// </summary>
	/// <param name="_DirDelta"></param>
	void AddMoveVector(const FVector& _DirDelta);

	// === 이동 함수들 ===

	/// <summary>
	/// 모든 Vector들을 합한다.
	/// </summary>
	void CalLastMoveVector();

	/// <summary>
	/// 이동 계산 함수.
	/// </summary>
	void CalMoveVector();

	/// <summary>
	/// 중력 계산 함수.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void CalGravityVector(float _DeltaTime);

	/// <summary>
	/// 합쳐진 Vector(LastMoveVector)를 이용해서 카메라와 캐릭터를 움직인다.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void MoveLastMoveVector(float _DeltaTime);

	/// <summary>
	/// 위 이동 함수들을 모아 놓는 함수.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void MoveUpdate(float _DeltaTime);

	/// <summary>
	/// 카메라 움직임에 대한 함수.
	/// </summary>
	void MoveCameraVector();

	// ==========================================

	UImageRenderer* Renderer = nullptr;
	UImageRenderer* Busterflash = nullptr; // 버스터 총구 섬광.
	UImageRenderer* MiddleChargeRender = nullptr;
	UImageRenderer* PullChargeRender = nullptr;

	// Run Vector
	FVector RunVector = FVector::Zero;

	// 대쉬
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
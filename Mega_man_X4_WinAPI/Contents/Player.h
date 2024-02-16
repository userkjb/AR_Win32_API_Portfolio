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
	void IdleStart();
	void RunStart();
	void JumpStart();

	// 상태 함수들
	
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
	/// <summary>
	/// 점프 상황일 때 처리 함수
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void Jump(float _DeltaTime);

	// ==========================================

	// Player 기본 상태.
	EPlayerState State = EPlayerState::None;
	// Player 방향 == Actor 방향
	EActorDir DirState = EActorDir::Right;
	// 현재 애니메이션의 이름. -> 애니메이션 이름도 신경 써야 함.
	std::string CurAnimationName = "None";

private :

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
	/// 이동 구현 함수.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void MoveLastMoveVector(float _DeltaTime);

	/// <summary>
	/// 위 이동 함수들을 모아 놓는 함수.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void MoveUpdate(float _DeltaTime);

	// ==========================================

	UImageRenderer* Renderer = nullptr;
	FVector MoveVector = FVector::Zero; // 최종 속력
	// 모든 방향의 합.
	FVector LastMoveVector = FVector::Zero; // 모든 Vector들을 함한 Vector

	// test ==========================================

	// 100
	float MoveSpeed = 500.0f;
};
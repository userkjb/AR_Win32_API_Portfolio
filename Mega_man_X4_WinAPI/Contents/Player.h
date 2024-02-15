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

	// Player 기본 상태.
	EPlayerState State = EPlayerState::None;

	// 방향은 가장 중요하므로 가장 위에.

	/// <summary>
	/// Player 의 방향을 체크한다.
	/// </summary>
	void DirCheck();




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

private :
	UImageRenderer* Renderer = nullptr;
};
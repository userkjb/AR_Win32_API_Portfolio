#pragma once
#include <EngineCore/Actor.h>
#include "ContentsGlobalData.h"

class APlayerFocus : public AActor
{
public:
	// constrcuter destructer
	APlayerFocus();
	~APlayerFocus();

	// delete Function
	APlayerFocus(const APlayerFocus& _Other) = delete;
	APlayerFocus(APlayerFocus&& _Other) noexcept = delete;
	APlayerFocus& operator=(const APlayerFocus& _Other) = delete;
	APlayerFocus& operator=(APlayerFocus&& _Other) noexcept = delete;

	/// <summary>
	/// 포커스 상태 설정.
	/// </summary>
	/// <param name="_State"></param>
	inline void SetFocusState(EFocusState _State)
	{
		State = _State;
	}

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private :
	void ForcusComponent();
	void CreateAnimations();

	void StateChange(EFocusState _State);
	void StateUpdate(float _DeltaTime);

	//void NoneStart();
	//void None(float _DeltaTime);
	//void CallCreateStart();
	void CallCreate(float _DeltaTime);
	void CreateStart();
	void Create(float _DeltaTime);
	void CreateLoopStart();
	void CreateLoop(float _DeltaTime);
	void RunStart();
	void Run(float _DeltaTime);
	void Rank(float _DeltaTime);
	void RunUpStart();
	void RunUp(float _DeltaTime);
	void RunRightStart();
	void RunRight(float _DeltaTime);
	void DeathStart();
	void Death(float _DeltaTime);

	EFocusState State = EFocusState::None;

	UImageRenderer* FocusRender = nullptr;
	UImageRenderer* FocusArrow = nullptr;

	//UImageRenderer* RightUpRender = nullptr;
	//UImageRenderer* LeftUpRender = nullptr;
	//UImageRenderer* LeftDownRender = nullptr;

	UImageRenderer* HurryUpRender = nullptr;

	FVector RunVector = FVector::Zero;
	float RunSpeed = 500.0f;
	int UpTargetLen = 150;
	int UpEndPos = 0;

	float CreateDelay = 0.0f;
	float ForcusTime = 0.0f;
};


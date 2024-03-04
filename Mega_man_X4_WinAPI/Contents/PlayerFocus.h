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

	inline void SetFocusState(EFocusState _State)
	{
		State = _State;
	}

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private :
	void ForcusComponent();

	void StateChange(EFocusState _State);
	void StateUpdate(float _DeltaTime);

	EFocusState State = EFocusState::None;

	UImageRenderer* RightUpRender = nullptr;
	UImageRenderer* LeftUpRender = nullptr;
	UImageRenderer* LeftDownRender = nullptr;
	UImageRenderer* HurryUpRender = nullptr;
	UImageRenderer* FocusRender = nullptr;

	float ForcusTime = 0.0f;
};


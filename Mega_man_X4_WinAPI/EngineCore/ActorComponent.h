#pragma once
#include "TickObject.h"

class AActor;

// Actor에 대한 기능을 담당하는 Component
class UActorComponent : public UTickObject
{
	friend AActor;

public:
	// constrcuter destructer
	UActorComponent();
	~UActorComponent();

	// delete Function
	UActorComponent(const UActorComponent& _Other) = delete;
	UActorComponent(UActorComponent&& _Other) noexcept = delete;
	UActorComponent& operator=(const UActorComponent& _Other) = delete;
	UActorComponent& operator=(UActorComponent&& _Other) noexcept = delete;

	// 어느 Actor에 속한 Component인지 확인.
	AActor* GetOwner()
	{
		return Owner;
	}

protected :

private :
	// 내가 속한 Actor를 받음.
	AActor* Owner = nullptr;

	// 해당 Component가 속한 Actor를 설정.
	void SetOwner(AActor* _Owner)
	{
		Owner = _Owner;
	}
};


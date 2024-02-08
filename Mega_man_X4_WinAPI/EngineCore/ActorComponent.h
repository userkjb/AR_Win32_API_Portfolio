#pragma once
#include "TickObject.h"

class AActor;

// Actor�� ���� ����� ����ϴ� Component
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

	// ��� Actor�� ���� Component���� Ȯ��.
	AActor* GetOwner()
	{
		return Owner;
	}

protected :

private :
	// ���� ���� Actor�� ����.
	AActor* Owner = nullptr;

	// �ش� Component�� ���� Actor�� ����.
	void SetOwner(AActor* _Owner)
	{
		Owner = _Owner;
	}
};


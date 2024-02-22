#pragma once
#include <EngineCore/Actor.h>
#include "ContentsGlobalData.h"


class ABuster : public AActor
{
public :
	// constrcuter destructer
	ABuster();
	~ABuster();

	// delete Function
	ABuster(const ABuster& _Other) = delete;
	ABuster(ABuster&& _Other) noexcept = delete;
	ABuster& operator=(const ABuster& _Other) = delete;
	ABuster& operator=(ABuster&& _Other) noexcept = delete;

	void SetDir(FVector _Dir)
	{
		Dir = _Dir;
	}

protected :
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void StateChange(EBusterState _State);

	void StateUpdate(float _DeltaTime);

	// ���� ���� �Լ�.
	void DefaultBusterStart();


	// Buster ����.
	EBusterState State = EBusterState::CreateBuster;

	// Render ����.
	ERenderOrder Order = ERenderOrder::None;

private :
	FVector Dir = FVector::Zero; // ���ư��� ����
	float BusterSpeed = 1.0f;	 // �߻� �ӵ�
	UCollision* BusterCollision = nullptr;
};


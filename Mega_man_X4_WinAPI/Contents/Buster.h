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

	

	void SetBusterState(EBusterState _State)
	{
		BusterState = _State;
	}

protected :
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	std::string GetAnimationName(std::string _Name);

	void StateChange(EBusterState _State);

	void StateUpdate(float _DeltaTime);

	// ���� ���� �Լ�. Begin
	void DefaultBusterStart();
	void MiddleChargeStart();
	void PullchargeStart();
	void BusterCrashStart();
	void BusterEndStart();


	// Buster ����.
	EBusterState BusterState = EBusterState::DefaultCharge;

	// Render ����.
	ERenderOrder Order = ERenderOrder::None;

	// Buster ����.

private :

	// ���� ���� �Լ�.
	void DefaultBuster(float _DeltaTime);
	void MiddleCharge(float _DeltaTime);
	void PullCharge(float _DeltaTime);
	void BusterCrash(float _DeltaTime);
	void BusterEnd(float _DeltaTime);



	//=============================================

	UImageRenderer* Renderer = nullptr;
	UCollision* BusterCollision = nullptr;
	const float BusterSpeed = 0.05f; // �߻� �ӵ�

	float BusterLifeTime = 0.0f;
};


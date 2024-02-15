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

	// Player �⺻ ����.
	EPlayerState State = EPlayerState::None;

	// ������ ���� �߿��ϹǷ� ���� ����.

	/// <summary>
	/// Player �� ������ üũ�Ѵ�.
	/// </summary>
	void DirCheck();




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
	void IdleStart();
	void RunStart();
	void JumpStart();

	// ���� �Լ���
	
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
	/// <summary>
	/// ���� ��Ȳ�� �� ó�� �Լ�
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void Jump(float _DeltaTime);

private :
	UImageRenderer* Renderer = nullptr;
};
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
		
	// ������ ���� �߿��ϹǷ� ���� ����.

	/// <summary>
	/// <para>Player(Actor)�� ������ �����Ѵ�.</para>
	/// <para>������ �ٸ��� �ش� ���⿡ �´� Animation���� �ٲ۴�.</para>
	/// </summary>
	void DirCheck();

	/// <summary>
	/// ���� �������� Animation�� �̸��� �����´�.
	/// </summary>
	/// <param name="_Name"></param>
	/// <returns></returns>
	std::string GetAnimationName(std::string _Name);

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

	// ==========================================

	// Player �⺻ ����.
	EPlayerState State = EPlayerState::None;
	// Player ���� -> Actor ����
	EActorDir DirState = EActorDir::Right;
	// ���� �ִϸ��̼��� �̸�. -> �ִϸ��̼� �̸��� �Ű� ��� ��.
	std::string CurAnimationName = "None";

private :
	UImageRenderer* Renderer = nullptr;
};
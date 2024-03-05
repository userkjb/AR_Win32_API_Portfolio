#include "SpikeMarl.h"
#include "Egseu.h"
#include "Buster.h"

ASpikeMarl::ASpikeMarl()
{
}

ASpikeMarl::~ASpikeMarl()
{
}

void ASpikeMarl::BeginPlay()
{
	AActor::BeginPlay();

	SpikeMarlBeginPlay();
}

void ASpikeMarl::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	StateUpdate(_DeltaTime);
	CollisionCheck(_DeltaTime);
}

void ASpikeMarl::SpikeMarlBeginPlay()
{
	// Render
	SpikeMarlRender = CreateImageRenderer(static_cast<int>(ERenderOrder::Enemy));
	SpikeMarlRender->SetImage("SpikeBall_Right.png");
	SpikeMarlRender->AutoImageScale(2.0f);


	// Collision
	SpikeMarlCollision = CreateCollision(static_cast<int>(ECollisionOrder::Enemy));
	SpikeMarlCollision->SetPosition({ 0.0f, -26.0f });
	SpikeMarlCollision->SetScale({54, 44});
	SpikeMarlCollision->SetColType(ECollisionType::CirCle);

	AttackCollision = CreateCollision(static_cast<int>(ECollisionOrder::Enemy));
	AttackCollision->SetPosition({ 0.0f, -26.0f });
	AttackCollision->SetScale({ 54, 44 });
	AttackCollision->SetColType(ECollisionType::CirCle);
	AttackCollision->ActiveOff();


	// Animation
	
	// ¼ÒÈ¯ Animation
	SpikeMarlRender->CreateAnimation("Summon_Right", "SpikeBall_Right.png", 0, 1, 0.5f, true);
	SpikeMarlRender->CreateAnimation("Run_Right", "SpikeBall_Right.png", 2, 4, 0.5f, true);
	SpikeMarlRender->CreateAnimation("TransformSearch_Right", "SpikeBall_Right.png", 5, 11, 0.5f, false);
	SpikeMarlRender->CreateAnimation("_Right", "SpikeBall_Right.png", 5, 11, 0.5f, false);




	StateChange(ESpikeMarlState::None);
}

void ASpikeMarl::StateChange(ESpikeMarlState _State)
{
	if (State != _State)
	{
		switch (_State)
		{
		case ESpikeMarlState::None:
			break;
		case ESpikeMarlState::Summon:
			SummonStart();
			break;
		case ESpikeMarlState::Idle:
			IdleStart();
			break;
		case ESpikeMarlState::Run:
			RunStart();
			break;
		case ESpikeMarlState::Attack:
			AttackStart();
			break;
		case ESpikeMarlState::Death:
			DeathStart();
			break;
		default:
			break;
		}
	}
	State = _State;
}

void ASpikeMarl::StateUpdate(float _DeltaTime)
{
	switch (State)
	{
	case ESpikeMarlState::None:
		break;
	case ESpikeMarlState::Summon:
		Summon(_DeltaTime);
		break;
	case ESpikeMarlState::Idle:
		Idle(_DeltaTime);
		break;
	case ESpikeMarlState::Run:
		Run(_DeltaTime);
		break;
	case ESpikeMarlState::Attack:
		Attack(_DeltaTime);
		break;
	case ESpikeMarlState::Death:
		Death(_DeltaTime);
		break;
	default:
		break;
	}
}

#pragma region Summon
void ASpikeMarl::SummonStart()
{
}

void ASpikeMarl::Summon(float _DeltaTime)
{
}
#pragma endregion

#pragma region Idle
void ASpikeMarl::IdleStart()
{
}

void ASpikeMarl::Idle(float _DeltaTime)
{
}
#pragma endregion

#pragma region Run
void ASpikeMarl::RunStart()
{
}

void ASpikeMarl::Run(float _DeltaTime)
{
}
#pragma endregion

#pragma region Attack
void ASpikeMarl::AttackStart()
{
}
void ASpikeMarl::Attack(float _DeltaTime)
{
}
#pragma endregion

#pragma region Death
void ASpikeMarl::DeathStart()
{
}

void ASpikeMarl::Death(float _DeltaTime)
{
}
#pragma endregion

#pragma region Vector
void ASpikeMarl::MoveUpdate(float _DeltaTime)
{
	CalGravityVector(_DeltaTime);
	CalLastMoveVector();
	MoveLastMoveVector(_DeltaTime);
}

void ASpikeMarl::CalGravityVector(float _DeltaTime)
{
	GravityVector += GravityAcc * _DeltaTime;
}

void ASpikeMarl::CalLastMoveVector()
{
	LastMoveVector = FVector::Zero;
	LastMoveVector += RunVector;
	LastMoveVector += GravityVector;
}

void ASpikeMarl::MoveLastMoveVector(float _DeltaTime)
{
	AddActorLocation(LastMoveVector * _DeltaTime);
}
#pragma endregion


void ASpikeMarl::CollisionCheck(float _DeltaTime)
{
	std::vector<UCollision*> Result;
	if (true == SpikeMarlCollision->CollisionCheck(ECollisionOrder::Weapon, Result))
	{
		ABuster* Buster = dynamic_cast<ABuster*>(Result[0]->GetOwner());

	}
}
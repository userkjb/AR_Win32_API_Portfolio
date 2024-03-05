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
	// 소환 BackGround
	SummonBG = CreateImageRenderer(static_cast<int>(ERenderOrder::Enemy));
	SummonBG->SetImage("SummonEnemy.png");
	SummonBG->AutoImageScale(2.0f);

	// Enemy Render (본체)
	SpikeMarlRender = CreateImageRenderer(static_cast<int>(ERenderOrder::Enemy));
	SpikeMarlRender->SetImage("SpikeBall_Left.png");
	SpikeMarlRender->AutoImageScale(2.0f);

	// 소환 Pos
	SummonPosEffect = CreateImageRenderer(static_cast<int>(ERenderOrder::Enemy));
	SummonPosEffect->SetImage("SummonEnemy_Dummy.png");
	SummonPosEffect->AutoImageScale(2.0f);

	// Collision
	// 기본 Collision
	SpikeMarlCollision = CreateCollision(static_cast<int>(ECollisionOrder::Enemy));
	SpikeMarlCollision->SetPosition({ 0.0f, -26.0f });
	SpikeMarlCollision->SetScale({54, 44});
	SpikeMarlCollision->SetColType(ECollisionType::CirCle);

	// 공격 Collision
	AttackCollision = CreateCollision(static_cast<int>(ECollisionOrder::Enemy));
	AttackCollision->SetPosition({ 0.0f, -26.0f });
	AttackCollision->SetScale({ 54, 44 });
	AttackCollision->SetColType(ECollisionType::CirCle);
	AttackCollision->ActiveOff();


	// Animation
	//SummonBG->CreateAnimation("SummonStart", "SummonEnemy_End.png", {7, 6, 5, 4, 3, 2 ,1 ,0}, 0.05f, true);
	SummonBG->CreateAnimation("SummonLoop", "SummonEnemy.png", 0, 10, 0.05f, true);
	SummonBG->CreateAnimation("SummonEnd", "SummonEnemy_End.png", 0, 7, 0.05f, false);
	
	SummonPosEffect->CreateAnimation("SummonPos", "SummonEnemy_Dummy.png", 0, 2, 0.05f, true);

	// 소환 Animation
	SpikeMarlRender->CreateAnimation("Summon", "SpikeBall_Left.png", 0, 1, 0.5f, true);
	SpikeMarlRender->CreateAnimation("Run_Left", "SpikeBall_Left.png", 2, 4, 0.5f, true);
	SpikeMarlRender->CreateAnimation("TransformSearch_Left", "SpikeBall_Left.png", 5, 11, 0.5f, false);
	
	SpikeMarlRender->CreateAnimation("_Left", "SpikeBall_Right.png", 5, 11, 0.5f, false);




	StateChange(ESpikeMarlState::Summon);
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
	SummonBG->ChangeAnimation("SummonLoop");
	SpikeMarlRender->ChangeAnimation("Summon");
	SummonPosEffect->ChangeAnimation("SummonPos");
}

void ASpikeMarl::Summon(float _DeltaTime)
{
	if (b_ActorLocation == false)
	{
		b_ActorLocation = true;
		ActorPosFix = GetActorLocation();
	}
	RunVector = FVector::Left * SummonSpeed;
	MoveUpdate(_DeltaTime, false);

	bool x = SummonBG->IsActive();
	int a = 0;
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
void ASpikeMarl::MoveUpdate(float _DeltaTime, bool _Gravity)
{
	CalGravityVector(_DeltaTime, _Gravity);
	CalLastMoveVector();
	MoveLastMoveVector(_DeltaTime);
}

void ASpikeMarl::CalGravityVector(float _DeltaTime, bool _Gravity)
{
	if (_Gravity)
	{
		GravityVector += GravityAcc * _DeltaTime;
	}
	else
	{
		GravityVector = FVector::Zero;
	}
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
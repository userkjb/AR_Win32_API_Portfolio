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
	SummonBGL = CreateImageRenderer(static_cast<int>(ERenderOrder::EnemyPrev));
	SummonBGL->SetImage("SummonEnemy_L.png");
	SummonBGL->AutoImageScale(2.0f);

	SummonBGR = CreateImageRenderer(static_cast<int>(ERenderOrder::EnemyNext));
	SummonBGR->SetImage("SummonEnemy_R.png");
	SummonBGR->AutoImageScale(2.0f);

	// Enemy Render (본체)
	SpikeMarlRender = CreateImageRenderer(static_cast<int>(ERenderOrder::Enemy));
	SpikeMarlRender->SetImage("SpikeBall_Left.png");
	SpikeMarlRender->AutoImageScale(2.0f);
	SpikeMarlRender->SetPosition({ 10, 30 });

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
	SummonBGL->CreateAnimation("SummonStart", "SummonEnemy_End.png", {7, 6, 5, 4, 3, 2 ,1 ,0}, 0.2f, false);
	SummonBGL->CreateAnimation("SummonLoop", "SummonEnemy_L.png", 0, 10, 0.05f, true);
	SummonBGL->CreateAnimation("SummonEnd", "SummonEnemy_End.png", 0, 7, 0.2f, false);
	
	SummonBGR->CreateAnimation("SummonLoop", "SummonEnemy_R.png", 0, 10, 0.05f, true);
	SummonBGR->ActiveOff();
	
	SummonPosEffect->CreateAnimation("SummonPos", "SummonEnemy_Dummy.png", 0, 2, 0.05f, true);
	SummonPosEffect->ActiveOff();

	// 소환 Animation
	//SpikeMarlRender->CreateAnimation("Summon", "SpikeBall_Left.png", 0, 1, 0.2f, false);
	SpikeMarlRender->CreateAnimation("Run_Left", "SpikeBall_Left.png", 2, 4, 0.3f, true);
	SpikeMarlRender->CreateAnimation("TransformSearch_Left", "SpikeBall_Left.png", 5, 11, 0.2f, false);
	
	SpikeMarlRender->CreateAnimation("_Left", "SpikeBall_Right.png", 5, 11, 0.5f, false);
	SpikeMarlRender->ActiveOff();




	StateChange(ESpikeMarlState::SummonStart);
}

void ASpikeMarl::StateChange(ESpikeMarlState _State)
{
	if (State != _State)
	{
		switch (_State)
		{
		case ESpikeMarlState::None:
			break;
		case ESpikeMarlState::SummonStart:
			SummonStart();
			break;
		case ESpikeMarlState::SummonLoop:
			SummonLoopStart();
			break;
		case ESpikeMarlState::SummonEnd:
			SummonEndStart();
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
	case ESpikeMarlState::SummonStart:
		SummonStart(_DeltaTime);
		break;
	case ESpikeMarlState::SummonLoop:
		SummonLoop(_DeltaTime);
		break;
	case ESpikeMarlState::SummonEnd:
		SummonEnd(_DeltaTime);
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
	SummonBGL->ChangeAnimation("SummonStart");
}

void ASpikeMarl::SummonStart(float _DeltaTime)
{
	if (SummonBGL->GetCurAnimationImageFrame() == 0)
	{
		SummonTime += _DeltaTime;
	}

	if (0.1f < SummonTime)
	{
		SummonTime = 0.0f;
		StateChange(ESpikeMarlState::SummonLoop);
		return;
	}
}
void ASpikeMarl::SummonLoopStart()
{
	SummonBGL->ChangeAnimation("SummonLoop");
	SummonBGR->ActiveOn();
	SummonBGR->ChangeAnimation("SummonLoop");
	SpikeMarlRender->ActiveOn();
	SpikeMarlRender->ChangeAnimation("Run_Left");
	SummonPosEffect->ActiveOn();
	SummonPosEffect->ChangeAnimation("SummonPos");
	if (SummonTime != 0.0f)
	{
		SummonTime = 0.0f;
	}
	RunVector = FVector::Left * SummonSpeed;
}
void ASpikeMarl::SummonLoop(float _DeltaTime)
{
	SummonTime += _DeltaTime;
	
	AddActorLocation(RunVector * _DeltaTime);
	//MoveUpdate(_DeltaTime, false);

	SummonBGL->AddPosition(FVector::Left * SummonSpeed * -1.0f * _DeltaTime);
	SummonBGR->AddPosition(FVector::Left * SummonSpeed * -1.0f * _DeltaTime);
	SummonPosEffect->AddPosition(FVector::Left * SummonSpeed * -1.0f * _DeltaTime);

	if (1.0f < SummonTime)
	{
		SummonTime = 0.0f;
		StateChange(ESpikeMarlState::SummonEnd);
		return;
	}
}
void ASpikeMarl::SummonEndStart()
{
	if (SummonTime != 0.0f)
	{
		SummonTime = 0.0f;
	}

	SummonBGL->ChangeAnimation("SummonEnd");
	SummonBGR->Destroy();
	SummonBGR = nullptr;
	SummonPosEffect->Destroy();
	SummonPosEffect = nullptr;
	DownVector = FVector::Down * DownSpeed;
}
void ASpikeMarl::SummonEnd(float _DeltaTime)
{
	// 떨어진다~~
	AddActorLocation(RunVector * _DeltaTime);
	AddActorLocation(DownVector * _DeltaTime);
	SummonBGL->AddPosition(RunVector * -1.0f * _DeltaTime);
	SummonBGL->AddPosition(DownVector * -1.0f * _DeltaTime);
	

	if ( 6 == SummonBGL->GetCurAnimationImageFrame())
	{
		SummonBGL->ActiveOff();
	}

	// 지면에 닿으면,
	FVector CheckPos = GetActorLocation();
	CheckPos.Y += 30.0f;
	Color8Bit Color = UContentsGlobalData::ColMapImage->GetColor(CheckPos.iX(), CheckPos.iY(), Color8Bit::MagentaA);
	if (Color == Color8Bit(255, 0, 255, 0))
	{
		RunVector = FVector::Zero;
		DownVector = FVector::Zero;
		
		StateChange(ESpikeMarlState::Idle);
		return;
	}
}
#pragma endregion

#pragma region Idle
void ASpikeMarl::IdleStart()
{
	SummonBGL->Destroy();
	SummonBGL = nullptr;
	SpikeMarlRender->ChangeAnimation("TransformSearch_Left");
}

void ASpikeMarl::Idle(float _DeltaTime)
{
	// 다리 꺼냄.
	if (SpikeMarlRender->IsCurAnimationEnd())
	{
		// Player와의 거리를 체크.
		FVector DePos = this->GetActorLocation(); // 내 위치.
		FVector PlPos = AEgseu::GetMainPlayer()->GetActorLocation(); // Player 위치

		float result = std::sqrtf(std::powf(DePos.X - PlPos.X, 2) + std::powf(DePos.Y - PlPos.Y, 2));

		int Between = std::lround(result); // Player와의 거리
		UEngineDebug::OutPutDebugText(std::to_string(Between));

		if (Between <= 300)
		{
			// 있으면 Attack
			StateChange(ESpikeMarlState::Attack);
			return;
		}
		else
		{
			// 없으면 run.
			StateChange(ESpikeMarlState::Run);
			return;
		}
	}
}
#pragma endregion

#pragma region Run
// 바라보고 있는 방향으로 일정 거리 움직이고,
// Player가 없으면, 반대 방향으로 돈다.
void ASpikeMarl::RunStart()
{

}

void ASpikeMarl::Run(float _DeltaTime)
{
}
#pragma endregion

#pragma region Attack
// Player가 있으면 Roll 형태로 변해서
// 특정 거리만큼 굴러간다.
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
//void ASpikeMarl::MoveUpdate(float _DeltaTime, bool _Gravity)
//{
//	CalGravityVector(_DeltaTime, _Gravity);
//	CalLastMoveVector();
//	MoveLastMoveVector(_DeltaTime);
//}

//void ASpikeMarl::CalGravityVector(float _DeltaTime, bool _Gravity)
//{
//	if (_Gravity)
//	{
//		DownVector += GravityAcc * _DeltaTime;
//	}
//	else
//	{
//		DownVector = FVector::Zero;
//	}
//}

//void ASpikeMarl::CalLastMoveVector()
//{
//	LastMoveVector = FVector::Zero;
//	LastMoveVector += RunVector;
//	LastMoveVector += DownVector;
//}

//void ASpikeMarl::MoveLastMoveVector(float _DeltaTime)
//{
//	AddActorLocation(LastMoveVector * _DeltaTime);
//}
#pragma endregion


void ASpikeMarl::CollisionCheck(float _DeltaTime)
{
	std::vector<UCollision*> Result;
	if (true == SpikeMarlCollision->CollisionCheck(ECollisionOrder::Weapon, Result))
	{
		ABuster* Buster = dynamic_cast<ABuster*>(Result[0]->GetOwner());
		int DefaultBusterDamage = Buster->GetDefaultBusterDamage();
		Hp -= DefaultBusterDamage;
	}
}
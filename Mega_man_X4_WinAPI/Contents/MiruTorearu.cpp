#include "MiruTorearu.h"
#include "Egseu.h"

AMiruTorearu::AMiruTorearu()
{
}

AMiruTorearu::~AMiruTorearu()
{
}

void AMiruTorearu::BeginPlay()
{
	AActor::BeginPlay();

	BeginRender();
}

void AMiruTorearu::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	StateUpdate(_DeltaTime);
	CollisionCheck();
}

void AMiruTorearu::BeginRender()
{
	MiruTorearuRender = CreateImageRenderer(static_cast<int>(ERenderOrder::Enemy));
	MiruTorearuRender->SetImage("MiruToraeru_Death_Effect.png");
	MiruTorearuRender->AutoImageScale(2.0f);

	MiruTorearuCollision = CreateCollision(ECollisionOrder::Enemy);
	MiruTorearuCollision->SetScale({ 48 * 2, 46 * 2 });
	MiruTorearuCollision->SetColType(ECollisionType::CirCle);

	DeathRender = CreateImageRenderer(static_cast<int>(ERenderOrder::EnemyEffect));
	DeathRender->SetImage("MiruToraeru_Death_Effect.png");
	DeathRender->AutoImageScale(2.0f);
	DeathRender->ActiveOff();

	StateChange(EMiruTorearuState::None);
	BeginCreateAnimation();
}

void AMiruTorearu::BeginCreateAnimation()
{
	MiruTorearuRender->CreateAnimation("Stop", "ElecballAtRest.png", 0, 4, 0.2f, true);
	MiruTorearuRender->CreateAnimation("Run_Left", "Rotating Elecball.png", 0, 10, 0.2f, true);
	MiruTorearuRender->CreateAnimation("Run_Right", "Rotating Elecball.png", { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 }, 0.2f, true);
	MiruTorearuRender->CreateAnimation("Attack", "GetPlayer.png", 0, 5, 0.2f, true);
	
	DeathRender->CreateAnimation("Dummy", "MiruToraeru_Death_Effect.png", 0, 0, 1.0f, true);
	DeathRender->CreateAnimation("Death_Start", "MiruToraeru_Death_Effect.png", 0, 6, 0.2f, false);
	MiruTorearuRender->CreateAnimation("Death_Loop", "MiruToraeru_Death.png", 0, 13, 0.2f, false);

	DeathRender->ChangeAnimation("Dummy");
}

std::string AMiruTorearu::SetAnimationName(std::string _Name)
{
	std::string DirName = "";

	switch (MiruDir)
	{
	case EActorDir::Left:
		DirName = "_Left";
		break;
	case EActorDir::Right:
		DirName = "_Right";
		break;
	default :
		break;
	}

	return _Name + DirName;
}

void AMiruTorearu::StateChange(EMiruTorearuState _State)
{
	if (State != _State)
	{
		switch (_State)
		{
		case EMiruTorearuState::Stop:
			StopStart();
			break;
		case EMiruTorearuState::Run:
			RunStart();
			break;
		case EMiruTorearuState::Attack:
			AttackStart();
			break;
		case EMiruTorearuState::DeathStart:
			DeathStartStart();
			break;
		case EMiruTorearuState::DeathLoop:
			DeathLoopStart();
			break;
		case EMiruTorearuState::DeathEnd:
			DeathEndStart();
			break;
		default:
			break;
		}
	}
	State = _State;
}

void AMiruTorearu::StateUpdate(float _DeltaTime)
{
	switch (State)
	{
	case EMiruTorearuState::None:
		None(_DeltaTime);
		break;
	case EMiruTorearuState::StopCreate:
		StopCreate(_DeltaTime);
		break;
	case EMiruTorearuState::RunCreate:
		RunCreate(_DeltaTime);
		break;
	case EMiruTorearuState::Stop:
		Stop(_DeltaTime);
		break;
	case EMiruTorearuState::Run:
		Run(_DeltaTime);
		break;
	case EMiruTorearuState::Attack:
		Attack(_DeltaTime);
		break;
	case EMiruTorearuState::DeathStart:
		DeathStart(_DeltaTime);
		break;
	case EMiruTorearuState::DeathLoop:
		DeathLoop(_DeltaTime);
		break;
	case EMiruTorearuState::DeathEnd:
		DeathEnd(_DeltaTime);
		break;
	default:
		break;
	}
}

#pragma region None / Create
void AMiruTorearu::None(float _DeltaTime)
{
}
void AMiruTorearu::StopCreate(float _DeltaTime)
{
	StateChange(EMiruTorearuState::Stop);
	return;
}
void AMiruTorearu::RunCreate(float _DeltaTime)
{
	StateChange(EMiruTorearuState::Run);
	return;
}
#pragma endregion

#pragma region Stop
void AMiruTorearu::StopStart()
{
	MiruTorearuRender->ChangeAnimation("Stop");
}
void AMiruTorearu::Stop(float _DeltaTime)
{
	int a = 0;
}
#pragma endregion

#pragma region Run
void AMiruTorearu::RunStart()
{
	MiruTorearuRender->ChangeAnimation(SetAnimationName("Run"));
}
void AMiruTorearu::Run(float _DeltaTime)
{
	if (MiruDir == EActorDir::Left)
	{
		RunVector = FVector::Left * RunSpeed * _DeltaTime;
	}
	else if (MiruDir == EActorDir::Right)
	{
		RunVector = FVector::Right * RunSpeed * _DeltaTime;
	}
	
	AddActorLocation(RunVector);
}
#pragma endregion

#pragma region Attack
void AMiruTorearu::AttackStart()
{
	MiruTorearuCollision->ActiveOff();
	RunVector = FVector::Zero;
	MiruTorearuRender->ChangeAnimation("Attack");
}

void AMiruTorearu::Attack(float _DeltaTime)
{
	FVector PlayerPos = Player->GetActorLocation();
	FVector PlayerDir = PlayerPos - this->GetActorLocation();
	PlayerDir.Normalize2D();

	PlayerVector = -PlayerDir * PlayerMoveSpeed * _DeltaTime;
	Player->AddActorLocation(PlayerVector);

	// Test
	if (true == UEngineInput::IsDown('Y'))
	{
		StateChange(EMiruTorearuState::DeathStart);
		return;
	}
}
#pragma endregion

#pragma region DeathStart
void AMiruTorearu::DeathStartStart()
{
	DeathRender->ActiveOn();
	DeathRender->ChangeAnimation("Death_Start");
	MiruTorearuRender->ChangeAnimation("Stop");
}

void AMiruTorearu::DeathStart(float _DeltaTime)
{
	if (true == DeathRender->IsCurAnimationEnd())
	{
		StateChange(EMiruTorearuState::DeathLoop);
		return;
	}
}
#pragma endregion

#pragma region DeathLoop
void AMiruTorearu::DeathLoopStart()
{
	DeathRender->ActiveOff();
	MiruTorearuRender->ChangeAnimation("Death_Loop");
}

void AMiruTorearu::DeathLoop(float _DeltaTime)
{
	if (true == MiruTorearuRender->IsCurAnimationEnd())
	{
		StateChange(EMiruTorearuState::DeathEnd);
		return;
	}
}
#pragma endregion

#pragma region DeathEnd
void AMiruTorearu::DeathEndStart()
{
	MiruTorearuRender->ActiveOff();
}

void AMiruTorearu::DeathEnd(float _DeltaTime)
{
	this->Destroy(0.1f);
}
#pragma endregion

void AMiruTorearu::CollisionCheck()
{
	std::vector<UCollision*> Result;
	if (true == MiruTorearuCollision->CollisionCheck(ECollisionOrder::Player, Result))
	{
		Player = dynamic_cast<AEgseu*>(Result[0]->GetOwner());
		StateChange(EMiruTorearuState::Attack);
		return;
	}
}

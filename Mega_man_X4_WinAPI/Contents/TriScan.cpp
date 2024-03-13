#include "TriScan.h"
#include <EngineCore/EngineCore.h>
#include "Egseu.h"
#include "Buster.h"

ATriScan::ATriScan()
{
}

ATriScan::~ATriScan()
{
}

void ATriScan::BeginPlay()
{
	AActor::BeginPlay();

	StateChange(ETriScanState::None);
	
	TriScanBeginPlay();
}

void ATriScan::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	GetPlayerPos_Tick();
	StateUpdate(_DeltaTime);
	CollisionCheck();
}

void ATriScan::TriScanBeginPlay()
{
	// TriScan Render
	TriScanRender = CreateImageRenderer(static_cast<int>(ERenderOrder::Enemy));
	TriScanRender->SetImage("Triangle.png");
	TriScanRender->AutoImageScale(2.0f);

	TriScanRender->CreateAnimation("TriScanIdle", "Triangle.png", 0, 4, 0.5f, true);
	TriScanRender->CreateAnimation("TriScanRun", "Triangle.png", 5, 9, 0.5f, true);

	// Fragments Render
	FragmentsRender = CreateImageRenderer(static_cast<int>(ERenderOrder::Enemy));
	FragmentsRender->SetImage("TriScan_Fragments.png");
	FragmentsRender->AutoImageScale(2.0f);
	FragmentsRender->ActiveOff();

	// Collision
	TriScanCollision = CreateCollision(ECollisionOrder::Enemy);
	TriScanCollision->SetScale({26, 24});
	TriScanCollision->SetColType(ECollisionType::CirCle);

	// Default Setting
	TriScanRender->ChangeAnimation("TriScanIdle");
	StateChange(ETriScanState::Idle);
}

void ATriScan::StateChange(ETriScanState _State)
{
	if (State != _State)
	{
		switch (_State)
		{
		case ETriScanState::None:
			break;
		case ETriScanState::Idle:
			IdleStart();
			break;
		case ETriScanState::Run:
			RunStart();
			break;
		case ETriScanState::Death:
			DeathStart();
			break;
		case ETriScanState::BusterCollision:
			DeathStart();
			break;
		default:
			break;
		}
	}
	State = _State;
}

void ATriScan::StateUpdate(float _DeltaTime)
{
	switch (State)
	{
	case ETriScanState::None:
		break;
	case ETriScanState::Idle:
		Idle(_DeltaTime);
		break;
	case ETriScanState::Run:
		Run(_DeltaTime);
		break;
	case ETriScanState::Death:
		Death(_DeltaTime);
		break;
	case ETriScanState::BusterCollision:
		BusterCollision(_DeltaTime);
		break;
	default:
		break;
	}
}

void ATriScan::GetPlayerPos_Tick()
{
	//AEgseu* Player = AEgseu::GetMainPlayer();
	PlayerPos = AEgseu::GetMainPlayer()->GetActorLocation();
}


#pragma region Idle
void ATriScan::IdleStart()
{
	TriScanRender->ChangeAnimation("TriScanIdle");
}

void ATriScan::Idle(float _DeltaTime)
{
	FVector DePos = this->GetActorLocation(); // 내 위치.
	FVector PlPos = PlayerPos;

	float result = std::sqrtf(std::powf(DePos.X - PlPos.X, 2) + std::powf(DePos.Y - PlPos.Y, 2));

	int Between = std::lround(result);
	//UEngineDebug::OutPutDebugText(std::to_string(Between));

	// Player 와 TriScan 의 거리가 수치 이하일 때,
	// if (Between <= 500) /////////////////////////////////////////// Test
	if (Between <= 300)
	{
		SearchTime += _DeltaTime;
		// 수치 만큼의 딜레이를 가지고.
		//if (SearchTime < 1.0f)  //////////////////////////////////// Test
		if (SearchTime < 5.0f)
		{
			return;
		}
		else
		{
			StateChange(ETriScanState::Run);
			return;
		}
	}
	else
	{
		SearchTime = 0.0f;
		return;
	}
}
#pragma endregion


#pragma region Run
void ATriScan::RunStart()
{
	SearchTime = 0.0f;
	PlayerAttackPos = PlayerPos; // 딱 한번 Player의 좌표를 가져오고.
	TriScanRender->ChangeAnimation("TriScanRun");
}

void ATriScan::Run(float _DeltaTime)
{	
	// 내 기준에서 Player가 있는 방향을 알아야 한다.
	PlayerDir = PlayerAttackPos - this->GetActorLocation(); //  뱡향
	PlayerDir.Normalize2D();

	// 이동!!
	RunVector = PlayerDir * RunSpeed;
	MoveUpdate(_DeltaTime);

	FVector DePos = this->GetActorLocation(); // 내 위치.
	FVector PlPos = PlayerAttackPos;

	float result = std::sqrtf(std::powf(DePos.X - PlPos.X, 2) + std::powf(DePos.Y - PlPos.Y, 2));
	int Between = std::lround(result);

	if (Between <= 0)
	{
		PlayerDir = FVector::Zero;
		RunVector = FVector::Zero;
		StateChange(ETriScanState::Idle);
		return;
	}
}
#pragma endregion


#pragma region Death
void ATriScan::DeathStart()
{
	TriScanRender->ActiveOff();
	TriScanCollision->ActiveOff();
	FragmentsRender->ActiveOn();
}

void ATriScan::Death(float _DeltaTime)
{
	DeathTime += _DeltaTime;
	
	RunVector = FVector::Left * RunSpeed;
	RunVector = RunVector + (FVector::Up * 450.0f);

	MoveUpdate(_DeltaTime, true);

	if (2.0f <= DeathTime)
	{
		this->Destroy(0.0f);
	}
}
#pragma endregion

#pragma region BusterCollision
void ATriScan::BusterCollisionStart()
{
	PreState = State;
	int a = 0;
}
void ATriScan::BusterCollision(float _DeltaTime)
{
	int BusterType = static_cast<int>(PrevBuster->GetBusterState());

	if (BusterType == static_cast<int>(EBusterState::DefaultCharge))
	{
		Hp -= PrevBuster->GetDefaultBusterDamage();
	}
	else if (BusterType == static_cast<int>(EBusterState::MiddleCharge))
	{
		Hp -= PrevBuster->GetMiddleBusterDamage();
	}
	else if (BusterType == static_cast<int>(EBusterState::PullCharge))
	{
		Hp -= PrevBuster->GetPullBusterDamage();
	}

	if (Hp <= 0)
	{
		PlayerDir = FVector::Zero;
		RunVector = FVector::Zero;
		StateChange(ETriScanState::Death);
		return;
	}

	StateChange(PreState);
	return;
}
#pragma endregion

void ATriScan::MoveUpdate(float _DeltaTime, bool _Gravity)
{
	CalGravityVector(_DeltaTime, _Gravity);
	CalLastMoveVector();
	MoveLastMoveVector(_DeltaTime);
}

void ATriScan::CalGravityVector(float _DeltaTime, bool _Gravity)
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

void ATriScan::CalLastMoveVector()
{
	LastMoveVector = FVector::Zero;
	LastMoveVector += RunVector;
	LastMoveVector += GravityVector;
}

void ATriScan::MoveLastMoveVector(float _DeltaTime)
{
	AddActorLocation(LastMoveVector * _DeltaTime);
}

// Buster와 중복됨...
void ATriScan::CollisionCheck()
{
	// 중복 충돌 해결 해야 함.
	std::vector<UCollision*> Result;
	if (true == TriScanCollision->CollisionCheck(ECollisionOrder::Weapon, Result))
	{
		//if (0 == CollisionCount)
		ABuster* Buster = dynamic_cast<ABuster*>(Result[0]->GetOwner());
		PrevBuster = Buster;
		StateChange(ETriScanState::BusterCollision);
		return;
	}
}


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
	GetPlayerPos_Tick();
	StateUpdate(_DeltaTime);
	CollisionCheck();
}

void ATriScan::TriScanBeginPlay()
{
	TriScanRender = CreateImageRenderer(static_cast<int>(ERenderOrder::Enemy));
	TriScanRender->SetImage("Triangle.png");
	TriScanRender->AutoImageScale();

	TriScanRender->CreateAnimation("TriScanIdle", "Triangle.png", 0, 4, 1.0f, true);
	TriScanRender->CreateAnimation("TriScanRun", "Triangle.png", 5, 9, 1.0f, true);

	TriScanCollision = CreateCollision(ECollisionOrder::Enemy);
	FVector ImageScale = TriScanRender->GetImage()->GetScale();
	TriScanCollision->SetScale(ImageScale);
	TriScanCollision->SetColType(ECollisionType::CirCle);

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
	if (Between <= 200)
	{
		SearchTime += _DeltaTime;
		// 수치 만큼의 딜레이를 가지고.
		//if (SearchTime < 5.0f)
		if (SearchTime < 0.5f)
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
	RunVector = PlayerDir * 100.0f;
	CalVector(_DeltaTime);

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


void ATriScan::DeathStart()
{
}

void ATriScan::Death(float _DeltaTime)
{
}

void ATriScan::CalVector(float _DeltaTime)
{
	AddActorLocation(RunVector * _DeltaTime);
}

void ATriScan::CollisionCheck()
{
	std::vector<UCollision*> Result;
	if (true == TriScanCollision->CollisionCheck(ECollisionOrder::Weapon, Result))
	{
		//ABuster* Buster = (ABuster*)Result[0]->GetOwner();
		ABuster* Buster = dynamic_cast<ABuster*>(Result[0]->GetOwner());
		int BusterType = static_cast<int>(Buster->GetBusterState());

		if (BusterType == 0)
		{
			Hp -= Buster->GetDefaultBusterDamage();
		}

		if (Hp <= 0)
		{
			StateChange(ETriScanState::Death);
			return;
		}
	}
}


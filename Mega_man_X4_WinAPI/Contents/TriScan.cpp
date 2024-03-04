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
	FVector DePos = this->GetActorLocation(); // �� ��ġ.
	FVector PlPos = PlayerPos;

	float result = std::sqrtf(std::powf(DePos.X - PlPos.X, 2) + std::powf(DePos.Y - PlPos.Y, 2));

	int Between = std::lround(result);
	//UEngineDebug::OutPutDebugText(std::to_string(Between));

	// Player �� TriScan �� �Ÿ��� ��ġ ������ ��,
	if (Between <= 200)
	{
		SearchTime += _DeltaTime;
		// ��ġ ��ŭ�� �����̸� ������.
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
	PlayerAttackPos = PlayerPos; // �� �ѹ� Player�� ��ǥ�� ��������.
	TriScanRender->ChangeAnimation("TriScanRun");
}

void ATriScan::Run(float _DeltaTime)
{	
	// �� ���ؿ��� Player�� �ִ� ������ �˾ƾ� �Ѵ�.
	PlayerDir = PlayerAttackPos - this->GetActorLocation(); //  ����
	PlayerDir.Normalize2D();

	// �̵�!!
	RunVector = PlayerDir * 100.0f;
	CalVector(_DeltaTime);

	FVector DePos = this->GetActorLocation(); // �� ��ġ.
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


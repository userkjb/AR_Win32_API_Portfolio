#include "Buster.h"
#include <EngineCore/EngineCore.h> // Helper

ABuster::ABuster()
{
}

ABuster::~ABuster()
{
}

//void ABuster::SetBusterAnimation(std::string_view _Name)
//{
//	Renderer->ChangeAnimation(_Name);
//}

void ABuster::BeginPlay()
{
	AActor::BeginPlay();

	Renderer = CreateImageRenderer(static_cast<int>(ERenderOrder::Buster));
	Renderer->SetImage("x_Buster_Default_Right.png");
	Renderer->AutoImageScale(2.0f);
	//UWindowImage* Image = Renderer->GetImage();
	//FVector ImageScale = Image->GetScale();
	//Renderer->SetTransform({ {0, 0}, {ImageScale.X, (ImageScale.Y) * 3} }); // test

	Renderer->CreateAnimation("Buster_Default_Right", "x_Buster_Default_Right.png", 0, 4, 0.05f, true);
	Renderer->CreateAnimation("Buster_Default_Left", "x_Buster_Default_Left.png", 0, 4, 0.05f, true);

	Renderer->CreateAnimation("Buster_Middle_Right", "x_Buster_Middle_Right.png", 0, 3, 0.05f, true);
	Renderer->CreateAnimation("Buster_Middle_Left", "x_Buster_Middle_Left.png", 0, 3, 0.05f, true);

	Renderer->CreateAnimation("Buster_Pull_Right", "x_Buster_Pull_Right.png", 0, 2, 0.05f, true);
	Renderer->CreateAnimation("Buster_Pull_Left", "x_Buster_Pull_Left.png", 0, 2, 0.05f, true);
		
	BusterCollision = CreateCollision(ECollisionOrder::Weapon);
	//BusterCollision->SetScale(ImageScale); // 각 Buster에 대한 Start 함수가 있어야 할 것 같은데?
	BusterCollision->SetColType(ECollisionType::CirCle);

	StateChange(EBusterState::CreateBuster);
} // end BeginPlay()

void ABuster::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	StateUpdate(_DeltaTime);
	CollisionCheck();
}


// BeginPlay Functions
void ABuster::StateChange(EBusterState _State)
{
	// 상태가 변했다면,
	if (E_BusterState != _State)
	{
		switch (_State)
		{
		case EBusterState::DefaultCharge:
			DefaultChargeStart();
			break;
		case EBusterState::MiddleCharge:
			MiddleChargeStart();
			break;
		case EBusterState::PullCharge:
			PullChargeStart();
			break;
		case EBusterState::BusterCrash:
			BusterCrashStart();
			break;
		case EBusterState::BusterEnd:
			BusterEndStart();
			break;
		default:
			break;
		}
	}

	E_BusterState = _State;
}

// Tick Functions
void ABuster::StateUpdate(float _DeltaTime)
{
	switch (E_BusterState)
	{
	case EBusterState::CreateDefault:
		DefaultBegin(_DeltaTime);
		break;
	case EBusterState::CreateMiddle:
		MiddleBegin(_DeltaTime);
		break;
	case EBusterState::CreatePull:
		PullBegin(_DeltaTime);
		break;
	case EBusterState::DefaultCharge:
		DefaultBuster(_DeltaTime);
		break;
	case EBusterState::MiddleCharge:
		MiddleCharge(_DeltaTime);
		break;
	case EBusterState::PullCharge:
		PullCharge(_DeltaTime);
		break;
	case EBusterState::BusterCrash:
		BusterCrash(_DeltaTime);
		break;
	case EBusterState::BusterEnd:
		BusterEnd(_DeltaTime);
		break;
	default:
		break;
	}
}


std::string ABuster::GetRorL() const
{
	std::string Name = "";
	if (DirState == EActorDir::Right)
	{
		Name = "_Right";
	}
	else if (DirState == EActorDir::Left)
	{
		Name = "_Left";
	}
	return Name;
}

void ABuster::DefaultBegin(float _DeltaTime)
{
	StateChange(EBusterState::DefaultCharge);
	return;
}

void ABuster::MiddleBegin(float _DeltaTime)
{
	StateChange(EBusterState::MiddleCharge);
	return;
}

void ABuster::PullBegin(float _DeltaTime)
{
	StateChange(EBusterState::PullCharge);
	return;
}

#pragma region DefaultCharge
void ABuster::DefaultChargeStart()
{
	std::string AniName = "Buster_Default" + GetRorL();
	Renderer->ChangeAnimation(AniName);
	BusterCollision->SetPosition({ 15.0f, 0.0f });
	BusterCollision->SetScale({ 15.0f, 10.0f });
	BusterLifeTime = 0.0f;
	BusterVector = FVector::Zero;
}

void ABuster::DefaultBuster(float _DeltaTime)
{
	BusterLifeTime += _DeltaTime;
	if (DirState == EActorDir::Right)
	{
		BusterVector = FVector::Right * BusterSpeed * _DeltaTime;
	}
	else if (DirState == EActorDir::Left)
	{
		BusterVector = FVector::Left * BusterSpeed * _DeltaTime;
	}
	AddActorLocation(BusterVector);

	if (BusterLifeTime >= BusterLife)
	{
		StateChange(EBusterState::BusterEnd);
		return;
	}
}
#pragma endregion

#pragma region MiddleCharge
void ABuster::MiddleChargeStart()
{
	std::string AniName = "Buster_Middle" + GetRorL();
	Renderer->ChangeAnimation(AniName);
	BusterCollision->SetPosition({ 28.0f, 0.0f });
	BusterCollision->SetScale({ 28.0f, 18.0f });
	BusterLifeTime = 0.0f;
	BusterVector = FVector::Zero;
}

void ABuster::MiddleCharge(float _DeltaTime)
{
	BusterLifeTime += _DeltaTime;
	if (DirState == EActorDir::Right)
	{
		BusterVector = FVector::Right * BusterSpeed * _DeltaTime;
	}
	else if (DirState == EActorDir::Left)
	{
		BusterVector = FVector::Left * BusterSpeed * _DeltaTime;
	}
	AddActorLocation(BusterVector);

	if (BusterLifeTime >= BusterLife)
	{
		StateChange(EBusterState::BusterEnd);
		return;
	}
}
#pragma endregion

#pragma region PullCharge
void ABuster::PullChargeStart()
{
	std::string AniName = "Buster_Pull" + GetRorL();
	Renderer->ChangeAnimation(AniName);
	BusterCollision->SetPosition({ 64.0f, 0.0f });
	BusterCollision->SetScale({ 64.0f, 32.0f });
	BusterLifeTime = 0.0f;
	BusterVector = FVector::Zero;
}

void ABuster::PullCharge(float _DeltaTime)
{
	BusterLifeTime += _DeltaTime;
	if (DirState == EActorDir::Right)
	{
		BusterVector = FVector::Right * BusterSpeed * _DeltaTime;
	}
	else if (DirState == EActorDir::Left)
	{
		BusterVector = FVector::Left * BusterSpeed * _DeltaTime;
	}
	AddActorLocation(BusterVector);

	if (BusterLifeTime >= BusterLife)
	{
		StateChange(EBusterState::BusterEnd);
		return;
	}
}
#pragma endregion

#pragma region BusterCrash
void ABuster::BusterCrashStart()
{
	// 충동시 나오는 임펙트 출력. //================================
	int a = 0;
	Renderer->ActiveOff();

}

void ABuster::BusterCrash(float _DeltaTime)
{
	BusterVector = FVector::Zero;
	MoveUpdate(_DeltaTime);

	if (true == Renderer->IsCurAnimationEnd())
	{
		StateChange(EBusterState::BusterEnd);
		return;
	}
}
#pragma endregion

#pragma region BusterEnd
void ABuster::BusterEndStart()
{
	BusterVector = FVector::Zero;
	BusterLifeTime = 0.0f;
}

void ABuster::BusterEnd(float _DeltaTime)
{
	this->Destroy(0.0f);
	//Buster->Destroy(0.0f);
	//Buster = nullptr;
}
#pragma endregion


void ABuster::MoveUpdate(float _DeltaTime)
{
	LastMoveVector = FVector::Zero;
	LastMoveVector += BusterVector;

	AddActorLocation(LastMoveVector);
}



void ABuster::CollisionCheck()
{
	// 1. Object
	// 2. Enemy
	// 3. Boss

	std::vector<UCollision*> EnemyResult;
	//std::vector<UCollision*> MapObjectResult;
	
	if (true == BusterCollision->CollisionCheck(ECollisionOrder::Enemy, EnemyResult))
	{
		StateChange(EBusterState::BusterCrash);
		return;
	}

	if (true == BusterCollision->CollisionCheck(ECollisionOrder::Boss, EnemyResult))
	{
		return;
	}
}

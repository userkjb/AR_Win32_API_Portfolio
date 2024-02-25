#include "Buster.h"
#include <EngineCore/EngineCore.h> // Helper

ABuster::ABuster()
{
}

ABuster::~ABuster()
{
}

void ABuster::SetBusterAnimation(std::string_view _Name)
{
	Renderer->ChangeAnimation(_Name);
}

void ABuster::BeginPlay()
{
	AActor::BeginPlay();

	Renderer = CreateImageRenderer(static_cast<int>(ERenderOrder::Buster));
	Renderer->SetImage("x_Buster_Default_Right.png");
	UWindowImage* Image = Renderer->GetImage();
	FVector ImageScale = Image->GetScale();
	Renderer->SetTransform({ {0, 0}, {ImageScale.X, (ImageScale.Y) * 3} }); // test

	Renderer->CreateAnimation("Buster_Default_Right", "x_Buster_Default_Right.png", 0, 4, 0.05f, true);
	Renderer->CreateAnimation("Buster_Default_Left", "x_Buster_Default_Left.png", 0, 4, 0.05f, true);
	Renderer->CreateAnimation("Buster_Middle_Right", "x_Buster_Middle_Right.png", 0, 3, 0.05f, true);
	Renderer->CreateAnimation("Buster_Middle_Left", "x_Buster_Middle_Left.png", 0, 3, 0.05f, true);
	Renderer->CreateAnimation("Buster_Pull_Right", "x_Buster_Pull_Right.png", 0, 3, 0.05f, true);
	Renderer->CreateAnimation("Buster_Pull_Left", "x_Buster_Pull_Left.png", 0, 3, 0.05f, true);
		
	BusterCollision = CreateCollision(ECollisionOrder::Weapon);
	BusterCollision->SetScale(ImageScale);
	BusterCollision->SetColType(ECollisionType::CirCle);

	//StateChange(EBusterState::CreateBuster);
} // end BeginPlay()

void ABuster::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	StateUpdate(_DeltaTime);
}


// BeginPlay Functions
void ABuster::StateChange(EBusterState _State)
{
	// 상태가 변했다면,
	if (E_BusterState != _State)
	{
		switch (_State)
		{
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


#pragma region Start Function

void ABuster::BusterCrashStart()
{
	int a = 0;
}

void ABuster::BusterEndStart()
{
	int a = 0;
}

#pragma endregion



void ABuster::DefaultBuster(float _DeltaTime)
{
	BusterLifeTime += _DeltaTime;
	AActor::AddActorLocation(Dir * BusterSpeed);

	if (BusterLifeTime >= BusterLife)
	{
		BusterLifeTime = 0.0f;
		StateChange(EBusterState::BusterEnd);
		return;
	}
}

void ABuster::MiddleCharge(float _DeltaTime)
{
	BusterLifeTime += _DeltaTime;
	AActor::AddActorLocation(Dir * BusterSpeed);
	if (BusterLifeTime >= BusterLife)
	{
		BusterLifeTime = 0.0f;
		StateChange(EBusterState::BusterEnd);
		return;
	}
}

void ABuster::PullCharge(float _DeltaTime)
{
	BusterLifeTime += _DeltaTime;
	AActor::AddActorLocation(Dir * BusterSpeed);
	if (BusterLifeTime >= BusterLife)
	{
		BusterLifeTime = 0.0f;
		StateChange(EBusterState::BusterEnd);
		return;
	}
}

// 충돌
void ABuster::BusterCrash(float _DeltaTime)
{
}

void ABuster::BusterEnd(float _DeltaTime)
{
	int a = 0;
	this->Destroy(0.0f);
}

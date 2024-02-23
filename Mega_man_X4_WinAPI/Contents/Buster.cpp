#include "Buster.h"
#include <EngineCore/EngineCore.h> // Helper

ABuster::ABuster()
{
}

ABuster::~ABuster()
{
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

	//Renderer->ChangeAnimation("Buster_Default_Right");

	
	BusterCollision = CreateCollision(ERenderOrder::Buster);
	BusterCollision->SetScale(ImageScale);
	BusterCollision->SetColType(ECollisionType::CirCle);

	StateChange(EBusterState::DefaultCharge);
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
	if (BusterState != _State)
	{
		switch (_State)
		{
		case EBusterState::DefaultCharge :
			DefaultBusterStart();
			break;
		case EBusterState::MiddleCharge:
			MiddleChargeStart();
			break;
		case EBusterState::PullCharge:
			PullchargeStart();
			break;
		case EBusterState::BusterCollision:
			BusterCrashStart();
			break;
		case EBusterState::BusterEnd:
			BusterEndStart();
			break;
		default:
			break;
		}
	}

	BusterState = _State;
}

// Tick Functions
void ABuster::StateUpdate(float _DeltaTime)
{
	switch (BusterState)
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
	case EBusterState::BusterCollision:
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

void ABuster::DefaultBusterStart()
{
	Renderer->ChangeAnimation("Buster_Default_Right");
}

void ABuster::MiddleChargeStart()
{
}

void ABuster::PullchargeStart()
{
}

void ABuster::BusterCrashStart()
{
}

void ABuster::BusterEndStart()
{
}

#pragma endregion




void ABuster::DefaultBuster(float _DeltaTime)
{
	BusterLifeTime += _DeltaTime;
	AActor::AddActorLocation(Dir * BusterSpeed);

	if (BusterLifeTime >= 2.0f)
	{
		BusterLifeTime = 0.0f;
		StateChange(EBusterState::BusterEnd);
		return;
	}
}

void ABuster::MiddleCharge(float _DeltaTime)
{
}

void ABuster::PullCharge(float _DeltaTime)
{
}

void ABuster::BusterCrash(float _DeltaTime)
{
}

void ABuster::BusterEnd(float _DeltaTime)
{
	int a = 0;
	//this->Destroy(0.0f);
}

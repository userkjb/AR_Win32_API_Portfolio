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
	
	UImageRenderer* Renderer = CreateImageRenderer(static_cast<int>(ERenderOrder::Buster));
	Renderer->SetImage("x_Buster_Default_Right.png");
	UWindowImage* Image = Renderer->GetImage();
	FVector ImageScale = Image->GetScale();
	Renderer->SetTransform({ {0, 0}, {ImageScale.X, (ImageScale.Y) * 3} }); // test

	Renderer->CreateAnimation("Buster_Default_Right", "x_Buster_Default_Right.png", 0, 4, 0.05f, true);
	Renderer->CreateAnimation("Buster_Default_Left", "x_Buster_Default_Left.png", 0, 4, 0.05f, true);

	//Renderer->ChangeAnimation("Buster_Default_Right");

	StateChange(EBusterState::CreateBuster);
	//Destroy(time);

	BusterCollision = CreateCollision(ERenderOrder::Buster);
	BusterCollision->SetScale(ImageScale);
	BusterCollision->SetColType(ECollisionType::CirCle);
}

void ABuster::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	StateUpdate(_DeltaTime);
	//AActor::AddActorLocation(Dir * BusterSpeed);
}

void ABuster::StateChange(EBusterState _State)
{
	// 상태가 변했다면,
	if (State != _State)
	{
		switch (_State)
		{
		case EBusterState::Default :
			DefaultBusterStart();
			break;
		case EBusterState::MiddleCharge:
			break;
		case EBusterState::PullCharge:
			break;
		default:
			break;
		}
	}

	State = _State;
}

void ABuster::StateUpdate(float _DeltaTime)
{
	switch (State)
	{
	case EBusterState::Default:

		break;
	case EBusterState::MiddleCharge:

		break;
	case EBusterState::PullCharge:

		break;
	default:
		break;
	}
}




void ABuster::DefaultBusterStart()
{

}

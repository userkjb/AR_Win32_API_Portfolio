#include "Buster.h"

ABuster::ABuster()
{
}

ABuster::~ABuster()
{
}

void ABuster::BeginPlay()
{
	AActor::BeginPlay();

	UImageRenderer* Renderer = CreateImageRenderer(2);
	Renderer->SetImage("x_Buster_Default_Right.png");
	UWindowImage* Image = Renderer->GetImage();
	FVector ImageScale = Image->GetScale();
	Renderer->SetTransform({ {0, 0}, {ImageScale.X, (ImageScale.Y) * 3} }); // test

	Renderer->CreateAnimation("Buster_Default_Right", "x_Buster_Default_Right.png", 0, 4, 0.05f, true);
	Renderer->CreateAnimation("Buster_Default_Left", "x_Buster_Default_Left.png", 0, 4, 0.05f, true);

	Renderer->ChangeAnimation("Buster_Default_Right");
}

void ABuster::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	AActor::AddActorLocation(Dir * BusterSpeed);
}

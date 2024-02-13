#include "BackGroundMap.h"
#include <EngineCore/EngineResourcesManager.h>

ABackGroundMap::ABackGroundMap()
{
}

ABackGroundMap::~ABackGroundMap()
{
}

void ABackGroundMap::SetMapImage(std::string_view _MapImageName)
{
	Renderer->SetImage(_MapImageName);
	UWindowImage* Image = Renderer->GetImage();
	FVector ImageScale = Image->GetScale();
	Renderer->SetTransform({ ImageScale*1.25f, ImageScale * 2.5f });
}

void ABackGroundMap::SetMenuImage(std::string_view _LogoImageName)
{
	LogoRenderer->SetImage(_LogoImageName);
	UWindowImage* Image = LogoRenderer->GetImage();
	FVector ImageScale = Image->GetScale();
	LogoRenderer->SetTransform({ {ImageScale.X + 140, ImageScale.Y + 50}, ImageScale * 2.5f });
}

void ABackGroundMap::BeginPlay()
{
	// Render 순서 설정
	Renderer = CreateImageRenderer(0);
	//LogoRenderer = CreateImageRenderer(1);
}

void ABackGroundMap::Tick(float _DeltaTime)
{
}

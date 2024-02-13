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
	Renderer->SetTransform({ ImageScale.Half2D(), ImageScale });
}

void ABackGroundMap::BeginPlay()
{
	// Render ���� ����
	Renderer = CreateImageRenderer(0);
}

void ABackGroundMap::Tick(float _DeltaTime)
{
}

#include "CyberSpaceMap.h"
#include "ContentsGlobalData.h"

ACyberSpaceMap::ACyberSpaceMap()
{
}

ACyberSpaceMap::~ACyberSpaceMap()
{
}

void ACyberSpaceMap::SetMapImage(std::string_view _MapImageName)
{
	MapRenderer->SetImage(_MapImageName);
	UWindowImage* Image = MapRenderer->GetImage();
	FVector ImageScale = Image->GetScale();
	MapRenderer->SetTransform({ ImageScale.Half2D(), ImageScale });
}

void ACyberSpaceMap::SetColMapImage(std::string_view _MapImageName)
{
	ColRenderer->SetImage(_MapImageName);
	UWindowImage* Image = ColRenderer->GetImage();
	FVector ImageScale = Image->GetScale();
	UContentsGlobalData::ColMapImage = Image;
	ColRenderer->SetTransform({ ImageScale.Half2D(), ImageScale });
}

void ACyberSpaceMap::BeginPlay()
{
	AActor::BeginPlay();

	MapRenderer = CreateImageRenderer(1);
	ColRenderer = CreateImageRenderer(0);
	ColRenderer->SetActive(false);
}

void ACyberSpaceMap::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
}

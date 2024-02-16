#include "IntroStageMap.h"
#include <EngineCore/EngineCore.h>
#include "ContentsGlobalData.h"

AIntroStageMap::AIntroStageMap()
{
}

AIntroStageMap::~AIntroStageMap()
{
}

void AIntroStageMap::SetMapImage(std::string_view _MapImageName)
{
	Renderer->SetImage(_MapImageName);
	UWindowImage* Image = Renderer->GetImage();
	FVector ImageScale = Image->GetScale();
	Renderer->SetTransform({ ImageScale.Half2D(), ImageScale});
}

void AIntroStageMap::SetColMapImage(std::string_view _MapImageName)
{
	ColRenderer->SetImage(_MapImageName);
	UWindowImage* Image = ColRenderer->GetImage();
	UContentsGlobalData::ColMapImage = Image;
	FVector ImageScale = Image->GetScale();
	ColRenderer->SetTransform({ ImageScale.Half2D(), ImageScale });
	ColRenderer->SetActive(false);
}

void AIntroStageMap::SwitchDebug()
{
	if (true == Renderer->IsActive())
	{
		Renderer->SetActive(false);
		ColRenderer->SetActive(true);
	}
	else
	{
		Renderer->SetActive(true);
		ColRenderer->SetActive(false);
	}
}



void AIntroStageMap::BeginPlay()
{
	AActor::BeginPlay();

	Renderer = CreateImageRenderer(1);
	ColRenderer = CreateImageRenderer(0);
	ColRenderer->SetActive(false);
}

void AIntroStageMap::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	if (true == UEngineInput::IsDown('O'))
	{
		SwitchDebug();
	}
}

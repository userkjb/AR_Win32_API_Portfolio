#include "BackGroundMap.h"
#include <EngineCore/EngineResourcesManager.h>
#include <EngineBase/EngineDebug.h>

// Helper
#include <EngineCore/EngineCore.h>

ABackGroundMap::ABackGroundMap()
{
}

ABackGroundMap::~ABackGroundMap()
{
}

void ABackGroundMap::SetMapImage(std::string_view _MapImageName)
{
	TitleRenderer->SetImage(_MapImageName);
	UWindowImage* Image = TitleRenderer->GetImage();
	FVector ImageScale = Image->GetScale();
	TitleRenderer->SetTransform({ ImageScale*1.25f, ImageScale * 2.5f });
}

void ABackGroundMap::SetMenuImage(std::string_view _LogoImageName)
{
	MenuRenderer->SetImage(_LogoImageName);
	UWindowImage* Image = MenuRenderer->GetImage();
	FVector ImageScale = Image->GetScale();
	MenuRenderer->SetTransform({ {ImageScale.X, ImageScale.Y}, ImageScale });
}

void ABackGroundMap::SetGameStartTextImage(std::string_view _ImageName)
{
	GameStartTextRenderer->SetImage(_ImageName);
	UWindowImage* Image = GameStartTextRenderer->GetImage();
	FVector ImageScale = Image->GetScale();
	GameStartTextRenderer->SetTransform({ {(ImageScale.X * 2.0f) + 100.0f, (ImageScale.Y * 2.0f) + 350.0f}, ImageScale * 2.0f });
}

void ABackGroundMap::BeginPlay()
{
	AActor::BeginPlay();

	// Render 순서 설정
	TitleRenderer = CreateImageRenderer(0);
	//MenuRenderer = CreateImageRenderer(1);
	GameStartTextRenderer = CreateImageRenderer(2);
}

void ABackGroundMap::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	if (UEngineInput::IsDown('O'))
	{
		GameStartTextRenderer->ActiveOff();
	}
}

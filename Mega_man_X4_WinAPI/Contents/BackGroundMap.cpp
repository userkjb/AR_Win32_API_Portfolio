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
	FVector ShowPosition = { 400, 400 };
	MenuRenderer->SetImage(_LogoImageName);
	UWindowImage* Image = MenuRenderer->GetImage();
	FVector ImageScale = Image->GetScale();
	MenuRenderer->SetTransform({ ShowPosition, {ImageScale.X / 3, ImageScale.Y} });
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
	GameStartTextRenderer = CreateImageRenderer(1);
	GameStartTextRenderer->SetActive(false); // test

	MenuRenderer = CreateImageRenderer(1);
}

void ABackGroundMap::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	if (true == UEngineInput::IsDown('O')) // Enter 키 수정 예정.
	{
		MenuChange();
		EnterKey = true;
	}

	if (EnterKey == true)
	{
		if (true == UEngineInput::IsDown(38)) // Up
		{

		}
		else if (true == UEngineInput::IsDown(40)) // Down
		{

		}
	}
}

void ABackGroundMap::MenuChange()
{
	if (true == GameStartTextRenderer->IsActive())
	{
		GameStartTextRenderer->SetActive(false);
		MenuRenderer->SetActive(true);
	}
}

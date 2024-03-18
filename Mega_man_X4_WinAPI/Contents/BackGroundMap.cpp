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

void ABackGroundMap::SetGameStartTextImage(std::string_view _ImageName)
{
	GameStartTextRenderer->SetImage(_ImageName);
	UWindowImage* Image = GameStartTextRenderer->GetImage();
	FVector ImageScale = Image->GetScale();
	GameStartTextRenderer->SetTransform({ {(ImageScale.X * 2.0f) + 100.0f, (ImageScale.Y * 2.0f) + 350.0f}, ImageScale * 2.0f });
}

void ABackGroundMap::SetMenuAnimation(std::string_view _LogoImageName)
{
	FVector ShowPosition = { 400, 400 };

	MenuRenderer->SetImage(_LogoImageName);
	UWindowImage* Image = MenuRenderer->GetImage();
	FVector ImageScale = Image->GetScale();
	MenuRenderer->SetTransform({ ShowPosition, {ImageScale.X / 3, ImageScale.Y} });

	MenuRenderer->CreateAnimation("Menu_1", _LogoImageName, 0, 0, 0.1f, false);
	MenuRenderer->CreateAnimation("Menu_2", _LogoImageName, 1, 1, 0.1f, false);
	MenuRenderer->CreateAnimation("Menu_3", _LogoImageName, 2, 2, 0.1f, false);
	
	MenuRenderer->ChangeAnimation("Menu_1");
}

void ABackGroundMap::BeginPlay()
{
	AActor::BeginPlay();

	// Render 순서 설정
	TitleRenderer = CreateImageRenderer(0);
	GameStartTextRenderer = CreateImageRenderer(1);

	MenuRenderer = CreateImageRenderer(1);
	MenuRenderer->SetActive(false);
}

void ABackGroundMap::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	if (true == UEngineInput::IsDown(VK_RETURN) && false == EnterKey) // Enter 키 수정 예정.
	{
		GameStartTextRenderer->SetActive(false);
		MenuRenderer->SetActive(true);
		EnterKey = true;
	}

	if (EnterKey == true)
	{
		if (true == UEngineInput::IsDown(VK_UP)) // Up
		{
			KeyCount--;
			if (KeyCount <= 0)
			{
				KeyCount = 1;
			}
		}
		else if (true == UEngineInput::IsDown(VK_DOWN)) // Down
		{
			KeyCount++;
			if (KeyCount > 3)
			{
				KeyCount = 3;
			}
		}

		switch (KeyCount)
		{
		case 1 :
			MenuRenderer->ChangeAnimation("Menu_1");
			UEngineDebug::OutPutDebugText(std::to_string(KeyCount));
			break;
		case 2 :
			MenuRenderer->ChangeAnimation("Menu_2");
			UEngineDebug::OutPutDebugText(std::to_string(KeyCount));
			break;
		case 3 :
			MenuRenderer->ChangeAnimation("Menu_3");
			UEngineDebug::OutPutDebugText(std::to_string(KeyCount));
			break;
		default :
			break;
		}
	}

	if (true == UEngineInput::IsDown('Q'))
	{
		GEngine->ChangeLevel("CyberSpaceLevel");
	}
}


// GEngine->ChangeLevel("IntroLevel");
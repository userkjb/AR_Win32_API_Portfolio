#include "BackGroundMap.h"
#include <EngineCore/EngineResourcesManager.h>
#include <EngineBase/EngineDebug.h>
#include <EnginePlatform/EngineSound.h>

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

	StateChange(EBackGroundMapState::None);

	// Render 순서 설정
	TitleRenderer = CreateImageRenderer(0);
	GameStartTextRenderer = CreateImageRenderer(1);

	MenuRenderer = CreateImageRenderer(1);
	MenuRenderer->SetActive(false);
}

void ABackGroundMap::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	StateUpdate(_DeltaTime);
}

void ABackGroundMap::StateChange(EBackGroundMapState _State)
{
	if (State != _State)
	{
		switch (_State)
		{
		case EBackGroundMapState::None:
			break;
		case EBackGroundMapState::Title:
			TitleStart();
			break;
		case EBackGroundMapState::GameMenu:
			GameMenuStart();
			break;
		default :
			break;
		}
	}
	State = _State;
}

void ABackGroundMap::StateUpdate(float _DeltaTime)
{
	switch (State)
	{
	case EBackGroundMapState::None:
		break;
	case EBackGroundMapState::Title:
		Title(_DeltaTime);
		break;
	case EBackGroundMapState::GameMenu:
		GameMenu(_DeltaTime);
		break;
	default:
		break;
	}
}


void ABackGroundMap::TitleStart()
{
	if (false == GameStartTextRenderer->IsActive())
	{
		GameStartTextRenderer->SetActive(true);
	}
	if (true == MenuRenderer->IsActive())
	{
		MenuRenderer->SetActive(false);
	}
}

void ABackGroundMap::Title(float _DeltaTime)
{
	if (true == UEngineInput::IsDown(VK_RETURN) || true == UEngineInput::IsDown(VK_SPACE))
	{
		UEngineSound::SoundPlay("SelectFocus.mp3");
		StateChange(EBackGroundMapState::GameMenu);
		return;
	}
}

void ABackGroundMap::GameMenuStart()
{
	if (true == GameStartTextRenderer->IsActive())
	{
		GameStartTextRenderer->SetActive(false);
	}
	if (false == MenuRenderer->IsActive())
	{
		MenuRenderer->SetActive(true);
	}

	KeyCount = 1;
}

void ABackGroundMap::GameMenu(float _DeltaTime)
{
	if (true == UEngineInput::IsDown(VK_UP)) // Up
	{
		UEngineSound::SoundPlay("SelectFocus.mp3");
		KeyCount--;
		if (KeyCount <= 0)
		{
			KeyCount = 1;
		}
	}
	else if (true == UEngineInput::IsDown(VK_DOWN)) // Down
	{
		UEngineSound::SoundPlay("SelectFocus.mp3");
		KeyCount++;
		if (KeyCount > 3)
		{
			KeyCount = 3;
		}
	}

	switch (KeyCount)
	{
	case 1:
		MenuRenderer->ChangeAnimation("Menu_1");
		UEngineDebug::OutPutDebugText(std::to_string(KeyCount));
		break;
	case 2:
		MenuRenderer->ChangeAnimation("Menu_2");
		UEngineDebug::OutPutDebugText(std::to_string(KeyCount));
		break;
	case 3:
		MenuRenderer->ChangeAnimation("Menu_3");
		UEngineDebug::OutPutDebugText(std::to_string(KeyCount));
		break;
	default:
		break;
	}

	if (true == UEngineInput::IsDown(VK_RETURN) || true == UEngineInput::IsDown(VK_SPACE))
	{
		UEngineSound::SoundPlay("Select.mp3");
		GEngine->ChangeLevel("CyberSpaceLevel");
	}
}


// GEngine->ChangeLevel("IntroLevel");
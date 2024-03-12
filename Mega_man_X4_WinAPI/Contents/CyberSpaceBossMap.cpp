#include "CyberSpaceBossMap.h"
#include "ContentsGlobalData.h"
#include <EngineCore/EngineResourcesManager.h> // test


ACyberSpaceBossMap::ACyberSpaceBossMap()
{
}

ACyberSpaceBossMap::~ACyberSpaceBossMap()
{
}

void ACyberSpaceBossMap::BeginPlay()
{
	AActor::BeginPlay();

	MapRenderer = CreateImageRenderer(static_cast<int>(ERenderOrder::Map));
	MapColRenderer = CreateImageRenderer(static_cast<int>(ERenderOrder::Map));
	MapColRenderer->SetActive(false);
	BossDoor_1 = CreateImageRenderer(static_cast<int>(ERenderOrder::MapObject));
	BossDoor_2 = CreateImageRenderer(static_cast<int>(ERenderOrder::MapObject));
}

void ACyberSpaceBossMap::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	if (true == UEngineInput::IsDown('O'))
	{
		SwitchDebug();
	}
}

void ACyberSpaceBossMap::SetMapImage(std::string_view _MapImageName)
{
	MapRenderer->UImageRenderer::SetImage(_MapImageName);
	UWindowImage* Image = MapRenderer->GetImage();
	ImageScale = Image->GetScale();
	MapRenderer->SetTransform({ ImageScale.Half2D(), ImageScale });
}

void ACyberSpaceBossMap::SetMapColImage(std::string_view _MapColImageName)
{
	MapColRenderer->SetImage(_MapColImageName);
	UWindowImage* Image = MapColRenderer->GetImage();
	ImageScale = Image->GetScale();
	UContentsGlobalData::ColMapImage = Image;
	MapColRenderer->SetTransform({ ImageScale.Half2D(), ImageScale });
}

void ACyberSpaceBossMap::SetDoorImage(std::string_view _DoorImageName)
{
	BossDoor_1->SetImage(_DoorImageName);
	UWindowImage* Image = BossDoor_1->GetImage();
	//ImageScale = Image->GetScale();
	//BossDoor_1->SetTransform({ ImageScale.Half2D(), ImageScale });
	//BossDoor_1->SetPosition({ 1080, 480 });
	BossDoor_1->AutoImageScale(2.5f);
	BossDoor_1->SetPosition(BossDoorPos_1);

	BossDoor_2->SetImage(_DoorImageName);
	UWindowImage* Image_2 = BossDoor_2->GetImage();
	//ImageScale = Image_2->GetScale();
	//BossDoor_2->SetTransform({ ImageScale.Half2D(), ImageScale });
	//BossDoor_2->SetPosition({ 1840, 480 });
	BossDoor_2->AutoImageScale(2.5f);
	BossDoor_2->SetPosition(BossDoorPos_2);

	CreateDoorAni();
}

void ACyberSpaceBossMap::CreateDoorAni()
{

}

void ACyberSpaceBossMap::SwitchDebug()
{
	if (true == MapRenderer->IsActive())
	{
		MapRenderer->SetActive(false);
		MapColRenderer->SetActive(true);
	}
	else
	{
		MapRenderer->SetActive(true);
		MapColRenderer->SetActive(false);
	}
}

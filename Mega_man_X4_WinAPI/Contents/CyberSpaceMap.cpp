#include "CyberSpaceMap.h"
#include "ContentsGlobalData.h"
#include "Egseu.h"
#include "PlayerFocus.h"

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
	ImageScale = Image->GetScale();
	MapRenderer->SetTransform({ ImageScale.Half2D(), ImageScale });
}

void ACyberSpaceMap::SetColMapImage(std::string_view _MapImageName)
{
	ColRenderer->SetImage(_MapImageName);
	UWindowImage* Image = ColRenderer->GetImage();
	ImageScale = Image->GetScale();
	UContentsGlobalData::ColMapImage = Image;
	ColRenderer->SetTransform({ ImageScale.Half2D(), ImageScale });
}

void ACyberSpaceMap::SetMapExitImage(std::string_view _MapImageName)
{
	MapExit->SetImage(_MapImageName);
	MapExit->AutoImageScale(); // test
	MapExit->SetPosition({ 4817, 300 });
}

void ACyberSpaceMap::SwitchDebug()
{
	if (true == MapRenderer->IsActive())
	{
		MapRenderer->SetActive(false);
		ColRenderer->SetActive(true);
	}
	else
	{
		MapRenderer->SetActive(true);
		ColRenderer->SetActive(false);
	}
}

void ACyberSpaceMap::BeginPlay()
{
	AActor::BeginPlay();

	MapRenderer = CreateImageRenderer(static_cast<int>(ERenderOrder::Map));
	ColRenderer = CreateImageRenderer(static_cast<int>(ERenderOrder::Map));
	ColRenderer->SetActive(false);
	MapExit = CreateImageRenderer(static_cast<int>(ERenderOrder::MapObject));
}

void ACyberSpaceMap::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	if (true == UEngineInput::IsDown('O'))
	{
		SwitchDebug();
	}

	PlayerPosEvent(_DeltaTime);
}

void ACyberSpaceMap::PlayerPosEvent(float _DeltaTime)
{
	FVector PlayerPos = Player->GetActorLocation();
	UEngineDebug::OutPutDebugText(std::to_string(PlayerPos.X));
	
	//----- ��Ŀ�� ----------
	// 1. ��ǥ.
	// 2. �ȼ�.
	// ��ǥ�� ���� ���.
	// �Ʒ��� ���ǿ� �����ϴ� ���
	//if (990.0f <= PlayerPos.X && PlayerPos.X <= 1000.0f) // ���ӿ��� ��Ŀ���� ����� ��ǥ.
	if (300.0f <= PlayerPos.X && PlayerPos.X <= 350.0f) // test
	{
		//FocusTime += _DeltaTime;
		//if (FocusTime <= 2.0f)
		//{
		//	GetWorld()->SetAllTimeScale(0.0f);
		//	GetWorld()->SetOtherTimeScale(ERenderOrder::Player, 0.0f);
		//	return;
		//}
		// ��Ŀ���� ������ ����� �ְ�.
		if (Focus == nullptr)
		{
			Focus = GetWorld()->SpawnActor<APlayerFocus>();
			Focus->SetActorLocation({ PlayerPos.iX(), PlayerPos.iY() - 50 });
			Focus->SetFocusState(EFocusState::CallCreate);
			FocusCount = 0;
		}
	}
	//FocusTime += _DeltaTime;

	// ����.
	if (FocusCount == 0)
	{
		//if (4400.0f <= PlayerPos.X && PlayerPos.X <= 4450.0f) // ����
		if (500.0f <= PlayerPos.X && PlayerPos.X <= 550.0f) // test
		{
			FocusCount++;
			Focus->SetFocusState(EFocusState::Rank);
		}
	}
}

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

void ACyberSpaceMap::SetRankRenderImage(std::string_view _MapImageName)
{
	RankRender->SetImage(_MapImageName);
	MapExit->AutoImageScale();
	
	RankRender->CreateAnimation("B_Rank", _MapImageName, 0, 0, 0.1f, false);
	RankRender->CreateAnimation("A_Rank", _MapImageName, 1, 1, 0.1f, false);
	RankRender->CreateAnimation("S_Rank", _MapImageName, 2, 2, 0.1f, false);

	RankRender->ChangeAnimation("B_Rank");
	RankRender->ActiveOff();
}

void ACyberSpaceMap::SetExitAniImage(std::string_view _MapImageName)
{
	ExitAni->SetImage(_MapImageName);
	ExitAni->AutoImageScale();

	ExitAni->CreateAnimation("ExitAni", _MapImageName, 0, 13, 0.5f, false);

	ExitAni->ChangeAnimation("ExitAni");
	ExitAni->ActiveOff();
}



void ACyberSpaceMap::SwitchDebug()
{
	if (true == MapRenderer->IsActive())
	{
		MapRenderer->SetActive(false);
		MapExit->SetActive(false);
		ColRenderer->SetActive(true);
	}
	else
	{
		MapRenderer->SetActive(true);
		MapExit->SetActive(true);
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

	// Focus
	Focus = GetWorld()->SpawnActor<APlayerFocus>(static_cast<int>(EActorType::MapObject));
	//Focus->ActiveOff();
	Focus->SetActive(false);
	
	// Exit
	RankRender = CreateImageRenderer(static_cast<int>(ERenderOrder::MapObject));
	ExitAni = CreateImageRenderer(static_cast<int>(ERenderOrder::MapObject));
}

void ACyberSpaceMap::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	if (true == UEngineInput::IsDown('O'))
	{
		SwitchDebug();
	}
	StateUpdate(_DeltaTime);
	PlayerPosEvent(_DeltaTime);
}

void ACyberSpaceMap::StateChange(ECyberSpaceMapState _State)
{
	if (State != _State)
	{
		switch (_State)
		{
		case ECyberSpaceMapState::None:
			break;
		case ECyberSpaceMapState::PlayerFocus_Start:
			PlayerFocus_StartBegin();
			break;
		case ECyberSpaceMapState::PlayerFocus_Loop:
			PlayerFocus_LoopBegin();
			break;
		case ECyberSpaceMapState::PlayerFocus_End:
			PlayerFocus_EndBegin();
			break;
		default :
			break;
		}
	}
	State = _State;
}

void ACyberSpaceMap::StateUpdate(float _DeltaTime)
{
	switch (State)
	{
	case ECyberSpaceMapState::None:
		break;
	case ECyberSpaceMapState::PlayerFocus_Start:
		PlayerFocus_Start(_DeltaTime);
		break;
	case ECyberSpaceMapState::PlayerFocus_Loop:
		PlayerFocus_Loop(_DeltaTime);
		break;
	case ECyberSpaceMapState::PlayerFocus_End:
		PlayerFocus_End(_DeltaTime);
		break;
	default:
		break;
	}
}


//==================================================================================

void ACyberSpaceMap::PlayerFocus_StartBegin()
{
	FocusCreateTime = 0.0f;
	
	// 이 시간 동안 포커스를 제외한 모든 Actor들은 멈춰야 한다.
	GetWorld()->SetAllTimeScale(0.0f);
	GetWorld()->SetTimeScale(EActorType::Map, 1.0f);
	//GetWorld()->SetTimeScale(static_cast<int>(EActorType::MapObject), 1.0f);
	
	Focus->SetFocusState(EFocusState::CallCreate);
	Focus->ActiveOn();

	Player->StateChange(EEgseuState::FocusCreate);
}

void ACyberSpaceMap::PlayerFocus_Start(float _DeltaTime)
{
	if (FocusCreateTime <= 2.0f)
	{
		FocusCreateTime += _DeltaTime;

		FVector PlayerPos = Player->GetActorLocation();
		Focus->SetActorLocation({ PlayerPos.iX(), PlayerPos.iY() - 50 });
	}
	else
	{
		Player->StateChange(EEgseuState::FocusLoop);
		Focus->SetFocusState(EFocusState::CallRun);
		StateChange(ECyberSpaceMapState::PlayerFocus_Loop);
		return;
	}
}

void ACyberSpaceMap::PlayerFocus_LoopBegin()
{
	GetWorld()->SetAllTimeScale(1.0f);
	FocusTime = 0.0f;
}

void ACyberSpaceMap::PlayerFocus_Loop(float _DeltaTime)
{
	FocusTime += _DeltaTime;
}

void ACyberSpaceMap::PlayerFocus_EndBegin()
{
	Player->StateChange(EEgseuState::FocusEnd);
	Focus->SetFocusState(EFocusState::Rank);
}

void ACyberSpaceMap::PlayerFocus_End(float _DeltaTime)
{
	float x = FocusTime;

	//bool nbx = Focus->IsDestroy();
	if (Focus->IsDestroy())
	{
		if(x <= 10.0f)
		{
			// S
			Player->AutoRightRun = true;
		}
		else if (10.0f < x || x <= 15.0f)
		{
			// A
			Player->AutoRightRun = true;
		}
		else if (x > 15.0f)
		{
			// B
			Player->AutoRightRun = true;
		}
	}
}


/// <summary>
/// 플레이어 좌표 이벤트
/// </summary>
/// <param name="_DeltaTime"></param>
void ACyberSpaceMap::PlayerPosEvent(float _DeltaTime)
{
	FVector PlayerPos = Player->GetActorLocation();
	//UEngineDebug::OutPutDebugText(std::to_string(PlayerPos.X));
	
	if (State == ECyberSpaceMapState::None)
	{
		//if (300.0f <= PlayerPos.X && PlayerPos.X <= 350.0f) // test
		if (990.0f <= PlayerPos.X && PlayerPos.X <= 1000.0f) // 게임에서 포커스가 생기는 좌표.
		{
			StateChange(ECyberSpaceMapState::PlayerFocus_Start);
			return;
		}
	}
	
	// 정산.
	if (State == ECyberSpaceMapState::PlayerFocus_Loop)
	{
		//if (500.0f <= PlayerPos.X && PlayerPos.X <= 550.0f) // test
		if (4400.0f <= PlayerPos.X && PlayerPos.X <= 4450.0f) // 정산
		{
			StateChange(ECyberSpaceMapState::PlayerFocus_End);
			return;
		}
	}
}
#include "CyberSpaceBossMap.h"
#include "ContentsGlobalData.h"
#include "Egseu.h"
#include "CyberPeacock.h"

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
	WhiteRender = CreateImageRenderer(static_cast<int>(ERenderOrder::WhiteImage));
	BossDoor_1 = CreateImageRenderer(static_cast<int>(ERenderOrder::MapObject));
	BossDoor_2 = CreateImageRenderer(static_cast<int>(ERenderOrder::MapObject));

	StateChange(ECyberBossMapState::Idle);
}

void ACyberSpaceBossMap::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	if (true == UEngineInput::IsDown('O'))
	{
		SwitchDebug();
	}

	StateUpdate(_DeltaTime);
	CollisionCheck(_DeltaTime);
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
	//UWindowImage* Image = BossDoor_1->GetImage();
	//ImageScale = Image->GetScale();
	//BossDoor_1->SetTransform({ ImageScale.Half2D(), ImageScale });
	//BossDoor_1->SetPosition({ 1080, 480 });
	BossDoor_1->AutoImageScale(2.5f);
	BossDoor_1->SetPosition(BossDoorPos_1);

	BossDoor_2->SetImage(_DoorImageName);
	//UWindowImage* Image_2 = BossDoor_2->GetImage();
	//ImageScale = Image_2->GetScale();
	//BossDoor_2->SetTransform({ ImageScale.Half2D(), ImageScale });
	//BossDoor_2->SetPosition({ 1840, 480 });
	BossDoor_2->AutoImageScale(2.5f);
	BossDoor_2->SetPosition(BossDoorPos_2);

	BossDoor_Coll_1 = CreateCollision(ECollisionOrder::BossDoor);
	BossDoor_Coll_1->SetColType(ECollisionType::Rect);
	BossDoor_Coll_1->SetScale({75, 160});
	BossDoor_Coll_1->SetPosition(BossDoor_1->GetPosition());

	BossDoor_Coll_2 = CreateCollision(ECollisionOrder::BossDoor);
	BossDoor_Coll_2->SetColType(ECollisionType::Rect);
	BossDoor_Coll_2->SetScale({ 75, 160 });
	BossDoor_Coll_2->SetPosition(BossDoor_2->GetPosition());

	CreateDoorAni();
}

void ACyberSpaceBossMap::SetWhiteImage(std::string_view _WhiteImageName)
{
	WhiteRender->SetImage(_WhiteImageName);
	WhiteRender->AutoImageScale(2.0f);
	WhiteRender->SetPosition({2500.0f, 100.0f});
	WhiteRender->SetAlpha(0.0f);
}

void ACyberSpaceBossMap::CreateDoorAni()
{
	BossDoor_1->CreateAnimation("Idle", "mmx4-bossdoor.png", 0, 0, 0.5f, true);
	BossDoor_1->CreateAnimation("Open", "mmx4-bossdoor.png", { 0, 1, 2, 3, 4, 4, 3, 2, 1, 0, 5, 6, 7, 8, 9, 0, 5, 6, 7, 8, 9, 0, 10, 11, 12 }, 0.05f, false);
	BossDoor_1->CreateAnimation("Close", "mmx4-bossdoor.png", { 12, 12, 10, 9, 9, 0, 1, 2, 3, 4, 4, 3, 2, 1, 0, 5, 6, 7, 8, 9, 0, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 4, 3, 2, 1, 0 }, 0.05f, false);

	BossDoor_2->CreateAnimation("Idle", "mmx4-bossdoor.png", 0, 0, 0.5f, true);
	BossDoor_2->CreateAnimation("Open", "mmx4-bossdoor.png", { 0, 1, 2, 3, 4, 4, 3, 2, 1, 0, 5, 6, 7, 8, 9, 0, 5, 6, 7, 8, 9, 0, 10, 11, 12 }, 0.05f, false);
	BossDoor_2->CreateAnimation("Close", "mmx4-bossdoor.png", { 12, 12, 10, 9, 9, 0, 1, 2, 3, 4, 4, 3, 2, 1, 0, 5, 6, 7, 8, 9, 0, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 4, 3, 2, 1, 0 }, 0.05f, false);

	BossDoor_1->ChangeAnimation("Idle");
	BossDoor_2->ChangeAnimation("Idle");
}

void ACyberSpaceBossMap::StateChange(ECyberBossMapState _State)
{
	if (State != _State)
	{
		switch (_State)
		{
		case ECyberBossMapState::Idle:
			IdleStart();
			break;
		case ECyberBossMapState::SlowMove:
			SlowMoveStart();
			break;
		case ECyberBossMapState::CheckPointRoom:
			CheckPointRoomStart();
			break;
		case ECyberBossMapState::BossSlowMove:
			BossSlowMoveStart();
			break;
		case ECyberBossMapState::BossRoom:
			BossRoomStart();
			break;
		case ECyberBossMapState::White:
			WhiteStart();
			break;
		case ECyberBossMapState::Restore:
			RestoreStart();
			break;
		default :
			break;
		}
	}
	State = _State;
}

void ACyberSpaceBossMap::StateUpdate(float _DeltaTime)
{
	switch (State)
	{
	case ECyberBossMapState::Idle:
		Idle(_DeltaTime);
		break;
	case ECyberBossMapState::SlowMove:
		SlowMove(_DeltaTime);
		break;
	case ECyberBossMapState::CheckPointRoom:
		CheckPointRoom(_DeltaTime);
		break;
	case ECyberBossMapState::BossSlowMove:
		BossSlowMove(_DeltaTime);
		break;
	case ECyberBossMapState::BossRoom:
		BossRoom(_DeltaTime);
		break;
	case ECyberBossMapState::White:
		White(_DeltaTime);
		break;
	case ECyberBossMapState::Restore:
		Restore(_DeltaTime);
		break;
	default:
		break;
	}
}

void ACyberSpaceBossMap::IdleStart()
{
	// 적어도 한 번은 탄다.
}

void ACyberSpaceBossMap::Idle(float _DeltaTime)
{
	// 평상시.
	if (Player == nullptr)
	{
		Player = AEgseu::GetMainPlayer();
	}
	if (CyberBoss == nullptr)
	{
		CyberBoss = ACyberPeacock::GetMainBoss();
	}
}

// 앞 문 충돌.
void ACyberSpaceBossMap::SlowMoveStart()
{
	BossDoor_1->ChangeAnimation("Open");
	FrontDoor = true; // Level
}

void ACyberSpaceBossMap::SlowMove(float _DeltaTime)
{
	// Collision에서 상태가 바뀌어서 들어옴.
	// 문이 다 열림.
	if (true == BossDoor_1->IsCurAnimationEnd())
	{
		if (FrontDoorOpen == false)
		{
			UEngineSound::SoundPlay("InDoor.mp3");
			FrontDoorOpen = true;
		}

		if (IsFDCollision == false)
		{
			// 문을 통과하면, 다음 상태로 이동.
			StateChange(ECyberBossMapState::CheckPointRoom);
			return;
		}
	}
}

void ACyberSpaceBossMap::CheckPointRoomStart()
{
	BossDoor_1->ChangeAnimation("Close");
	// 체크포인트 수정. - TODO....
}

void ACyberSpaceBossMap::CheckPointRoom(float _DeltaTime)
{
	
}

// 뒷 문 충돌.
void ACyberSpaceBossMap::BossSlowMoveStart()
{
	// Collision에서 상태가 바뀌어서 들어옴.
	BossDoor_2->ChangeAnimation("Open");
	BackDoor = true; // Level
}
void ACyberSpaceBossMap::BossSlowMove(float _DeltaTime)
{
	// 캐릭터가 느릐게 움직이면서 들어감.
	if (true == BossDoor_2->IsCurAnimationEnd())
	{
		//RunVector = FVector::Right * RunSpeed * _DeltaTime;
		//Player->AddActorLocation(RunVector);
		if (BackDoorOpen == false)
		{
			BackDoorOpen = true;
			UEngineSound::SoundPlay("InDoor.mp3");
		}

		// 문을 통과하면,
		if (IsBDCollision == false)
		{
			//FVector CameraPos = GetWorld()->GetCameraPos();
			//다음 상태로 이동.
			StateChange(ECyberBossMapState::BossRoom);
			return;
		}
	}
}

void ACyberSpaceBossMap::BossRoomStart()
{
	BossDoor_2->ChangeAnimation("Close"); // 문 닫힘 애니메이션.
}

// 보스전 Start
void ACyberSpaceBossMap::BossRoom(float _DeltaTime)
{
	int a = 0;
}

#pragma region White - 맵 흰색으로.
void ACyberSpaceBossMap::WhiteStart()
{
	ToWhitTime = 0.0f;
	MapWhite = false;
}
void ACyberSpaceBossMap::White(float _DeltaTime)
{
	ToWhitTime += _DeltaTime;

	float DefaultAlpha = 0.0f;
	float ToWhit = DefaultAlpha + (ToWhitTime * 0.25f);
	WhiteRender->SetAlpha(ToWhit);

	// 전부 진행 되었다면,
	if (ToWhit >= 1.0f && MapWhite == false)
	{
		MapWhite = true;
	}

	//if (ToWhitTime >= 3.0f && MapWhite == false)
	//{
	//	MapWhite = true;
	//}
	//else
	//{
	//	return;
	//}
}
#pragma endregion

#pragma region Restore - 흰색 된 맵을 다시 복구.
void ACyberSpaceBossMap::RestoreStart()
{
	ToOriginalTime = 0.0f;
}
void ACyberSpaceBossMap::Restore(float _DeltaTime)
{
	ToOriginalTime += _DeltaTime;

	float DefaultAlpha = 1.0f;
	float ToWhit = DefaultAlpha - (ToOriginalTime * 0.25f);
	WhiteRender->SetAlpha(ToWhit);

	if (ToWhit <= 0.0f && MapRestore == false)
	{
		MapRestore = true;
	}

	//if (ToOriginalTime >= 2.0f)
	//{
	//	MapRestore = true;
	//}
}
#pragma endregion


// Collicion
void ACyberSpaceBossMap::CollisionCheck(float _DeltaTime)
{
	std::vector<UCollision*> PlayerResult;
	if (true == BossDoor_Coll_1->CollisionCheck(ECollisionOrder::Player, PlayerResult))
	{
		IsFDCollision = true;
		if (CollisionCount_1 == 0)
		{
			CollisionCount_1 = 1;
			//ColPlayer = dynamic_cast<AEgseu*>(PlayerResult[0]->GetOwner()); // 주석 남김.
			StateChange(ECyberBossMapState::SlowMove);
			return;
		}
	}
	else
	{
		IsFDCollision = false;
	}

	if (true == BossDoor_Coll_2->CollisionCheck(ECollisionOrder::Player, PlayerResult))
	{
		IsBDCollision = true;
		if (CollisionCount_2 == 0)
		{
			CollisionCount_2 = 1;
			StateChange(ECyberBossMapState::BossSlowMove);
			return;
		}
	}
	else
	{
		IsBDCollision = false;
	}
}


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
}

void ACyberSpaceBossMap::SlowMoveStart()
{
	BossDoor_1->ChangeAnimation("Open");
}

void ACyberSpaceBossMap::SlowMove(float _DeltaTime)
{
	// Collision에서 상태가 바뀌어서 들어옴.
	// 문이 다 열림.
	if (true == BossDoor_1->IsCurAnimationEnd())
	{
		// 캐릭터 움직이고
	}

	// 문을 통과하면, 다음 상태로 이동.
	//StateChange(ECyberBossMapState::CheckPointRoom);
	//return;
	

	// Test
	//if (true == UEngineInput::IsDown('T'))
	//{
	//	StateChange(ECyberBossMapState::CheckPointRoom);
	//	return;
	//}
}

void ACyberSpaceBossMap::CheckPointRoomStart()
{
	BossDoor_1->ChangeAnimation("Close");
	// 체크포인트 수정.
}

void ACyberSpaceBossMap::CheckPointRoom(float _DeltaTime)
{
	// 딱히 할 것 없음.

	// Test
	//if (true == UEngineInput::IsDown('Y'))
	//{
	//	StateChange(ECyberBossMapState::SlowMove);
	//	return;
	//}
}

void ACyberSpaceBossMap::BossSlowMoveStart()
{
	// Collision에서 상태가 바뀌어서 들어옴.
	BossDoor_2->ChangeAnimation("Open");
}

void ACyberSpaceBossMap::BossSlowMove(float _DeltaTime)
{	
	// 캐릭터가 느릐게 움직이면서 들어감.
}

void ACyberSpaceBossMap::BossRoomStart()
{
	BossDoor_2->ChangeAnimation("Close");
}

void ACyberSpaceBossMap::BossRoom(float _DeltaTime)
{
	// 보스전 Start
}



void ACyberSpaceBossMap::CollisionCheck(float _DeltaTime)
{
	std::vector<UCollision*> PlayerResult;
	if (true == BossDoor_Coll_1->CollisionCheck(ECollisionOrder::Player, PlayerResult))
	{
		if (CollisionCount_1 == 0)
		{
			CollisionCount_1 = 1;

			StateChange(ECyberBossMapState::SlowMove);
			return;
		}
	}

	if (true == BossDoor_Coll_2->CollisionCheck(ECollisionOrder::Player, PlayerResult))
	{
		if (CollisionCount_2 == 0)
		{
			CollisionCount_2 = 1;
			StateChange(ECyberBossMapState::BossSlowMove);
			return;
		}
	}
}

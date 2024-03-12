#include "CyberSpaceBossMap.h"
#include "ContentsGlobalData.h"
#include "Egseu.h"


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
	Player = AEgseu::GetMainPlayer();
}

void ACyberSpaceBossMap::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	if (true == UEngineInput::IsDown('O'))
	{
		SwitchDebug();
	}

	MoveCameraVector(IsBoss);
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
	// ��� �� ���� ź��.
}

void ACyberSpaceBossMap::Idle(float _DeltaTime)
{
	// ����.
	if (Player == nullptr)
	{
		Player = AEgseu::GetMainPlayer();
	}	
}

// �� �� �浹.
void ACyberSpaceBossMap::SlowMoveStart()
{
	BossDoor_1->ChangeAnimation("Open");
	RunVector = FVector::Zero;
	Player->StateChange(EEgseuState::BossRoomAutoRun);
}

void ACyberSpaceBossMap::SlowMove(float _DeltaTime)
{
	// Collision���� ���°� �ٲ� ����.
	// ���� �� ����.
	if (true == BossDoor_1->IsCurAnimationEnd())
	{
		// ĳ���� �����̰�
		//UAnimationInfo* PlayerAni = Player->PlayerRender->GetCurAnimation();
		//std::string AniName = PlayerAni->Name;
		//Player->PlayerRender->ChangeAnimation(AniName);

		RunVector = FVector::Right * RunSpeed * _DeltaTime;
		Player->AddActorLocation(RunVector);

		if (IsFCollision == false)
		{
			// ���� ����ϸ�, ���� ���·� �̵�.
			StateChange(ECyberBossMapState::CheckPointRoom);
			return;
		}
	}
		

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
	Player->StateChange(EEgseuState::Idle);
	// üũ����Ʈ ����.
}

void ACyberSpaceBossMap::CheckPointRoom(float _DeltaTime)
{
	// ���� �� �� ����.

	// Test
	//if (true == UEngineInput::IsDown('Y'))
	//{
	//	StateChange(ECyberBossMapState::SlowMove);
	//	return;
	//}
}

// �� �� �浹.
void ACyberSpaceBossMap::BossSlowMoveStart()
{
	// Collision���� ���°� �ٲ� ����.
	BossDoor_2->ChangeAnimation("Open");
	RunVector = FVector::Zero;
	Player->StateChange(EEgseuState::BossRoomAutoRun);
	IsBoss = true; // ������ ����ϴ� MoveCameraVector �������� ����.
	CameraRunPos = FVector::Zero;
}
void ACyberSpaceBossMap::BossSlowMove(float _DeltaTime)
{
	// ī�޶� ������
	BossStartCameraVector(_DeltaTime);

	// ĳ���Ͱ� ���l�� �����̸鼭 ��.
	if (true == BossDoor_2->IsCurAnimationEnd())
	{
		RunVector = FVector::Right * RunSpeed * _DeltaTime;
		Player->AddActorLocation(RunVector);

		// ���� ����ϸ�,
		if (IsBCollision == false)
		{
			FVector CameraPos = GetWorld()->GetCameraPos();
			//���� ���·� �̵�.
			StateChange(ECyberBossMapState::BossRoom);
			return;
		}
	}
}

void ACyberSpaceBossMap::BossRoomStart()
{
	BossDoor_2->ChangeAnimation("Close"); // �� ���� �ִϸ��̼�.
	Player->StateChange(EEgseuState::Idle); // �÷��̾� ���¸� Idle��.
	IsBoss = true; // ������ ����ϴ� MoveCameraVector �������� ����.
}

// ������ Start
void ACyberSpaceBossMap::BossRoom(float _DeltaTime)
{
	// ī�޶� ����.
	BossCameraVector();


	int a = 0;
}


// Collicion
void ACyberSpaceBossMap::CollisionCheck(float _DeltaTime)
{
	std::vector<UCollision*> PlayerResult;
	if (true == BossDoor_Coll_1->CollisionCheck(ECollisionOrder::Player, PlayerResult))
	{
		IsFCollision = true;
		if (CollisionCount_1 == 0)
		{
			CollisionCount_1 = 1;
			Player = dynamic_cast<AEgseu*>(PlayerResult[0]->GetOwner());
			StateChange(ECyberBossMapState::SlowMove);
			return;
		}
	}
	else
	{
		IsFCollision = false;
	}

	if (true == BossDoor_Coll_2->CollisionCheck(ECollisionOrder::Player, PlayerResult))
	{
		IsBCollision = true;
		if (CollisionCount_2 == 0)
		{
			CollisionCount_2 = 1;
			StateChange(ECyberBossMapState::BossSlowMove);
			return;
		}
	}
	else
	{
		IsBCollision = false;
	}
}

void ACyberSpaceBossMap::MoveCameraVector(bool _IsBoss)
{
	if (_IsBoss == true)
	{
		return;
	}
	if (Player == nullptr)
	{
		return;
	}
	FVector CameraPos = GetWorld()->GetCameraPos();
	FVector PlayerPos = Player->GetActorLocation();
	FVector ImageScale = this->GetImageScale();
	FVector WindowScale = GEngine->MainWindow.GetWindowScale();

	CameraPos.X = PlayerPos.X - WindowScale.hX();
	CameraPos.Y = PlayerPos.Y - 564.0f;

	if (0.0f >= CameraPos.X)
	{
		CameraPos.X = 0.0f;
	}
	if (CameraPos.X >= ImageScale.X - WindowScale.X)
	{
		CameraPos.X = ImageScale.X - WindowScale.X;
	}

	if (0.0f >= CameraPos.Y)
	{
		CameraPos.Y = 0.0f;
	}

	GetWorld()->SetCameraPos(CameraPos);
}

void ACyberSpaceBossMap::BossStartCameraVector(float _DeltaTime)
{
	FVector CameraPos = GetWorld()->GetCameraPos();

	CameraRunPos = FVector::Right * CameraSpeed * _DeltaTime;
	if (CameraPos.iX() <= 1838)
	{
		GetWorld()->AddCameraPos(CameraRunPos);
	}
	else
	{
		GetWorld()->AddCameraPos(FVector::Zero);
	}
}

void ACyberSpaceBossMap::BossCameraVector()
{
	// Boss Room X : 1838 ~ 2042
	FVector CameraPos = GetWorld()->GetCameraPos();
	FVector PlayerPos = Player->GetActorLocation();
	
	FVector WindowScale = GEngine->MainWindow.GetWindowScale();
	FVector ImageScale = this->GetImageScale();

	CameraPos.X = PlayerPos.X - WindowScale.hX();
	CameraPos.Y = PlayerPos.Y - 562;

	if (1838.0f >= CameraPos.X)
	{
		CameraPos.X = 1838.0f;
	}
	if (2042.0f <= CameraPos.X)
	{
		CameraPos.X = 2042.0f;
	}

	if (0.0f >= CameraPos.Y)
	{
		CameraPos.Y = 0.0f;
	}

	GetWorld()->SetCameraPos(CameraPos);
}

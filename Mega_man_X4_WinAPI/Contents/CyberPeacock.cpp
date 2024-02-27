#include "CyberPeacock.h"
#include <EngineCore/EngineCore.h> // Helper
//#include "Player.h"
#include "Egseu.h"

ACyberPeacock::ACyberPeacock()
{
}

ACyberPeacock::~ACyberPeacock()
{
}


void ACyberPeacock::BeginPlay()
{
	AActor::BeginPlay();

	PeacockRenderer = CreateImageRenderer(static_cast<int>(ERenderOrder::Boss));
	PeacockRenderer->SetImage("Fight_Ready_Right.png");
	UWindowImage* Image = PeacockRenderer->GetImage();
	FVector ImageScale = Image->GetScale();
	PeacockRenderer->SetTransform({ {0, 0}, ImageScale * 2 });

	// Collision
	PeacockCollision = CreateCollision(ECollisionOrder::Boss);
	PeacockCollision->SetScale(ImageScale);
	PeacockCollision->SetColType(ECollisionType::Rect);


	// Animation
	// Intro
	PeacockRenderer->CreateAnimation("Peacock_Intro", "Peacock_Intro.png", 0, 30, 0.05f, false);

	PeacockRenderer->CreateAnimation("Fight_Ready_Left_one", "Fight_Ready_Left.png", 0, 5, 0.05f, false); // �̹��� �̽�
	PeacockRenderer->CreateAnimation("Fight_Ready_Left", "Fight_Ready_Left.png", 6, 6, 1.5f, false);
	PeacockRenderer->CreateAnimation("Fight_Ready_Right", "Fight_Ready_Right.png", 0, 0, 0.5f, false);

	PeacockRenderer->CreateAnimation("Disappear_Appear_Right", "Disappear_Appear_Right.png", 0, 2, 0.05f, true); // �̹��� �̽�
	PeacockRenderer->CreateAnimation("Disappear_Appear_Left", "Disappear_Appear_Left.png", 0, 2, 0.05f, true);

	PeacockRenderer->ChangeAnimation("Peacock_Intro");

	StateChange(ECyberPeacockState::Intro);
}

void ACyberPeacock::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	CollisionCheck();

	StateUpdate(_DeltaTime);
}

void ACyberPeacock::DirCheck()
{
	
}

void ACyberPeacock::StateChange(ECyberPeacockState _State)
{
	if (State != _State)
	{
		switch (_State)
		{
		case ECyberPeacockState::Intro:
			IntroStart();
			break;
		case ECyberPeacockState::IntroEnd:
			IntroEndStart();
			break;
		case ECyberPeacockState::Disappear:
			DisappearStart();
			break;
		case ECyberPeacockState::Appear:
			AppearStart();
			break;
		case ECyberPeacockState::FeatherAttack:
			FeatherAttackStart();
			break;
		case ECyberPeacockState::RisingSlash:
			RisingSlashStart();
			break;
		case ECyberPeacockState::TrackingShot:
			TrackingShotStart();
			break;
		case ECyberPeacockState::Death:
			DeathStart();
			break;
		default :
			break;
		}
	}

	State = _State;
}

void ACyberPeacock::StateUpdate(float _DeltaTime)
{
	switch (State)
	{
	case ECyberPeacockState::Intro :
		Intro(_DeltaTime);
		break;
	case ECyberPeacockState::IntroEnd:
		IntroEnd(_DeltaTime);
		break;
	case ECyberPeacockState::Disappear:
		Disappear(_DeltaTime);
		break;
	case ECyberPeacockState::Appear:
		Appear(_DeltaTime);
		break;
	case ECyberPeacockState::FeatherAttack:
		FeatherAttack(_DeltaTime);
		break;
	case ECyberPeacockState::RisingSlash:
		RisingSlash(_DeltaTime);
		break;
	case ECyberPeacockState::TrackingShot:
		TrackingShot(_DeltaTime);
		break;
	case ECyberPeacockState::Death:
		Death(_DeltaTime);
		break;
	default :
		break;
	}
}

std::string ACyberPeacock::GetAnimationName(std::string _Name)
{
	std::string DirName = "";

	AEgseu* Player = AEgseu::GetMainPlayer();
	EActorDir PlayerDir = Player->GetActorDir();
	switch (PlayerDir)
	{
	case EActorDir::Left:
		DirName = "_Left";
		break;
	case EActorDir::Right:
		DirName = "_Right";
		break;
	default :
		break;
	}

	CurAnimationName = _Name;

	return _Name + DirName;
}

void ACyberPeacock::IntroStart()
{
	
}

void ACyberPeacock::IntroEndStart()
{
	PeacockRenderer->ChangeAnimation("Fight_Ready_Left_one");
}

void ACyberPeacock::DisappearStart()
{
	RandValue = rand() % 3; // 0 ~ 2
	PeacockRenderer->ChangeAnimation(GetAnimationName("Disappear_Appear"));
}

void ACyberPeacock::AppearStart()
{
	PeacockRenderer->ChangeAnimation(GetAnimationName("Disappear_Appear"));
}

void ACyberPeacock::FeatherAttackStart()
{
}

void ACyberPeacock::RisingSlashStart()
{
}

void ACyberPeacock::TrackingShotStart()
{
}

void ACyberPeacock::DeathStart()
{
}




void ACyberPeacock::Intro(float _DeltaTime)
{
	// �ֿ;ƾ� ������.
	BossPatternTime += _DeltaTime;
	
	if (true == PeacockRenderer->IsCurAnimationEnd())
	{
		// �������.

		// ��� ���� �� ��Ī.
		if (5.0f <= BossPatternTime)
		{
			BossPatternTime = 0.0f;
			StateChange(ECyberPeacockState::IntroEnd);
			return;
		}
	}
}

void ACyberPeacock::IntroEnd(float _DeltaTime)
{
	// ��Ī �� ����
	BossPatternTime += _DeltaTime;

	// UI ü�¹� �ö� ����.
	

	// �����.
	if (true == PeacockRenderer->IsCurAnimationEnd())
	{
		BossPatternTime = 0.0f;
		PeacockCollision->ActiveOff();
		StateChange(ECyberPeacockState::Disappear);
		return;
	}
}

void ACyberPeacock::Disappear(float _DeltaTime)
{
	BossPatternTime += _DeltaTime;
	
	if (true == PeacockRenderer->IsCurAnimationEnd())
	{
		// �������.
		PeacockRenderer->ActiveOff();

		// �÷��̾� ��ġ�� ������� ��Ÿ���� ��.
		AEgseu* Player = AEgseu::GetMainPlayer();
		if (nullptr == Player)
		{
			MsgBoxAssert("Player�� �����ϴ�.");
		}
		FVector PlayerPos = Player->GetActorLocation();
		EActorDir PlayerDir = Player->GetActorDir();

		// Start ���� RandValue ���� ����.
		if (RandValue == 0)
		{
			if (PlayerDir == EActorDir::Right)
			{
				this->SetActorLocation({ PlayerPos.iX() - 50, PlayerPos.iY() - 50 });
			}
			else if (PlayerDir == EActorDir::Right)
			{
				this->SetActorLocation({ PlayerPos.iX() + 50, PlayerPos.iY() - 50 });
			}
		}
		else if (RandValue == 1)
		{
			if (PlayerDir == EActorDir::Right)
			{
				this->SetActorLocation({ PlayerPos.iX() - 50, PlayerPos.iY() - 50 });
			}
			else if (PlayerDir == EActorDir::Right)
			{
				this->SetActorLocation({ PlayerPos.iX() + 50, PlayerPos.iY() - 50 });
			}
		}
		else if (RandValue == 2) // hp �� ���� �־�� ��.
		{
			// Player ��ġ�� ���� ��Ÿ���� ��ǥ�� 2���� ����.
			if (PlayerDir == EActorDir::Right)
			{
				this->SetActorLocation({ 550, 320 });
			}
			else if (PlayerDir == EActorDir::Right)
			{
				this->SetActorLocation({ 130, 320 });
			}
		}
	}

	// 2�� ��.
	if (2.0f <= BossPatternTime)
	{
		BossPatternTime = 0.0f;
		StateChange(ECyberPeacockState::Appear);
		return;
	}
}

void ACyberPeacock::Appear(float _DeltaTime)
{
	// ��Ÿ��. ��Ÿ�� ���� �ǰ� ������ ����.
	PeacockRenderer->ActiveOn();
	
	if (true == PeacockRenderer->IsCurAnimationEnd())
	{
		if (RandValue == 0)
		{
			StateChange(ECyberPeacockState::FeatherAttack);
			return;
		}
		else if (RandValue == 1)
		{
			StateChange(ECyberPeacockState::RisingSlash);
			return;
		}
		else if (RandValue == 2)
		{
			StateChange(ECyberPeacockState::TrackingShot);
			return;	
		}
	}
}


void ACyberPeacock::FeatherAttack(float _DeltaTime)
{
	PeacockCollision->ActiveOn();

	if (true == PeacockRenderer->IsCurAnimationEnd())
	{
		int a = 0;
	}
}

void ACyberPeacock::RisingSlash(float _DeltaTime)
{
	PeacockCollision->ActiveOn();

	if (true == PeacockRenderer->IsCurAnimationEnd())
	{
		int a = 0;
	}
}

void ACyberPeacock::TrackingShot(float _DeltaTime)
{
	PeacockCollision->ActiveOn();
	// ���� ��Ʈ �� �̻�, ������� hp�� ���̸� ���� ����.

	if (true == PeacockRenderer->IsCurAnimationEnd())
	{
		int a = 0;
	}
}



void ACyberPeacock::Death(float _DeltaTime)
{
	int a = 0;
	return;
}



void ACyberPeacock::CollisionCheck()
{
	std::vector<UCollision*> Result;
	if (true == PeacockCollision->CollisionCheck(ECollisionOrder::Player, Result))
	{
		
	}
	else if (true == PeacockCollision->CollisionCheck(ECollisionOrder::Weapon, Result))
	{
		HitCount++;

		// buster�� �Ѱ��� �������� �����´�.


		//Hp -= ....;
	}
}

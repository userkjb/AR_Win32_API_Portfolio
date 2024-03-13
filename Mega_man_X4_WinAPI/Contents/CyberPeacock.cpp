#include "CyberPeacock.h"
#include <EngineCore/EngineCore.h> // Helper
#include <EngineBase/EngineRandom.h> // Random
#include "Egseu.h"
#include "Buster.h"

ACyberPeacock::ACyberPeacock()
{
}

ACyberPeacock::~ACyberPeacock()
{
}

ACyberPeacock* ACyberPeacock::MainBoss = nullptr;

ACyberPeacock* ACyberPeacock::GetMainBoss()
{
	return MainBoss;
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

	PeacockRenderer->CreateAnimation("Fight_Ready_Left_one", "Fight_Ready_Left.png", 0, 5, 0.05f, false); // 이미지 이슈
	PeacockRenderer->CreateAnimation("Fight_Ready_Left", "Fight_Ready_Left.png", 6, 6, 1.5f, false);
	PeacockRenderer->CreateAnimation("Fight_Ready_Right", "Fight_Ready_Right.png", 0, 0, 0.5f, false);

	PeacockRenderer->CreateAnimation("Disappear_Appear_Right", "Disappear_Appear_Right.png", 0, 2, 0.05f, true); // 이미지 이슈
	PeacockRenderer->CreateAnimation("Disappear_Appear_Left", "Disappear_Appear_Left.png", 0, 2, 0.05f, true);

	PeacockRenderer->ChangeAnimation("Peacock_Intro");

	StateChange(ECyberPeacockState::None);
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

#pragma region None
#pragma endregion

void ACyberPeacock::IntroStart()
{
	int a = 0;
}

void ACyberPeacock::Intro(float _DeltaTime)
{
	// 주와아앙 나오고.
	BossPatternTime += _DeltaTime;

	if (true == PeacockRenderer->IsCurAnimationEnd())
	{
		// 대사진행.

		// 대사 종료 후 파칭.
		if (5.0f <= BossPatternTime)
		{
			BossPatternTime = 0.0f;
			StateChange(ECyberPeacockState::IntroEnd);
			return;
		}
	}
}

void ACyberPeacock::IntroEndStart()
{
	PeacockRenderer->ChangeAnimation("Fight_Ready_Left_one");
}

void ACyberPeacock::IntroEnd(float _DeltaTime)
{
	// 파칭 한 다음
	BossPatternTime += _DeltaTime;

	// UI 체력바 올라간 다음.


	// 사라짐.
	if (true == PeacockRenderer->IsCurAnimationEnd())
	{
		BossPatternTime = 0.0f;
		PeacockCollision->ActiveOff();
		StateChange(ECyberPeacockState::Disappear);
		return;
	}
}

void ACyberPeacock::DisappearStart()
{
	//RandValue = rand() % 3; // 0 ~ 2
	RandValue = UEngineRandom::MainRandom.RandomInt(0, 2);
	PeacockRenderer->ChangeAnimation(GetAnimationName("Disappear_Appear"));
}

void ACyberPeacock::Disappear(float _DeltaTime)
{
	BossPatternTime += _DeltaTime;

	if (true == PeacockRenderer->IsCurAnimationEnd())
	{
		// 사라졌음.
		PeacockRenderer->ActiveOff();

		// 플레이어 위치를 기반으로 나타나야 함.
		AEgseu* Player = AEgseu::GetMainPlayer();
		if (nullptr == Player)
		{
			MsgBoxAssert("Player가 없습니다.");
		}
		FVector PlayerPos = Player->GetActorLocation();
		EActorDir PlayerDir = Player->GetActorDir();

		// Start 에서 RandValue 값을 구함.
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
		else if (RandValue == 2) // hp 반 조건 넣어야 함.
		{
			// Player 위치에 따라 나타나는 좌표가 2개로 나뉨.
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

	// 2초 후.
	if (2.0f <= BossPatternTime)
	{
		BossPatternTime = 0.0f;
		StateChange(ECyberPeacockState::Appear);
		return;
	}
}

void ACyberPeacock::AppearStart()
{
	PeacockRenderer->ChangeAnimation(GetAnimationName("Disappear_Appear"));
}

void ACyberPeacock::Appear(float _DeltaTime)
{
	// 나타남. 나타날 때는 피격 판정이 없음.
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

void ACyberPeacock::FeatherAttackStart()
{
}

void ACyberPeacock::FeatherAttack(float _DeltaTime)
{
	PeacockCollision->ActiveOn();

	if (true == PeacockRenderer->IsCurAnimationEnd())
	{
		int a = 0;
	}
}

void ACyberPeacock::RisingSlashStart()
{
}

void ACyberPeacock::RisingSlash(float _DeltaTime)
{
	PeacockCollision->ActiveOn();

	if (true == PeacockRenderer->IsCurAnimationEnd())
	{
		int a = 0;
	}
}

void ACyberPeacock::TrackingShotStart()
{
}

void ACyberPeacock::TrackingShot(float _DeltaTime)
{
	PeacockCollision->ActiveOn();
	// 일정 히트 수 이상, 어느정도 hp가 깎이면 패턴 종료.

	if (true == PeacockRenderer->IsCurAnimationEnd())
	{
		int a = 0;
	}
}

void ACyberPeacock::DeathStart()
{
}

void ACyberPeacock::Death(float _DeltaTime)
{
	int a = 0;
	return;
}



void ACyberPeacock::CollisionCheck()
{
	// Buster와 충돌.
	std::vector<UCollision*> Result;
	if (true == PeacockCollision->CollisionCheck(ECollisionOrder::Weapon, Result))
	{
		HitCount++;

		// buster에 설정한 데미지를 가져온다.
		ABuster* x = (ABuster*)Result[0]->GetOwner();
		int DefaultBusterDamage = x->GetDefaultBusterDamage();

		Hp -= DefaultBusterDamage;
	}
}

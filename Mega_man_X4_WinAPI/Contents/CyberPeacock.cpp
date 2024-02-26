#include "CyberPeacock.h"
#include <EngineCore/EngineCore.h> // Helper
#include "Player.h"

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

	PeacockRenderer->CreateAnimation("Fight_Ready_Left_one", "Fight_Ready_Left.png", 0, 6, 1.5f, false);
	PeacockRenderer->CreateAnimation("Fight_Ready_Left", "Fight_Ready_Left.png", 6, 6, 1.5f, false);
	PeacockRenderer->CreateAnimation("Fight_Ready_Right", "Fight_Ready_Right.png", 0, 0, 0.5f, false);

	PeacockRenderer->CreateAnimation("Disappear_Appear_Right", "Disappear_Appear_Right.png", 0, 2, 1.0f, true);
	PeacockRenderer->CreateAnimation("Disappear_Appear_Left", "Disappear_Appear_Left.png", 0, 2, 1.0f, true);

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

	return std::string();
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
	// GetAnimationName()
	PeacockRenderer->ChangeAnimation("Disappear_Appear_Left");
}

void ACyberPeacock::AppearStart()
{
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
	// 주와아앙 나오고.
	if (true == PeacockRenderer->IsCurAnimationEnd())
	{
		// 대사진행.

		// 대사 종료 후 파칭.
		StateChange(ECyberPeacockState::IntroEnd);
		return;
	}
}

void ACyberPeacock::IntroEnd(float _DeltaTime)
{
	// 파칭 한 다음
	

	// UI 체력바 올라간 다음.
	

	// 사라짐.
	PeacockCollision->ActiveOff();
	StateChange(ECyberPeacockState::Disappear);
	return;
}

void ACyberPeacock::Disappear(float _DeltaTime)
{
	// 사라졌음.
	BossPatternTime += _DeltaTime;
	
	if (true == PeacockRenderer->IsCurAnimationEnd())
	{
		PeacockRenderer->ActiveOff();
	}
	
	// 플레이어 위치를 기반으로 나타나야 함.
	APlayer* Player = APlayer::GetMainPlayer();
	if (nullptr == Player)
	{
		MsgBoxAssert("Player가 없습니다.");
	}
	FVector PlayerPos = Player->GetActorLocation();
	EActorDir PlayerDir = Player->GetActorDir();
		
	int RandValue = rand() % 3; // 0 ~ 2
	if (RandValue == 0)
	{
		PatternNumber = 0;
		this->SetActorLocation({ 0, 0 });
	}
	else if (RandValue == 1)
	{
		PatternNumber = 1;
		this->SetActorLocation({ 0, 0 });
	}
	else if (RandValue == 2) // hp 반 조건 넣어야 함.
	{
		PatternNumber = 2;
		// Player 위치에 따라 나타나는 좌표가 2개로 나뉨.
		this->SetActorLocation({ 0, 0 });
	}

	// 2초 후.
	if (2.0f <= BossPatternTime)
	{
		BossPatternTime = 0.0f;
		StateChange(ECyberPeacockState::Appear);
		return;
	}
}

void ACyberPeacock::Appear(float _DeltaTime)
{
	// 나타남. 나타날 때는 피격 판정이 없음.
	PeacockRenderer->ActiveOn();
	
	if (PatternNumber == 0)
	{
		StateChange(ECyberPeacockState::FeatherAttack);
		return;
	}
	else if (PatternNumber == 1)
	{
		StateChange(ECyberPeacockState::RisingSlash);
		return;
	}
	else if (PatternNumber == 2)
	{
		StateChange(ECyberPeacockState::TrackingShot);
		return;	
	}
}


void ACyberPeacock::FeatherAttack(float _DeltaTime)
{
	PeacockCollision->ActiveOn();
}

void ACyberPeacock::RisingSlash(float _DeltaTime)
{
	PeacockCollision->ActiveOn();
}

void ACyberPeacock::TrackingShot(float _DeltaTime)
{
	PeacockCollision->ActiveOn();
	// 일정 히트 수 이상, 어느정도 hp가 깎이면 패턴 종료.
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

		// buster가 넘겨준 데미지를 가져온다.


		//Hp -= ....;
	}
}

#include "CyberPeacock.h"
#include <EngineCore/EngineCore.h> // Helper


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

	PeacockRenderer->CreateAnimation("Fight_Ready_Right", "Fight_Ready_Right.png", 0, 0, 0.5f, false);
	PeacockRenderer->CreateAnimation("Fight_Ready_Left", "Fight_Ready_Left.png", 0, 6, 0.5f, false);

	PeacockRenderer->CreateAnimation("Disappear_Appear_Right", "Disappear_Appear_Right.png", 0, 2, 0.5f, true);
	PeacockRenderer->CreateAnimation("Disappear_Appear_Left", "Disappear_Appear_Left.png", 0, 2, 0.5f, true);

	PeacockRenderer->ChangeAnimation("Peacock_Intro");

	StateChange(EBossState::Intro);
}

void ACyberPeacock::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	StateUpdate(_DeltaTime);
}

void ACyberPeacock::DirCheck()
{
	
}

void ACyberPeacock::StateChange(EBossState _State)
{
	if (State != _State)
	{
		switch (_State)
		{
		case EBossState::Intro :
			IntroStart();
			break;
		case EBossState::Ready:
			ReadyStart();
			break;
		case EBossState::FeatherAttack:
			FeatherAttackStart();
			break;
		case EBossState::RisingSlash:
			RisingSlashStart();
			break;
		case EBossState::TrackingShot:
			TrackingShotStart();
			break;
		case EBossState::Death:
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
	case EBossState::Intro :
		Intro(_DeltaTime);
		break;
	case EBossState::Ready:
		Ready(_DeltaTime);
		break;
	case EBossState::FeatherAttack:
		FeatherAttack(_DeltaTime);
		break;
	case EBossState::RisingSlash:
		RisingSlash(_DeltaTime);
		break;
	case EBossState::TrackingShot:
		TrackingShot(_DeltaTime);
		break;
	case EBossState::Death:
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

void ACyberPeacock::ReadyStart()
{
	//PeacockRenderer->ChangeAnimation()
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
	if (true == PeacockRenderer->IsCurAnimationEnd())
	{
		StateChange(EBossState::Ready);
		return;
	}
}

void ACyberPeacock::Ready(float _DeltaTime)
{
}

void ACyberPeacock::FeatherAttack(float _DeltaTime)
{
}

void ACyberPeacock::RisingSlash(float _DeltaTime)
{
}

void ACyberPeacock::TrackingShot(float _DeltaTime)
{
}

void ACyberPeacock::Death(float _DeltaTime)
{
}

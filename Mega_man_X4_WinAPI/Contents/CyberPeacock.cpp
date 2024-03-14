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
	MainBoss = this;

	PeacockRenderer = CreateImageRenderer(static_cast<int>(ERenderOrder::Boss));
	PeacockRenderer->SetImage("Fight_Ready_Right.png");
	PeacockRenderer->AutoImageScale(2.5f);
	//UWindowImage* Image = PeacockRenderer->GetImage();
	//FVector ImageScale = Image->GetScale();
	//PeacockRenderer->SetTransform({ {0, 0}, ImageScale * 2 });
	TrackingShotScope = CreateImageRenderer(static_cast<int>(ERenderOrder::BossObject));
	TrackingShotScope->SetImage("Scope.png");
	TrackingShotScope->AutoImageScale(2.0f);

	// Collision
	PeacockCollision = CreateCollision(ECollisionOrder::Boss);
	PeacockCollision->SetColType(ECollisionType::Rect);
	FVector CollisionScale = PeacockRenderer->GetImage()->GetScale();
	CollisionScale.X = CollisionScale.X / 7.0f;
	PeacockCollision->SetScale(CollisionScale);
	FVector CollisionPos = PeacockRenderer->GetPosition();
	PeacockCollision->SetPosition({ CollisionPos.X, CollisionPos.Y - 100.0f });


	// Animation
	// Intro
	PeacockRenderer->CreateAnimation("Peacock_Intro", "Peacock_Intro.png", 0, 30, 0.05f, false);
	// ��Ī
	PeacockRenderer->CreateAnimation("Fight_Ready_One", "Fight_Ready_Left.png", 0, 6, 0.05f, false);
	// �����. & ��Ÿ��.
	PeacockRenderer->CreateAnimation("Disappear_Appear_Right", "Disappear_Appear_Right.png", 0, 3, 0.05f, true);
	PeacockRenderer->CreateAnimation("Disappear_Appear_Left", "Disappear_Appear_Left.png", 0, 3, 0.05f, true);
	// FeatherAttack (����)
	PeacockRenderer->CreateAnimation("FeatherAttack_Right", "FeatherAttack_Right.png", 0, 14, 0.08f, false);
	PeacockRenderer->CreateAnimation("FeatherAttack_Left", "FeatherAttack_Left.png", 0, 14, 0.08f, false);
	// RisingSlash (����)
	PeacockRenderer->CreateAnimation("RisingSlash_Right", "RisingSlash_Right.png", 0, 4, 0.05f, false);
	PeacockRenderer->CreateAnimation("RisingSlash_Left", "RisingSlash_Left.png", 0, 4, 0.05f, false);
	PeacockRenderer->CreateAnimation("RisingSlash_Loop_Right", "RisingSlash_Right.png", 5, 7, 0.05f, true);
	PeacockRenderer->CreateAnimation("RisingSlash_Loop_Left", "RisingSlash_Left.png", 5, 7, 0.05f, true);
	// TrackingShot (�̻���)
	PeacockRenderer->CreateAnimation("TrackingShot_Right", "TrackingShot_Right.png", 0, 16, 0.05f, false);
	PeacockRenderer->CreateAnimation("TrackingShot_Left", "TrackingShot_Left.png", 0, 16, 0.05f, false);
	TrackingShotScope->CreateAnimation("Scope", "Scope.png", 9, 9, 1.0f, false);


	PeacockRenderer->ChangeAnimation("Peacock_Intro");
	TrackingShotScope->ChangeAnimation("Scope");
	
	TrackingShotScope->SetActive(false);

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
		case ECyberPeacockState::None:
			NoneStart();
			break;
		case ECyberPeacockState::Wait:
			WaitStart();
			break;
		case ECyberPeacockState::Intro:
			IntroStart();
			break;
		case ECyberPeacockState::BattleReady:
			BattleReadyStart();
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
		case ECyberPeacockState::RisingSlash_Loop:
			RisingSlash_LoopStart();
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
	case ECyberPeacockState::None:
		None(_DeltaTime);
		break;
	case ECyberPeacockState::Wait:
		Wait(_DeltaTime);
		break;
	case ECyberPeacockState::Intro :
		Intro(_DeltaTime);
		break;
	case ECyberPeacockState::BattleReady:
		BattleReady(_DeltaTime);
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
	case ECyberPeacockState::RisingSlash_Loop:
		RisingSlash_Loop(_DeltaTime);
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

	// �Լ� ��ɰ� ���� ����.
	// ������ ���� �ִϸ��̼��� �� �� �ֵ��� �ϴ� ������ �����ϴ� ����
	// �ش� �Լ��� �����ؼ� �� ���� ������ ������.
	CurAnimationName = _Name;

	return _Name + DirName;
}

std::string ACyberPeacock::GetPlayerOppositeAnimationName(std::string _Name)
{
	std::string DirName = "";

	EActorDir PlayerDir = Player->GetActorDir();
	
	switch (PlayerDir)
	{
	case EActorDir::Left:
		DirName = "_Right";
		break;
	case EActorDir::Right:
		DirName = "_Left";
		break;
	default:
		break;
	}

	CurAnimationName = _Name;

	return _Name + DirName;
}

#pragma region None
void ACyberPeacock::NoneStart()
{
}

void ACyberPeacock::None(float _DeltaTime)
{
}
#pragma endregion

#pragma region Wait
void ACyberPeacock::WaitStart()
{
}

void ACyberPeacock::Wait(float _DeltaTime)
{
}
#pragma endregion

#pragma region Intro
void ACyberPeacock::IntroStart()
{
	PeacockRenderer->ChangeAnimation("Peacock_Intro"); // �ֿ;ƾ�
}

void ACyberPeacock::Intro(float _DeltaTime)
{
	if (Player == nullptr)
	{
		Player = AEgseu::GetMainPlayer();
	}

	if (true == PeacockRenderer->IsCurAnimationEnd())
	{
		if (false == b_Intro)
		{
			b_Intro = true;
		}
	}
}
#pragma endregion

#pragma region BattleReady
void ACyberPeacock::BattleReadyStart()
{
	PeacockRenderer->ChangeAnimation("Fight_Ready_One"); // ��Ī~
}

void ACyberPeacock::BattleReady(float _DeltaTime)
{
	if (true == PeacockRenderer->IsCurAnimationEnd())
	{
		if (false == b_BattleReady)
		{
			b_BattleReady = true;
		}
	}
}
#pragma endregion

#pragma region Disappear
void ACyberPeacock::DisappearStart()
{
	//RandValue = rand() % 3; // 0 ~ 2
	//RandValue = UEngineRandom::MainRandom.RandomInt(0, 2); // ���� ����.
	RandValue = 0;
	PeacockCollision->ActiveOff(); // �ݸ��� ����.
	//PeacockRenderer->ChangeAnimation("Disappear_Appear_Left");
	PeacockRenderer->ChangeAnimation(GetPlayerOppositeAnimationName("Disappear_Appear"));
}

void ACyberPeacock::Disappear(float _DeltaTime)
{
	BossPatternTime += _DeltaTime;

	if (true == PeacockRenderer->IsCurAnimationEnd())
	{
		// �������.
		if (true == PeacockRenderer->IsActive()) // ���� ��� �ִٸ� ����.
		{
			PeacockRenderer->ActiveOff();
		}

		// �÷��̾� ��ġ�� ������� ��Ÿ���� ��.
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
				this->SetActorLocation({ PlayerPos.iX() - 50, PlayerPos.iY() });
			}
			else if (PlayerDir == EActorDir::Left)
			{
				this->SetActorLocation({ PlayerPos.iX() + 50, PlayerPos.iY() });
			}
		}
		else if (RandValue == 1)
		{
			if (PlayerDir == EActorDir::Right)
			{
				int x = PlayerPos.iX() - 50;
				if (x < 1838)
				{
					x = 1838;
				}
				this->SetActorLocation({ x, PlayerPos.iY() - 50 });
			}
			else if (PlayerDir == EActorDir::Left)
			{
				this->SetActorLocation({ PlayerPos.iX() + 50, PlayerPos.iY() - 50 });
			}
		}
		else if (RandValue == 2) // hp �� ���� �־�� ��. // �̻���
		{
			// Player ��ġ�� ���� ��Ÿ���� ��ǥ�� 2���� ����.
			if (PlayerDir == EActorDir::Right)
			{
				this->SetActorLocation({ 2050, 320 });
			}
			else if (PlayerDir == EActorDir::Left)
			{
				this->SetActorLocation({ 2550, 320 });
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
#pragma endregion

#pragma region Appear
void ACyberPeacock::AppearStart()
{
	PeacockRenderer->SetActive(true);
	PeacockRenderer->ChangeAnimation(GetPlayerOppositeAnimationName("Disappear_Appear"));
}

void ACyberPeacock::Appear(float _DeltaTime)
{
	// ��Ÿ��.
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
#pragma endregion

#pragma region FeatherAttack (����!!)
void ACyberPeacock::FeatherAttackStart()
{
	if (FeatherAttackCollision == nullptr)
	{
		FeatherAttackCollision = CreateCollision(ECollisionOrder::BossSkill);
		FeatherAttackCollision->SetColType(ECollisionType::Rect);
		FeatherAttackCollision->SetPosition(GetActorLocation());
		FeatherAttackCollision->SetScale({ 0.0f, 0.0f });
	}

	PeacockCollision->ActiveOn();
	PeacockRenderer->ChangeAnimation(GetPlayerOppositeAnimationName("FeatherAttack"));
}

void ACyberPeacock::FeatherAttack(float _DeltaTime)
{
	int Frame = PeacockRenderer->GetCurAnimationImageFrame();
	if (Frame == 7)
	{
		FeatherAttackCollision->SetScale({ 150, 87 });
	}
	else if (Frame == 8 || Frame == 9 || Frame == 10 || Frame == 11 || Frame == 12 || Frame == 13)
	{
		FeatherAttackCollision->SetScale({ 222, 125 });
	}
	else
	{
		FeatherAttackCollision->SetScale({ 0, 0 });
	}
	

	if (true == PeacockRenderer->IsCurAnimationEnd())
	{
		StateChange(ECyberPeacockState::Disappear);
		return;
	}
}
#pragma endregion

#pragma region RisingSlash (����!!)
void ACyberPeacock::RisingSlashStart()
{
	PeacockCollision->ActiveOn();
	PeacockRenderer->ChangeAnimation(GetPlayerOppositeAnimationName("RisingSlash"));
}

void ACyberPeacock::RisingSlash(float _DeltaTime)
{
	int Frame = PeacockRenderer->GetCurAnimationImageFrame();
	
	if (Frame == 4)
	{
		StateChange(ECyberPeacockState::RisingSlash_Loop);
		return;
	}
}
#pragma endregion

#pragma region RisingSlash_Loop
void ACyberPeacock::RisingSlash_LoopStart()
{
	if (RisingSlashTargetPos != 0.0f)
	{
		RisingSlashTargetPos = 0.0f;
	}
	PeacockRenderer->ChangeAnimation(GetPlayerOppositeAnimationName("RisingSlash_Loop"));
	FVector Pos = GetActorLocation();
	RisingSlashTargetPos = Pos.Y - 100.0f;
	RisingSlashVector = FVector::Zero;
}
void ACyberPeacock::RisingSlash_Loop(float _DeltaTime)
{
	RisingSlashVector = FVector::Up * RisingSlashSpeed * _DeltaTime;
	AddActorLocation(RisingSlashVector);
	
	FVector BossPos = GetActorLocation();
	if (BossPos.Y <= RisingSlashTargetPos)
	{
		StateChange(ECyberPeacockState::Disappear);
		return;
	}
}
#pragma endregion

#pragma region TrackingShot (�̻���!!)
void ACyberPeacock::TrackingShotStart()
{
	TrackingShotScope->ActiveOn();
	PeacockRenderer->ActiveOn();
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
#pragma endregion

#pragma region Death
void ACyberPeacock::DeathStart()
{
}

void ACyberPeacock::Death(float _DeltaTime)
{
	int a = 0;
	return;
}
#pragma endregion


void ACyberPeacock::CollisionCheck()
{
	// Buster�� �浹.
	std::vector<UCollision*> Result;
	if (true == PeacockCollision->CollisionCheck(ECollisionOrder::Weapon, Result))
	{
		HitCount++;

		// buster�� ������ �������� �����´�.
		ABuster* x = (ABuster*)Result[0]->GetOwner();
		int DefaultBusterDamage = x->GetDefaultBusterDamage();

		Hp -= DefaultBusterDamage;
	}
}

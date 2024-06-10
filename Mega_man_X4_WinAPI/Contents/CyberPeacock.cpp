#include "CyberPeacock.h"
#include <EngineCore/EngineCore.h> // Helper
#include <EngineBase/EngineRandom.h> // Random
#include "Egseu.h"
#include "Buster.h"
#include "FeatherMissile.h"

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

	MissileCount.reserve(8);
	MissileCount.push_back({ 70, 0 });
	MissileCount.push_back({ 71, -42 });
	MissileCount.push_back({ 52, -71 });
	MissileCount.push_back({ 22, -86 });
	MissileCount.push_back({ -32, -86 });
	MissileCount.push_back({ -69, -75 });
	MissileCount.push_back({ -89, -40 });
	MissileCount.push_back({ -94, 2 });
		
	Renderer();
}

void ACyberPeacock::Renderer()
{
	PeacockRenderer = CreateImageRenderer(static_cast<int>(ERenderOrder::Boss));
	PeacockRenderer->SetImage("Fight_Ready_Right.png");
	PeacockRenderer->AutoImageScale(2.5f);
	//UWindowImage* Image = PeacockRenderer->GetImage();
	//FVector ImageScale = Image->GetScale();
	//PeacockRenderer->SetTransform({ {0, 0}, ImageScale * 2 });
	TrackingShotScope = CreateImageRenderer(static_cast<int>(ERenderOrder::BossObject));
	TrackingShotScope->SetImage("Scope.png");
	TrackingShotScope->AutoImageScale(2.5f);

	// Collision
	PeacockCollision = CreateCollision(ECollisionOrder::Boss);
	PeacockCollision->SetColType(ECollisionType::Rect);
	FVector CollisionScale = PeacockRenderer->GetImage()->GetScale();
	CollisionScale.X = CollisionScale.X / 7.0f;
	PeacockCollision->SetScale(CollisionScale);
	FVector CollisionPos = PeacockRenderer->GetPosition();
	PeacockCollision->SetPosition({ 0.0f, -100.0f });

	// test
	TestImage = CreateImageRenderer(static_cast<int>(ERenderOrder::BossObject));
	TestImage->SetImage("Scope.png");
	TestImage->AutoImageScale();
	///---

	// Animation
	// Intro
	PeacockRenderer->CreateAnimation("Peacock_Intro", "Peacock_Intro.png", 0, 30, 0.025f, false);
	// ��Ī
	PeacockRenderer->CreateAnimation("Fight_Ready_One", "Fight_Ready_Left.png", 0, 6, 0.05f, false);
	// �����. & ��Ÿ��.
	PeacockRenderer->CreateAnimation("Disappear_Appear_Right", "Disappear_Appear_Right.png", 0, 3, 0.05f, true);
	PeacockRenderer->CreateAnimation("Disappear_Appear_Left", "Disappear_Appear_Left.png", 0, 3, 0.05f, true);
	// FeatherAttack (����)
	PeacockRenderer->CreateAnimation("FeatherAttack_Right", "FeatherAttack_Right.png", 0, 14, 0.08f, false);
	PeacockRenderer->CreateAnimation("FeatherAttack_Left", "FeatherAttack_Left.png", 0, 14, 0.08f, false);
	// RisingSlash (����)
	float RisingSlashSpeed = 0.05f;
	PeacockRenderer->CreateAnimation("RisingSlash_Right", "RisingSlash_Right.png", 0, 4, RisingSlashSpeed, false);
	PeacockRenderer->CreateAnimation("RisingSlash_Left", "RisingSlash_Left.png", 0, 4, RisingSlashSpeed, false);
	PeacockRenderer->CreateAnimation("RisingSlash_Loop_Right", "RisingSlash_Right.png", 5, 7, RisingSlashSpeed, true);
	PeacockRenderer->CreateAnimation("RisingSlash_Loop_Left", "RisingSlash_Left.png", 5, 7, RisingSlashSpeed, true);
	// TrackingShot (�̻���)
	PeacockRenderer->CreateAnimation("TrackingShot_Right", "TrackingShot_Right.png", 0, 14, 0.05f, false);
	PeacockRenderer->CreateAnimation("TrackingShot_Left", "TrackingShot_Left.png", 0, 14, 0.05f, false);
	PeacockRenderer->CreateAnimation("TrackingShot_Loop_Right", "TrackingShot_Right.png", 15, 16, 0.5f, true);
	PeacockRenderer->CreateAnimation("TrackingShot_Loop_Left", "TrackingShot_Left.png", 15, 16, 0.5f, true);
	// ����
	PeacockRenderer->CreateAnimation("Death", "Death_Image.png", 0, 0, 0.5f, true);
	// ������
	TrackingShotScope->CreateAnimation("Scope", "Scope.png", 0, 0, 0.5f, true);


	PeacockRenderer->ChangeAnimation("Peacock_Intro");
	TrackingShotScope->ChangeAnimation("Scope");

	//TrackingShotScope->SetActive(false);

	StateChange(ECyberPeacockState::None);
}

void ACyberPeacock::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	CollisionCheck();

	StateUpdate(_DeltaTime);

	if (true == UEngineInput::IsDown('H'))
	{
		Hp = 1;
	}

	// test
	//std::string PeacockHp = "Peacock Hp : " + std::to_string(Hp);
	//UEngineDebug::OutPutDebugText(PeacockHp.data());
	//if (true == UEngineInput::IsDown('K'))
	//{
	//	CreateMissile(0);
	//}
	//if (true == UEngineInput::IsDown('M'))
	//{
	//	if (false == b_test)
	//	{
	//		b_test = true;
	//	}
	//}
	//TestFunction(b_test);
	//if (true == UEngineInput::IsDown('N'))
	//{
	//	if (false == TestImage->IsActive())
	//	{
	//		TestImage->SetPosition(BossImageCenterPos);
	//		TestImage->SetActive(true);
	//	}
	//}
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
		case ECyberPeacockState::TrackingShot_Loop:
			TrackingShot_LoopStart();
			break;
		case ECyberPeacockState::Death:
			DeathStart();
			break;
		case ECyberPeacockState::Explosion:
			ExplosionStart();
			break;
		case ECyberPeacockState::End:
			EndStart();
			break;
		case ECyberPeacockState::Destroy:
			DestroyStart();
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
	case ECyberPeacockState::TrackingShot_Loop:
		TrackingShot_Loop(_DeltaTime);
		break;
	case ECyberPeacockState::Death:
		Death(_DeltaTime);
		break;
	case ECyberPeacockState::Explosion:
		Explosion(_DeltaTime);
		break;
	case ECyberPeacockState::End:
		End(_DeltaTime);
		break;
	case ECyberPeacockState::Destroy:
		Destroy(_DeltaTime);
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
	if (true == TrackingShotScope->IsActive())
	{
		TrackingShotScope->SetActive(false);
	}
	// test
	if (true == TestImage->IsActive())
	{
		TestImage->SetActive(false);
	}

	PeacockRenderer->ChangeAnimation("Peacock_Intro"); // �ֿ;ƾ�
	UEngineSound::SoundPlay("BossIntro.mp3"); // �ֿ;ƾ�
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
	RandValue = UEngineRandom::MainRandom.RandomInt(0, 2); // ���� ����.
	//RandValue = 1; /////////////////////////////////////////////////////////////////////////////////
	PeacockCollision->ActiveOff(); // �ݸ��� ����.
	//PeacockRenderer->ChangeAnimation("Disappear_Appear_Left");
	PeacockRenderer->ChangeAnimation(GetPlayerOppositeAnimationName("Disappear_Appear"));
	UEngineSound::SoundPlay("Disappear_Sound.mp3");
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
				CyberPeacockDir = EActorDir::Right;
				int x = PlayerPos.iX() - 50;
				if (x < 1917)
				{
					x = 1917;
				}
				//this->SetActorLocation({ x, PlayerPos.iY() });
				this->SetActorLocation({ x, 562 });
			}
			else if (PlayerDir == EActorDir::Left)
			{
				CyberPeacockDir = EActorDir::Left;
				int x = PlayerPos.iX() + 50;
				if (x >= 2681)
				{
					x = 2681;
				}
				//this->SetActorLocation({ x, PlayerPos.iY() });
				this->SetActorLocation({ x, 562 });
			}
		}
		else if (RandValue == 1)
		{
			if (PlayerDir == EActorDir::Right)
			{
				CyberPeacockDir = EActorDir::Right;
				int x = PlayerPos.iX() - 50;
				if (x < 1917)
				{
					x = 1917;
				}
				//this->SetActorLocation({ x, PlayerPos.iY() });
				this->SetActorLocation({ x, 562 });
			}
			else if (PlayerDir == EActorDir::Left)
			{
				CyberPeacockDir = EActorDir::Left;
				int x = PlayerPos.iX() + 50;
				if (x >= 2681)
				{
					x = 2681;
				}
				//this->SetActorLocation({ x, PlayerPos.iY() });
				this->SetActorLocation({ x, 562 });
			}
		}
		else if (RandValue == 2) // hp �� ���� �־�� ��. // �̻���
		{
			// Player ��ġ�� ���� ��Ÿ���� ��ǥ�� 2���� ����.

			int x = PlayerPos.iX();
			if (1917 <= x && x <= 2299) // ����
			{
				TrackingShotDir = EActorDir::Left;
				this->SetActorLocation({ 2050, 320 });
			}
			else if (2299< x && x <= 2681) // ������
			{
				TrackingShotDir = EActorDir::Right;
				this->SetActorLocation({ 2550, 320 });
			}
			else
			{
				MsgBoxAssert("��ġ ����");
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
	if (RandValue == 2)
	{
		if (TrackingShotDir == EActorDir::Left)
		{
			// ��Ÿ���� ��ġ�� ���� �� ��.
			PeacockRenderer->ChangeAnimation("Disappear_Appear_Right");
		}
		else if (TrackingShotDir == EActorDir::Right)
		{
			PeacockRenderer->ChangeAnimation("Disappear_Appear_Left");
		}
	}
	else
	{
		PeacockRenderer->ChangeAnimation(GetPlayerOppositeAnimationName("Disappear_Appear"));
	}
	AppearTime = 0.0f;
	UEngineSound::SoundPlay("Disappear_Sound.mp3");
}

void ACyberPeacock::Appear(float _DeltaTime)
{
	AppearTime += _DeltaTime;
	if (AppearTime <= 0.5f)
	{
		return;
	}
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
		FeatherAttackCollision = CreateCollision(ECollisionOrder::CyberPeacock_Feather);
		FeatherAttackCollision->SetColType(ECollisionType::Rect);
		FeatherAttackCollision->SetPosition({ 0.0f, -50.0f });
		FeatherAttackCollision->SetScale({ 1.0f, 1.0f });
	}

	PeacockCollision->ActiveOn();
	PeacockRenderer->ChangeAnimation(GetPlayerOppositeAnimationName("FeatherAttack"));

	UEngineSound::SoundPlay("X4_CyberPeacock_FeatherAttack.mp3");
}

void ACyberPeacock::FeatherAttack(float _DeltaTime)
{
	int Frame = PeacockRenderer->GetCurAnimationImageFrame();
	if (Frame == 7)
	{
		FVector SkillImageScale = PeacockRenderer->GetImage()->GetScale();
		FeatherAttackCollision->SetScale({ SkillImageScale.X / 2.0f, SkillImageScale.Y / 2.0f });
	}
	else if (Frame == 8 || Frame == 9 || Frame == 10 || Frame == 11 || Frame == 12 || Frame == 13)
	{
		FVector SkillImageScale = PeacockRenderer->GetImage()->GetScale();
		FeatherAttackCollision->SetScale({ SkillImageScale.X / 2.0f, SkillImageScale.Y / 2.0f });
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
	if (RisingSlashCollision == nullptr)
	{
		RisingSlashCollision = CreateCollision(ECollisionOrder::CyberPeacock_RisingSlash);
		RisingSlashCollision->SetColType(ECollisionType::Rect);
		RisingSlashCollision->SetPosition({ 0.0f, -110.0f });
		RisingSlashCollision->SetScale({ 1.0f, 1.0f });
	}
	PeacockCollision->ActiveOn();
	PeacockRenderer->ChangeAnimation(GetPlayerOppositeAnimationName("RisingSlash"));
	UEngineSound::SoundPlay("X4_CyberPeacock_RisingSlash.mp3");
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

#pragma region RisingSlash_Loop (����!!)
void ACyberPeacock::RisingSlash_LoopStart()
{
	if (RisingSlashTargetPos != 0.0f)
	{
		RisingSlashTargetPos = 0.0f;
	}
	PeacockRenderer->ChangeAnimation(GetPlayerOppositeAnimationName("RisingSlash_Loop"));
	FVector Pos = GetActorLocation();
	RisingSlashTargetPos = Pos.Y - 200.0f;
	RisingSlashVector = FVector::Zero;
	RisingSlashCollision->SetScale({ 250.0f, 250.0f });
}
void ACyberPeacock::RisingSlash_Loop(float _DeltaTime)
{
	RisingSlashVector = FVector::Up * RisingSlashSpeed * _DeltaTime;
	AddActorLocation(RisingSlashVector);
	
	FVector BossPos = GetActorLocation();
	if (BossPos.Y <= RisingSlashTargetPos)
	{
		RisingSlashCollision->SetScale({ 0.0f, 0.0f });
		StateChange(ECyberPeacockState::Disappear);
		return;
	}
}
#pragma endregion

#pragma region TrackingShot (�̻���!!)
void ACyberPeacock::TrackingShotStart()
{
	// ���� �ִϸ��̼� ����.
	if (CyberPeacockDir == EActorDir::Right)
	{
		PeacockRenderer->ChangeAnimation("TrackingShot_Left");
	}
	else if (CyberPeacockDir == EActorDir::Left)
	{
		PeacockRenderer->ChangeAnimation("TrackingShot_Right");
	}
	PeacockCollision->ActiveOn(); // ���� Collision ��.
	UEngineSound::SoundPlay("X4_CyberPeacock_TrackingShot.mp3");
}

void ACyberPeacock::TrackingShot(float _DeltaTime)
{
	// ���� ��Ʈ �� �̻�, ������� hp�� ���̸� ���� ����.

	// ���� ���� �ִϸ��̼��� ������,
	if (true == PeacockRenderer->IsCurAnimationEnd())
	{
		StateChange(ECyberPeacockState::TrackingShot_Loop);
		return;
	}
}
#pragma endregion

#pragma region TrackingShot_Loop (�̻���!!)
void ACyberPeacock::TrackingShot_LoopStart()
{	
	// ������ ���� ��ġ ����.
	// ������ �¿츦 Ȯ�� �� ����,
	if (TrackingShotDir == EActorDir::Left)
	{
		// ������ ���� ��ġ�� ����.
		TrackingShotScope->SetPosition({ 80.0f, -145.0f });
	}
	else if (TrackingShotDir == EActorDir::Right)
	{
		TrackingShotScope->SetPosition({ -80.0f, -145.0f });
	}	

	// ���� �ִϸ��̼� ����.
	if (CyberPeacockDir == EActorDir::Right)
	{
		PeacockRenderer->ChangeAnimation("TrackingShot_Loop_Left");
	}
	else if (CyberPeacockDir == EActorDir::Left)
	{
		PeacockRenderer->ChangeAnimation("TrackingShot_Loop_Right");
	}

	MissileCreateCount = 0;

	// ������ �ִϸ��̼� ����.
	TrackingShotScope->ChangeAnimation("Scope");
	TrackingShotScope->SetActive(true);
}

void ACyberPeacock::TrackingShot_Loop(float _DeltaTime)
{
	MissileFireDelay += _DeltaTime;
	FVector PlayerPos = Player->GetActorLocation();
	FVector ScopLocalPos = TrackingShotScope->GetPosition();
	FVector ScopGlobalPos = ScopLocalPos + GetActorLocation();
	ScopGlobalPos.Y += 60.0f;
	

	FVector ScorToPlayerLen = PlayerPos - ScopGlobalPos;
	float Len  = ScorToPlayerLen.Size2D();
	//UEngineDebug::OutPutDebugText(std::to_string(Len));

	FVector PlayerDir = PlayerPos - ScopGlobalPos;
	PlayerDir.Normalize2D();

	FVector Move = PlayerDir * 700.0f * _DeltaTime;
	TrackingShotScope->AddPosition(Move);

	// �̻��� ����.
	if (MissileFireDelay >= 2.0f && MissileCreateCount <= 7 && Len <= 5.0f)
	{
		CreateMissile(MissileCreateCount);
		MissileCreateCount++;
		MissileFireDelay = 0.0f;
	}

	if (MissileCreateCount == 8)
	{
		if (MissileFireDelay < 2.0f)
		{
			return;
		}
		TrackingShotScope->SetActive(false);
		StateChange(ECyberPeacockState::Disappear);
		return;
	}
}
#pragma endregion

#pragma region Death
void ACyberPeacock::DeathStart()
{
	if (true == TrackingShotScope->IsActive())
	{
		TrackingShotScope->SetActive(false);
	}

	PeacockRenderer->ChangeAnimation("Death"); // ��¦ ��¦.(�����°� �ƴ�)
	DeathTime = 0.0f;
}

void ACyberPeacock::Death(float _DeltaTime)
{
	DeathTime += _DeltaTime;
	if (DeathTime >= 3.0f) // 2�� ���� ������ �������� �ؾ� ��.
	{
		b_DeathAni = true;
		//StateChange(ECyberPeacockState::Explosion);
		return;
	}
}
#pragma endregion

#pragma region Explosion
void ACyberPeacock::ExplosionStart()
{
	ExplosionTime = 0.0f;
	// ������ �ִϸ��̼����� Change.
	// ������ ���� ���.
}
void ACyberPeacock::Explosion(float _DeltaTime)
{
	ExplosionTime += _DeltaTime;

	if (ExplosionTime >= 0.25f)
	{
		int RandValue = UEngineRandom::MainRandom.RandomInt(0, 2); // ���� ����.

		if (RandValue == 0)
		{
			UEngineSound::SoundPlay("BossExplosion_1.mp3");
		}
		else if (RandValue == 1)
		{
			UEngineSound::SoundPlay("BossExplosion_2.mp3");
		}
		else if (RandValue == 2)
		{
			UEngineSound::SoundPlay("BossExplosion_3.mp3");
		}

		ExplosionTime = 0.0f;
	}
}
#pragma endregion

#pragma region End
void ACyberPeacock::EndStart()
{
	PeacockRenderer->SetActive(false);
}

void ACyberPeacock::End(float _DeltaTime)
{
	
}
#pragma endregion

#pragma region Destroy
void ACyberPeacock::DestroyStart()
{
}
void ACyberPeacock::Destroy(float _DeltaTime)
{
	//this->Destroy(0.0f);
}
#pragma endregion


void ACyberPeacock::CollisionCheck()
{
	// Buster�� �浹.
	std::vector<UCollision*> Result;
	if (true == PeacockCollision->CollisionCheck(ECollisionOrder::Weapon, Result))
	{
		// Buster�� �����´�.
		ABuster* Buster = (ABuster*)Result[0]->GetOwner();
		if (PrevBuster == Buster)
		{
			return;
		}

		// ���� �浹.
		if (PrevBuster == nullptr)
		{
			PrevBuster = Buster;
			UsingBuster = Buster;
			
			CalCollision();
			//StateChange(ECyberPeacockState::BusterCollision);
			Buster->SetBusterState(EBusterState::BusterCrash);
			return;
		}
		else if (PrevBuster != Buster)
		{
			PrevBuster = Buster;
			UsingBuster = Buster;

			CalCollision();
			//StateChange(ECyberPeacockState::BusterCollision);
			Buster->SetBusterState(EBusterState::BusterCrash);
			return;
		}
	}
}

void ACyberPeacock::CreateMissile(int _Count)
{
	AFeatherMissile* Missile = GetWorld()->SpawnActor<AFeatherMissile>(static_cast<int>(EActorType::BossObject));
	
	Missile->SetMissileState(ECyberPeacockMissileState::Create); // �̻��� ���� ����.

	FVector BossCenter = BossImageCenterPos + GetActorLocation();
	FVector MissileCreatePos = BossCenter + MissileCount.at(_Count);
	Missile->SetActorLocation(MissileCreatePos); // ���� ��ġ ����.
}

void ACyberPeacock::CalCollision()
{
	EBusterState BusterState = UsingBuster->GetBusterState();
	if (BusterState == EBusterState::DefaultCharge)
	{
		int DefaultBusterDamage = UsingBuster->GetDefaultBusterDamage();
		Hp -= DefaultBusterDamage;
	}
	else if (BusterState == EBusterState::MiddleCharge)
	{
		int MiddleChargeDamage = UsingBuster->GetMiddleBusterDamage();
		Hp -= MiddleChargeDamage;
	}
	else if (BusterState == EBusterState::PullCharge)
	{
		int PullChargeDamage = UsingBuster->GetPullBusterDamage();
		Hp -= PullChargeDamage;
	}

	UEngineDebug::OutPutDebugText(std::to_string(Hp));

	if (Hp <= 0)
	{
		StateChange(ECyberPeacockState::Death);
		return;
	}
}




// Debug
void ACyberPeacock::TestFunction(bool _test)
{
	if (_test == false)
	{
		return;
	}

	if (true == UEngineInput::IsDown(VK_RIGHT))
	{
		BossImageCenterPos.X += 1.0f;
		UEngineDebug::OutPutDebugText(std::to_string(BossImageCenterPos.X) + " , " + std::to_string(BossImageCenterPos.Y));
	}
	if (true == UEngineInput::IsDown(VK_LEFT))
	{
		BossImageCenterPos.X -= 1.0f;
		UEngineDebug::OutPutDebugText(std::to_string(BossImageCenterPos.X) + " , " + std::to_string(BossImageCenterPos.Y));
	}
	if (true == UEngineInput::IsDown(VK_UP))
	{
		BossImageCenterPos.Y -= 1.0f;
		UEngineDebug::OutPutDebugText(std::to_string(BossImageCenterPos.X) + " , " + std::to_string(BossImageCenterPos.Y));
	}
	if (true == UEngineInput::IsDown(VK_DOWN))
	{
		BossImageCenterPos.Y += 1.0f;
		UEngineDebug::OutPutDebugText(std::to_string(BossImageCenterPos.X) + " , " + std::to_string(BossImageCenterPos.Y));
	}

	TestImage->SetPosition(BossImageCenterPos);
}

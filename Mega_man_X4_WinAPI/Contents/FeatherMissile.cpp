#include "FeatherMissile.h"
#include <EngineCore/EngineCore.h> // Helper
#include "Egseu.h"
#include "CyberPeacock.h"

AFeatherMissile::AFeatherMissile()
{
}

AFeatherMissile::~AFeatherMissile()
{
}

void AFeatherMissile::BeginPlay()
{
	AActor::BeginPlay();

	MissileRenderer = CreateImageRenderer(static_cast<int>(ERenderOrder::BossObject));
	MissileRenderer->SetImage("missile.png");
	MissileRenderer->AutoImageScale(2.0f);

	MissileCollision = CreateCollision(ECollisionOrder::CyberPeacock_Missile);
	MissileCollision->SetColType(ECollisionType::Rect);
	FVector ImageScale = MissileRenderer->GetImage()->GetScale();
	MissileCollision->SetScale({ ImageScale.X / 16, ImageScale.Y / 2 });

	// L               U               R               D        
	// 0  |  1  2  3 |  4 |  5  6  7 |  8 |  9 10 11 | 12 | 13 14 15
	// 16 | 17 18 19 | 20 | 21 22 23 | 24 | 25 26 27 | 28 | 29 30 31
	
	//MissileRenderer->CreateAnimation("Up", "missile.png", { 4, 20 }, 0.5f, true);
	//MissileRenderer->CreateAnimation("UpToRight", "missile.png", { 5, 21, 6, 22, 7, 23 }, 0.5f, true);
	//MissileRenderer->CreateAnimation("UpToLeft", "missile.png", { 3, 19, 2, 18, 1, 17 }, 0.5f, true);	
	//MissileRenderer->CreateAnimation("Right", "missile.png", { 8, 24 }, 0.5f, true);
	//MissileRenderer->CreateAnimation("RightToUp", "missile.png", { 7, 23, 6, 22, 5, 21 }, 0.5f, true);
	//MissileRenderer->CreateAnimation("RightToDown", "missile.png", { 15, 31, 14, 30, 13, 29 }, 0.5f, true);
	//MissileRenderer->CreateAnimation("Left", "missile.png", {0, 16}, 0.5f, true);
	//MissileRenderer->CreateAnimation("LeftToUp", "missile.png", { 1, 17, 2, 18, 3, 19 }, 0.5f, true);
	//MissileRenderer->CreateAnimation("LeftToDown", "missile.png", { 9, 25, 10, 26, 11, 27 }, 0.5f, true);
	//MissileRenderer->CreateAnimation("Down", "missile.png", { 12, 28 }, 0.5f, true);
	//MissileRenderer->CreateAnimation("DownToRight", "missile.png", { 13, 29, 14, 30, 15, 31 }, 0.5f, true);
	//MissileRenderer->CreateAnimation("DownToLeft", "missile.png", { 11, 27, 10, 26, 9, 25 }, 0.5f, true);

	float AnimationSpeed = 0.05f;
	MissileRenderer->CreateAnimation("Top", "missile.png",					  { 4, 20 },  AnimationSpeed, true);
	MissileRenderer->CreateAnimation("Bottom", "missile.png",				  { 12, 28 }, AnimationSpeed, true);
	MissileRenderer->CreateAnimation("Left", "missile.png",					  { 0, 16 },  AnimationSpeed, true);
	MissileRenderer->CreateAnimation("Right", "missile.png",				  { 8, 24 },  AnimationSpeed, true);
	MissileRenderer->CreateAnimation("Top-Left", "missile.png",				  { 2, 18 },  AnimationSpeed, true);
	MissileRenderer->CreateAnimation("Top-Right", "missile.png",			  { 6, 22 },  AnimationSpeed, true);
	MissileRenderer->CreateAnimation("Bottom-Left", "missile.png",			  { 10, 26 }, AnimationSpeed, true);
	MissileRenderer->CreateAnimation("Bottom-Right", "missile.png",			  { 14, 30 }, AnimationSpeed, true);
	MissileRenderer->CreateAnimation("Top-To-Top-Right", "missile.png",		  { 5, 21 },  AnimationSpeed, true);
	MissileRenderer->CreateAnimation("Top-To-Top-Left", "missile.png",		  { 3, 19 },  AnimationSpeed, true);
	MissileRenderer->CreateAnimation("Left-To-Top-Left", "missile.png",		  { 1, 17 },  AnimationSpeed, true);
	MissileRenderer->CreateAnimation("Left-To-Bottom-Left", "missile.png",    { 9, 25 },  AnimationSpeed, true);	
	MissileRenderer->CreateAnimation("Bottom-To-Bottom-Left", "missile.png",  { 11, 27 }, AnimationSpeed, true);
	MissileRenderer->CreateAnimation("Bottom-To-Bottom-Right", "missile.png", { 13, 29 }, AnimationSpeed, true);
	MissileRenderer->CreateAnimation("Right-To-Top-Right", "missile.png",	  { 7, 23 },  AnimationSpeed, true);
	MissileRenderer->CreateAnimation("Right-To-Bottom-Right", "missile.png",  { 15, 31 }, AnimationSpeed, true);
	MissileRenderer->CreateAnimation("Death", "Missile_Death.png", 0, 2, 0.05f, true);

	MissileRenderer->ChangeAnimation("Left");

	// Player
	if (Player == nullptr)
	{
		Player = AEgseu::GetMainPlayer();
	}
	// Boss
	if (Boss == nullptr)
	{
		Boss = ACyberPeacock::GetMainBoss();
	}
	
	StateChange(ECyberPeacockMissileState::Create);
}

void AFeatherMissile::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	StateUpdate(_DeltaTime);
	CollisionCheck();
}

void AFeatherMissile::StateChange(ECyberPeacockMissileState _State)
{
	if (MissileState != _State)
	{
		switch (_State)
		{
		case ECyberPeacockMissileState::None:
			NoneStart();
			break;
		case ECyberPeacockMissileState::Create:
			CreateStart();			
			break;
		case ECyberPeacockMissileState::Run:
			RunStart();
			break;
		case ECyberPeacockMissileState::Death:
			DeathStart();			
			break;
		default:
			break;
		}
	}

	MissileState = _State;
}

void AFeatherMissile::StateUpdate(float _DeltaTime)
{
	switch (MissileState)
	{
	case ECyberPeacockMissileState::None:
		None(_DeltaTime);
		break;
	case ECyberPeacockMissileState::Create:
		Create(_DeltaTime);
		break;
	case ECyberPeacockMissileState::Run:
		Run(_DeltaTime);
		break;
	case ECyberPeacockMissileState::Death:
		Death(_DeltaTime);
		break;
	default:
		break;
	}
}

#pragma region None
void AFeatherMissile::NoneStart()
{
}

void AFeatherMissile::None(float _DeltaTime)
{
}
#pragma endregion

#pragma region Create
void AFeatherMissile::CreateStart()
{
	int a = 0;
}

void AFeatherMissile::Create(float _DeltaTime)
{
	StateChange(ECyberPeacockMissileState::Run);
	return;
}
#pragma endregion

#pragma region Run
void AFeatherMissile::RunStart()
{
	// 미사일 켜고.
	if (false == MissileRenderer->IsActive())
	{
		MissileRenderer->SetActive(true);
	}
	MissileRenderer->ChangeAnimation("Right");
	MissileVector = FVector::Zero;
	LifeTime = 0.0f;
	MissileStartDelay = 0.0f;
}

void AFeatherMissile::Run(float _DeltaTime)
{
	MissileStartDelay += _DeltaTime;
	LifeTime += _DeltaTime;

	// 0.5초 동안 일정 방향 나가다가,
	if (MissileStartDelay <= 0.2f)
	{
		int Count = Boss->GetMissileCreateCount();
		MissileVector = FVector::Right * Speed * _DeltaTime;
		AddActorLocation(MissileVector);
		return;
	}

	{
		FVector PlayerPos = Player->GetActorLocation();
		PlayerPos.Y -= 50.0f;
		FVector MissilePos = GetActorLocation();

		FVector PlayerDir = PlayerPos - MissilePos;
		PlayerDir.Normalize2D();

		if (PlayerDir.X >= 0.0f) // 양
		{
			if (PlayerDir.Y >= 0.0f) // 양
			{
				// + +
				if (0.0f <= PlayerDir.X && PlayerDir.X < 0.125f)
				{
					MissileRenderer->ChangeAnimation("Bottom");
				}
				else if (0.125f <= PlayerDir.X && PlayerDir.X <= 0.375f)
				{
					MissileRenderer->ChangeAnimation("Bottom-To-Bottom-Right");
				}
				else if (0.375f < PlayerDir.X && PlayerDir.X < 0.625f)
				{
					MissileRenderer->ChangeAnimation("Bottom-Right");
				}
				else if (0.625f <= PlayerDir.X && PlayerDir.X <= 0.875)
				{
					MissileRenderer->ChangeAnimation("Right-To-Bottom-Right");
				}
				else if (0.875 < PlayerDir.X && PlayerDir.X <= 1.0f)
				{
					MissileRenderer->ChangeAnimation("Right");
				}
			}
			else if (PlayerDir.Y < 0.0f) // 음
			{
				// + -
				if (1.0f <= PlayerDir.X && PlayerDir.X < 0.875f)
				{
					MissileRenderer->ChangeAnimation("Right");
				}
				else if (0.875f <= PlayerDir.X && PlayerDir.X <= 0.625f)
				{
					MissileRenderer->ChangeAnimation("Right-To-Top-Right");
				}
				else if (0.625f < PlayerDir.X && PlayerDir.X < 0.375f)
				{
					MissileRenderer->ChangeAnimation("Top-Right");
				}
				else if (0.375f <= PlayerDir.X && PlayerDir.X <= 0.125f)
				{
					MissileRenderer->ChangeAnimation("Top-To-Top-Right");
				}
				else if (0.125f < PlayerDir.X && PlayerDir.X <= 0.0f)
				{
					MissileRenderer->ChangeAnimation("Top");
				}
			}
		}
		else if (PlayerDir.X < 0.0f) // 음
		{
			if (PlayerDir.Y >= 0.0f) // 양
			{
				// - +
				if (-1.0f <= PlayerDir.X && PlayerDir.X < -0.875f)
				{
					MissileRenderer->ChangeAnimation("Left");
				}
				else if (-0.875f <= PlayerDir.X && PlayerDir.X <= -0.625f)
				{
					MissileRenderer->ChangeAnimation("Left-To-Bottom-Left");
				}
				else if (-0.625f < PlayerDir.X && PlayerDir.X < -0.375f)
				{
					MissileRenderer->ChangeAnimation("Bottom-Left");
				}
				else if (-0.375f <= PlayerDir.X && PlayerDir.X <= -0.125f)
				{
					MissileRenderer->ChangeAnimation("Bottom-To-Bottom-Left");
				}
				else if (-0.125f < PlayerDir.X && PlayerDir.X <= 0.0f)
				{
					MissileRenderer->ChangeAnimation("Bottom");
				}
			}
			else if (PlayerDir.Y < 0.0f) // 음
			{
				// - -
				if (-1.0f <= PlayerDir.X && PlayerDir.X < -0.875f)
				{
					MissileRenderer->ChangeAnimation("Left");
				}
				else if (-0.875f <= PlayerDir.X && PlayerDir.X <= -0.625f)
				{
					MissileRenderer->ChangeAnimation("Left-To-Top-Left");
				}
				else if (-0.625f < PlayerDir.X && PlayerDir.X < -0.375f)
				{
					MissileRenderer->ChangeAnimation("Top-Left");
				}
				else if (-0.375f <= PlayerDir.X && PlayerDir.X <= -0.125f)
				{
					MissileRenderer->ChangeAnimation("Top-To-Top-Left");
				}
				else if (0.125f < PlayerDir.X && PlayerDir.X <= 0.0f)
				{
					MissileRenderer->ChangeAnimation("Top");
				}
			}
		}
		MissileVector = PlayerDir * Speed * _DeltaTime;
		AddActorLocation(MissileVector);
	}

	// 3초가 지나면,
	if (LifeTime >= 2.0f)
	{
		LifeTime = 0.0f;
		StateChange(ECyberPeacockMissileState::Death);
		return;
	}
}
#pragma endregion


#pragma region Death
void AFeatherMissile::DeathStart()
{
	MissileRenderer->ChangeAnimation("Death");
	MissileCollision->SetActive(false);
	DeathTime = 0.0f;
}

void AFeatherMissile::Death(float _DeltaTime)
{
	DeathTime += _DeltaTime;
	if (DeathTime >= DeathLifeTime)
	{
		this->Destroy(0.0f);
	}
}
#pragma endregion



#pragma region CollisionCheck
void AFeatherMissile::CollisionCheck()
{
	std::vector<UCollision*> TargetResult;
	if(true == MissileCollision->CollisionCheck(ECollisionOrder::Player, TargetResult))
	{
		AEgseu* Player = dynamic_cast<AEgseu*>(TargetResult[0]->GetOwner());
		Player->SetPlayerHp(Damage);
		Player->SetStateChange(EEgseuState::Hit);
		StateChange(ECyberPeacockMissileState::Death);
		return;
	}
}
#pragma endregion

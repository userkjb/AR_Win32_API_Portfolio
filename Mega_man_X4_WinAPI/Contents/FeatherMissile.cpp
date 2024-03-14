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

	MissileCollision = CreateCollision(ECollisionOrder::BossSkill);
	MissileCollision->SetColType(ECollisionType::Rect);

	// L               U               R               D        
	// 0  |  1  2  3 |  4 |  5  6  7 |  8 |  9 10 11 | 12 | 13 14 15
	// 16 | 17 18 19 | 20 | 21 22 23 | 24 | 25 26 27 | 28 | 29 30 31
	MissileRenderer->CreateAnimation("Up", "missile.png", { 4, 20 }, 0.5f, true);
	MissileRenderer->CreateAnimation("UpToRight", "missile.png", { 5, 21, 6, 22, 7, 23 }, 0.5f, true);
	MissileRenderer->CreateAnimation("UpToLeft", "missile.png", { 3, 19, 2, 18, 1, 17 }, 0.5f, true);
	
	MissileRenderer->CreateAnimation("Right", "missile.png", { 8, 24 }, 0.5f, true);
	MissileRenderer->CreateAnimation("RightToUp", "missile.png", { 7, 23, 6, 22, 5, 21 }, 0.5f, true);
	MissileRenderer->CreateAnimation("RightToDown", "missile.png", { 15, 31, 14, 30, 13, 29 }, 0.5f, true);

	MissileRenderer->CreateAnimation("Left", "missile.png", {0, 16}, 0.5f, true);
	MissileRenderer->CreateAnimation("LeftToUp", "missile.png", { 1, 17, 2, 18, 3, 19 }, 0.5f, true);
	MissileRenderer->CreateAnimation("LeftToDown", "missile.png", { 9, 25, 10, 26, 11, 27 }, 0.5f, true);

	MissileRenderer->CreateAnimation("Down", "missile.png", { 12, 28 }, 0.5f, true);
	MissileRenderer->CreateAnimation("DownToRight", "missile.png", { 13, 29, 14, 30, 15, 31 }, 0.5f, true);
	MissileRenderer->CreateAnimation("DownToLeft", "missile.png", { 11, 27, 10, 26, 9, 25 }, 0.5f, true);


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
		case ECyberPeacockMissileState::Run_Up:
			Run_UpStart();			
			break;
		case ECyberPeacockMissileState::Run_UpToRight:
			Run_UpToRightStart();			
			break;
		case ECyberPeacockMissileState::Run_UpToLeft:
			Run_UpToLeftStart();			
			break;
		case ECyberPeacockMissileState::Run_Right:
			Run_RightStart();			
			break;
		case ECyberPeacockMissileState::Run_RightToUp:
			Run_RightToUpStart();			
			break;
		case ECyberPeacockMissileState::Run_RightToDown:
			Run_RightToDownStart();			
			break;
		case ECyberPeacockMissileState::Run_Left:
			Run_LeftStart();			
			break;
		case ECyberPeacockMissileState::Run_LeftToUp:
			Run_LeftToUpStart();			
			break;
		case ECyberPeacockMissileState::Run_LeftToDown:
			Run_LeftToDownStart();			
			break;
		case ECyberPeacockMissileState::Run_Down:
			Run_DownStart();			
			break;
		case ECyberPeacockMissileState::Run_DownToRight:
			Run_DownToRightStart();			
			break;
		case ECyberPeacockMissileState::Run_DownToLeft:
			Run_DownToLeftStart();			
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
	case ECyberPeacockMissileState::Run_Up:
		Run_Up(_DeltaTime);
		break;
	case ECyberPeacockMissileState::Run_UpToRight:
		Run_UpToRight(_DeltaTime);
		break;
	case ECyberPeacockMissileState::Run_UpToLeft:
		Run_UpToLeft(_DeltaTime);
		break;
	case ECyberPeacockMissileState::Run_Right:
		Run_Right(_DeltaTime);
		break;
	case ECyberPeacockMissileState::Run_RightToUp:
		Run_RightToUp(_DeltaTime);
		break;
	case ECyberPeacockMissileState::Run_RightToDown:
		Run_RightToDown(_DeltaTime);
		break;
	case ECyberPeacockMissileState::Run_Left:
		Run_Left(_DeltaTime);
		break;
	case ECyberPeacockMissileState::Run_LeftToUp:
		Run_LeftToUp(_DeltaTime);
		break;
	case ECyberPeacockMissileState::Run_LeftToDown:
		Run_LeftToDown(_DeltaTime);
		break;
	case ECyberPeacockMissileState::Run_Down:
		Run_Down(_DeltaTime);
		break;
	case ECyberPeacockMissileState::Run_DownToRight:
		Run_DownToRight(_DeltaTime);
		break;
	case ECyberPeacockMissileState::Run_DownToLeft:
		Run_DownToLeft(_DeltaTime);
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
	int a = 0;
}

void AFeatherMissile::None(float _DeltaTime)
{
	int a = 0;
}
#pragma endregion

#pragma region Create
void AFeatherMissile::CreateStart()
{
	int a = 0;
}

void AFeatherMissile::Create(float _DeltaTime)
{
	if (MissileStartDir == EActorDir::Left)
	{
		StateChange(ECyberPeacockMissileState::Run_Left);
		return;
	}
	else
	{
		StateChange(ECyberPeacockMissileState::Run_Right);
		return;
	}
}
#pragma endregion

#pragma region Up
void AFeatherMissile::Run_UpStart()
{
}

void AFeatherMissile::Run_Up(float _DeltaTime)
{
}
#pragma endregion

#pragma region Up To Right
void AFeatherMissile::Run_UpToRightStart()
{
}

void AFeatherMissile::Run_UpToRight(float _DeltaTime)
{
}
#pragma endregion

#pragma region Up To Left
void AFeatherMissile::Run_UpToLeftStart()
{
}

void AFeatherMissile::Run_UpToLeft(float _DeltaTime)
{
}
#pragma endregion

#pragma region Right
void AFeatherMissile::Run_RightStart()
{
	if (false == MissileRenderer->IsActive())
	{
		MissileRenderer->SetActive(true);
	}
	MissileRenderer->ChangeAnimation("Right");
	MissileVector = FVector::Zero;
}

void AFeatherMissile::Run_Right(float _DeltaTime)
{
	MissileStartDelay += _DeltaTime;

	// 0.5초 동안 오른쪽으로만 나가다가,
	if (MissileStartDelay <= 0.2f)
	{
		MissileVector = FVector::Right * Speed * _DeltaTime;
		AddActorLocation(MissileVector);
		return;
	}

	// 미사일이 플레이어로 향할 때의 방향 계산.

	{
		FVector PlayerPos = Player->GetActorLocation();
		PlayerPos.Y -= 50.0f;
		FVector MissilePos = GetActorLocation();

		FVector PlayerDir = PlayerPos - MissilePos;
		PlayerDir.Normalize2D();

		MissileVector = PlayerDir * Speed * _DeltaTime;
		AddActorLocation(MissileVector);
	}
}
#pragma endregion

#pragma region Right To Up
void AFeatherMissile::Run_RightToUpStart()
{
}

void AFeatherMissile::Run_RightToUp(float _DeltaTime)
{
}
#pragma endregion

#pragma region Right To Down
void AFeatherMissile::Run_RightToDownStart()
{
}

void AFeatherMissile::Run_RightToDown(float _DeltaTime)
{
}
#pragma endregion

#pragma region Left
void AFeatherMissile::Run_LeftStart()
{
	int a = 0;
}

void AFeatherMissile::Run_Left(float _DeltaTime)
{
	int a = 0;
}
#pragma endregion

#pragma region str
#pragma endregion
void AFeatherMissile::Run_LeftToUpStart()
{
}

void AFeatherMissile::Run_LeftToUp(float _DeltaTime)
{
}

#pragma region str
#pragma endregion
void AFeatherMissile::Run_LeftToDownStart()
{
}

void AFeatherMissile::Run_LeftToDown(float _DeltaTime)
{
}

#pragma region str
#pragma endregion
void AFeatherMissile::Run_DownStart()
{
}

void AFeatherMissile::Run_Down(float _DeltaTime)
{
}

#pragma region str
#pragma endregion
void AFeatherMissile::Run_DownToRightStart()
{
}

void AFeatherMissile::Run_DownToRight(float _DeltaTime)
{
}

#pragma region str
#pragma endregion
void AFeatherMissile::Run_DownToLeftStart()
{
}

void AFeatherMissile::Run_DownToLeft(float _DeltaTime)
{
}

#pragma region str
#pragma endregion
void AFeatherMissile::DeathStart()
{
}

void AFeatherMissile::Death(float _DeltaTime)
{
}



#pragma region CollisionCheck
#pragma endregion
void AFeatherMissile::CollisionCheck()
{
}

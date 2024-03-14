#include "FeatherMissile.h"
#include <EngineCore/EngineCore.h> // Helper

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



	// Player
	if (Player == nullptr)
	{
		Player = AEgseu::GetMainPlayer();
	}

	StateChange(ECyberPeacockMissileState::None);
}

void AFeatherMissile::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	StateUpdate(_DeltaTime);
}

void AFeatherMissile::StateChange(ECyberPeacockMissileState _State)
{
	if (MillileState != _State)
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

	MillileState = _State;
}

void AFeatherMissile::StateUpdate(float _DeltaTime)
{
	switch (MillileState)
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

void AFeatherMissile::NoneStart()
{
}

void AFeatherMissile::None(float _DeltaTime)
{
}

void AFeatherMissile::CreateStart()
{
}

void AFeatherMissile::Create(float _DeltaTime)
{
}

void AFeatherMissile::Run_UpStart()
{
}

void AFeatherMissile::Run_Up(float _DeltaTime)
{
}

void AFeatherMissile::Run_UpToRightStart()
{
}

void AFeatherMissile::Run_UpToRight(float _DeltaTime)
{
}

void AFeatherMissile::Run_UpToLeftStart()
{
}

void AFeatherMissile::Run_UpToLeft(float _DeltaTime)
{
}

void AFeatherMissile::Run_RightStart()
{
}

void AFeatherMissile::Run_Right(float _DeltaTime)
{
}

void AFeatherMissile::Run_RightToUpStart()
{
}

void AFeatherMissile::Run_RightToUp(float _DeltaTime)
{
}

void AFeatherMissile::Run_RightToDownStart()
{
}

void AFeatherMissile::Run_RightToDown(float _DeltaTime)
{
}

void AFeatherMissile::Run_LeftStart()
{
}

void AFeatherMissile::Run_Left(float _DeltaTime)
{
}

void AFeatherMissile::Run_LeftToUpStart()
{
}

void AFeatherMissile::Run_LeftToUp(float _DeltaTime)
{
}

void AFeatherMissile::Run_LeftToDownStart()
{
}

void AFeatherMissile::Run_LeftToDown(float _DeltaTime)
{
}

void AFeatherMissile::Run_DownStart()
{
}

void AFeatherMissile::Run_Down(float _DeltaTime)
{
}

void AFeatherMissile::Run_DownToRightStart()
{
}

void AFeatherMissile::Run_DownToRight(float _DeltaTime)
{
}

void AFeatherMissile::Run_DownToLeftStart()
{
}

void AFeatherMissile::Run_DownToLeft(float _DeltaTime)
{
}

void AFeatherMissile::DeathStart()
{
}

void AFeatherMissile::Death(float _DeltaTime)
{
}




void AFeatherMissile::CollisionCheck()
{
}

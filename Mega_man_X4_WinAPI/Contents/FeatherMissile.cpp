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
		break;
	default:
		break;
	}
}

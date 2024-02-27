#include "Egseu.h"
#include <EngineCore/EngineCore.h> // Helper
#include "Buster.h"

AEgseu::AEgseu()
{
}

AEgseu::~AEgseu()
{
}

AEgseu* AEgseu::MainPlayer = nullptr;

AEgseu* AEgseu::GetMainPlayer()
{
	return MainPlayer;
}

void AEgseu::BeginPlay()
{
	AActor::BeginPlay();
	MainPlayer = this;

}

void AEgseu::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

}

void AEgseu::DirCheck()
{
}

std::string AEgseu::GetAnimationName(std::string _Name)
{
	return std::string();
}

void AEgseu::StateChange(EEgseuState _State)
{
}

void AEgseu::StateUpdate(float _DeltaTime)
{
}

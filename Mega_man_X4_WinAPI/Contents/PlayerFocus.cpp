#include "PlayerFocus.h"
#include "Egseu.h"

APlayerFocus::APlayerFocus()
{
}

APlayerFocus::~APlayerFocus()
{
}

void APlayerFocus::BeginPlay()
{
	AActor::BeginPlay();

	ForcusComponent();

	StateChange(EFocusState::None);
}

void APlayerFocus::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	StateUpdate(_DeltaTime);
}

void APlayerFocus::ForcusComponent()
{
	// Actor 의 위치.
	FVector ActorPos = GetActorLocation();

	FocusRender = CreateImageRenderer(static_cast<int>(ERenderOrder::MapObject));
	FocusRender->SetImage("Focus.png");
	FocusRender->AutoImageScale(3.0f);

	FocusArrow = CreateImageRenderer(static_cast<int>(ERenderOrder::MapObject));
	FocusArrow->SetImage("Focus.png");
	FocusArrow->AutoImageScale(3.0f);

	//RightUpRender = 
	//LeftUpRender = 
	//LeftDownRender = 
	
	HurryUpRender = CreateImageRenderer(static_cast<int>(ERenderOrder::MapObject));
	HurryUpRender->SetImage("HurryUp.png");
	HurryUpRender->AutoImageScale(3.0f);
	HurryUpRender->SetPosition({ 0, 80 });

	CreateAnimations();
}

void APlayerFocus::CreateAnimations()
{
	FocusRender->CreateAnimation("FocusStart", "Focus.png", 0, 2, 0.1f, false);
	FocusRender->CreateAnimation("FocusLoop", "Focus.png", 3, 4, 0.2f, true);
	FocusRender->CreateAnimation("FocusEnd", "Focus.png", { 2, 1, 0 }, 0.5f, false);
	FocusRender->CreateAnimation("FocusIcon", "Focus.png", 0, 0, 0.5f, false);

	FocusArrow->CreateAnimation("FocusLoop", "Focus.png", 5, 6, 0.3f, true);
	FocusArrow->ActiveOff();

	HurryUpRender->CreateAnimation("FocusLoop", "HurryUp.png", 0, 2, 0.2f, true);
	HurryUpRender->ActiveOff();


	FocusRender->ChangeAnimation("FocusStart");
	FocusArrow->ChangeAnimation("FocusLoop");
	HurryUpRender->ChangeAnimation("FocusLoop");
}

void APlayerFocus::StateChange(EFocusState _State)
{
	if (State != _State)
	{
		switch (_State)
		{
		case EFocusState::None:
			//NoneStart();
			break;
		case EFocusState::CallCreate:
			//CallCreateStart();
			break;
		case EFocusState::Create:
			CreateStart();
			break;
		case EFocusState::CreateLoop:
			CreateLoopStart();
			break;
		case EFocusState::Run:
			RunStart();
			break;
		case EFocusState::Rank:
			break;
		case EFocusState::RunUp:
			RunUpStart();
			break;
		case EFocusState::RunRight:
			RunRightStart();
			break;
		case EFocusState::Death:
			DeathStart();
			break;
		default :
			break;
		}
	}
	State = _State;
}

void APlayerFocus::StateUpdate(float _DeltaTime)
{
	switch (State)
	{
	case EFocusState::None:
		//None(_DeltaTime);
		break;
	case EFocusState::CallCreate:
		CallCreate(_DeltaTime);
		break;
	case EFocusState::Create:
		Create(_DeltaTime);
		break;
	case EFocusState::CreateLoop:
		CreateLoop(_DeltaTime);
		break;
	case EFocusState::CallRun:
		CallRun(_DeltaTime);
		break;
	case EFocusState::Run:
		Run(_DeltaTime);
		break;
	case EFocusState::Rank:
		Rank(_DeltaTime);
		break;
	case EFocusState::RunUp:
		RunUp(_DeltaTime);
		break;
	case EFocusState::RunRight:
		RunRight(_DeltaTime);
		break;
	case EFocusState::Death:
		Death(_DeltaTime);
		break;
	default:
		break;
	}
}

#pragma region CallCreate
void APlayerFocus::CallCreate(float _DeltaTime)
{
	StateChange(EFocusState::Create);
	return;
}
#pragma endregion

#pragma region Create
void APlayerFocus::CreateStart()
{
	GetWorld()->SetAllTimeScale(0.0f);
	GetWorld()->SetTimeScale(EActorType::Map, 1.0f);
	GetWorld()->SetTimeScale(EActorType::MapObject, 1.0f);

	FocusRender->ChangeAnimation("FocusStart");
	if (true == FocusArrow->IsActive())
	{
		FocusArrow->ActiveOff();
	}
	if (true == HurryUpRender->IsActive())
	{
		HurryUpRender->ActiveOff();
	}
}

void APlayerFocus::Create(float _DeltaTime)
{
	if (true == FocusRender->IsCurAnimationEnd())
	{
		StateChange(EFocusState::CreateLoop);
		return;
	}
}
#pragma endregion

#pragma region CreateLoop
void APlayerFocus::CreateLoopStart()
{
	FocusRender->ChangeAnimation("FocusLoop");
	if (false == FocusArrow->IsActive())
	{
		FocusArrow->ActiveOn();
	}
	if (false == HurryUpRender->IsActive())
	{
		HurryUpRender->ActiveOn();
	}
}

void APlayerFocus::CreateLoop(float _DeltaTime)
{
	//CreateDelay += _DeltaTime;
	////UEngineDebug::OutPutDebugText(std::to_string(CreateDelay));
	//if (CreateDelay <= 1.5f)
	//{
	//	return;
	//}
	//
	//StateChange(EFocusState::Run);
	return;
}

#pragma endregion

#pragma region Run
void APlayerFocus::CallRun(float _DeltaTime)
{
	StateChange(EFocusState::Run);
	return;
}

void APlayerFocus::RunStart()
{
	CreateDelay = 0.0f;
	HurryUpRender->ActiveOff();
}

void APlayerFocus::Run(float _DeltaTime)
{
	ForcusTime += _DeltaTime;
	// Player를 반박자 뒤에 따라다녀야 함.
	FVector PlayerPos = AEgseu::GetMainPlayer()->GetActorLocation();
	PlayerPos.Y -= 50.0f;
	FVector PlayerDir = PlayerPos - this->GetActorLocation();
	PlayerDir.Normalize2D();

	RunVector = PlayerDir * RunSpeed * _DeltaTime;
	AddActorLocation(RunVector);
	if (ForcusTime >= 10.0f)
	{
		HurryUpRender->ActiveOn();
	}
}
#pragma endregion

// Level에서 State를 바꾸면 이 쪽으로.
void APlayerFocus::Rank(float _DeltaTime)
{
	ForcusTime = 0.0f;
	StateChange(EFocusState::RunUp);
	return;
}

#pragma region RunUp
void APlayerFocus::RunUpStart()
{
	RunVector = FVector::Zero;
	FocusRender->ChangeAnimation("FocusEnd");
	HurryUpRender->ActiveOff();
	FocusArrow->ActiveOff();
	
	// 끝나는 위치 = 내 위치 + 목표거리.
	UpEndPos = GetActorLocation().iY() - UpTargetLen;
}
void APlayerFocus::RunUp(float _DeltaTime)
{
	RunVector = FVector::Up * RunSpeed * _DeltaTime;
	AddActorLocation(RunVector);

	// 움직이는 내 좌표와 끝나는 좌표랑 비교.
	int CurPos = GetActorLocation().iY();
	if (UpEndPos == CurPos)
	{
		StateChange(EFocusState::RunRight);
		return;
	}
}
#pragma endregion


#pragma region RunRight
void APlayerFocus::RunRightStart()
{
	RunVector = FVector::Zero;
	FocusRender->ChangeAnimation("FocusIcon");
}

void APlayerFocus::RunRight(float _DeltaTime)
{
	RunRightTime += _DeltaTime;
	RunVector = FVector::Right * RunSpeed * _DeltaTime;
	AddActorLocation(RunVector);

	// 오른쪽으로 이동.
	if (2.0f <= RunRightTime)
	{
		StateChange(EFocusState::Death);
		return;
	}
}
#pragma endregion


#pragma region Death
void APlayerFocus::DeathStart()
{
	
}

void APlayerFocus::Death(float _DeltaTime)
{
	this->Destroy(0.0f);
}
#pragma endregion

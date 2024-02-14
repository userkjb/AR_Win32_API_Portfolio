#include "EngineCore.h"
#include <Windows.h>
#include "Level.h"
#include <EnginePlatform/EngineInput.h>

UEngineCore* GEngine = nullptr;

UEngineCore::UEngineCore()
{
}

UEngineCore::~UEngineCore()
{
}

void UEngineCore::EngineStart(HINSTANCE _hInstance)
{
	GEngine = this;
	MainTimer.TimeCheckStart();
	CoreInit(_hInstance);
	BeginPlay();
	UEngineWindow::WindowMessageLoop(EngineTick, EngineEnd);
}

void UEngineCore::CoreInit(HINSTANCE _HINSTANCE)
{
	if (true == EngineInit)
	{
		return;
	}

	UEngineWindow::Init(_HINSTANCE);
	MainWindow.Open();

	EngineInit = true;
}

void UEngineCore::BeginPlay()
{}

void UEngineCore::Tick(float _DeltaTime)
{}

void UEngineCore::End()
{}

void UEngineCore::ChangeLevel(std::string_view _Name)
{
	std::string UpperName = UEngineString::ToUpper(_Name);

	if (false == AllLevel.contains(UpperName))
	{
		MsgBoxAssert(std::string(_Name) + "라는 존재하지 않는 레벨로 체인지 하려고 했습니다");
	}

	NextLevel = AllLevel[UpperName];
}

void UEngineCore::EngineTick()
{
	GEngine->CoreTick();
}

void UEngineCore::CoreTick()
{
	float DeltaTime = MainTimer.TimeCheck();
	double dDeltaTime = MainTimer.GetDeltaTime();

	if (1 <= Frame)
	{
		CurFrameTime += DeltaTime;

		if (CurFrameTime <= FrameTime)
		{
			return;
		}

		CurFrameTime -= FrameTime;
		DeltaTime = FrameTime;
	}

	UEngineInput::KeyCheckTick(DeltaTime);

	// 한 프레임 동안 레벨이 절대 변하지 않고,
	// 프레임이 시작할 때 Level이 변화 한다.
	// 이를 지키기 위해서 Release 프레임이 실행되는 동안에
	// 절대로 구조를 바꾸지 않는다.
	if (nullptr != NextLevel)
	{
		// 최초에는 현재 레벨이 존재하지 않는다. = 바꿀 레벨이 있다.
		if (nullptr != CurLevel)
		{
			// 레벨링이 끝났다는 것을 알린다.
			CurLevel->LevelEnd(NextLevel);
		}

		NextLevel->LevelStart(CurLevel);
		CurLevel = NextLevel;
		NextLevel = nullptr;
	}

	if (nullptr == GEngine->CurLevel)
	{
		MsgBoxAssert("엔진을 시작할 레벨이 지정되지 않았습니다 치명적인 오류입니다");
	}


	// 레벨이 먼저 틱을 돌리고
	CurLevel->Tick(DeltaTime);
	// Actor와 부가적인 Object들의 Tick을 돌린다. => 행동한다.
	CurLevel->LevelTick(DeltaTime);

	// Renderer 들의 Render를 통해서 화면에 그림을 그린다.
	MainWindow.ScreenClear();
	CurLevel->LevelRender(DeltaTime);
	MainWindow.ScreenUpdate();

	// 정리한다.(파괴할 오브젝트들을 다 파괴한다.)
	CurLevel->LevelRelease(DeltaTime);
}

void UEngineCore::EngineEnd()
{
	for (std::pair<const std::string, ULevel*>& _Pair : GEngine->AllLevel)
	{
		if (nullptr == _Pair.second)
		{
			continue;
		}

		delete _Pair.second;
		_Pair.second = nullptr;
	}

	GEngine->AllLevel.clear();
}

void UEngineCore::LevelInit(ULevel* _Level, std::string_view _Name)
{
	_Level->SetName(_Name);
	_Level->BeginPlay();
}

#include "EngineCore.h"
#include <Windows.h>
#include "Level.h"
#include <EnginePlatform/EngineInput.h>

EngineCore* GEngine = nullptr;

EngineCore::EngineCore()
{
}

EngineCore::~EngineCore()
{
}

void EngineCore::EngineStart(HINSTANCE _hInstance, EngineCore* _UserCore)
{
	EngineCore* Ptr = _UserCore;
	MainTimer.TimeCheckStart();
	GEngine = Ptr;
	Ptr->CoreInit(_hInstance);
	Ptr->BeginPlay();
	EngineWindow::WindowMessageLoop(EngineTick, EngineEnd);
}

void EngineCore::CoreInit(HINSTANCE _HINSTANCE)
{
	if (true == EngineInit)
	{
		return;
	}

	EngineWindow::Init(_HINSTANCE);
	MainWindow.Open();

	EngineInit = true;
}

void EngineCore::BeginPlay()
{}

void EngineCore::Tick(float _DeltaTime)
{}

void EngineCore::End()
{}

void EngineCore::ChangeLevel(std::string_view _Name)
{
	std::string UpperName = EngineString::ToUpper(_Name);

	if (false == AllLevel.contains(UpperName))
	{
		MsgBoxAssert(std::string(_Name) + "��� �������� �ʴ� ������ ü���� �Ϸ��� �߽��ϴ�");
	}

	CurLevel = AllLevel[UpperName];
}

void EngineCore::EngineTick()
{
	GEngine->CoreTick();
}

void EngineCore::CoreTick()
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

	EngineInput::KeyCheckTick(DeltaTime);

	if (nullptr == GEngine->CurLevel)
	{
		MsgBoxAssert("������ ������ ������ �������� �ʾҽ��ϴ� ġ������ �����Դϴ�");
	}


	// ������ ���� ƽ�� ������
	CurLevel->Tick(DeltaTime);

	CurLevel->LevelTick(DeltaTime);
}

void EngineCore::EngineEnd()
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

void EngineCore::LevelInit(ULevel* _Level)
{
	_Level->BeginPlay();
}

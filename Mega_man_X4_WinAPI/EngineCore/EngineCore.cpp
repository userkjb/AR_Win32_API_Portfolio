#include "EngineCore.h"
#include <Windows.h>
#include "Level.h"
#include <EnginePlatform/EngineInput.h>

bool UEngineCore::IsDebugValue = false;
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

void UEngineCore::DestroyLevel(std::string_view _Name)
{
	std::string UpperName = UEngineString::ToUpper(_Name);

	if (false == AllLevel.contains(UpperName))
	{
		MsgBoxAssert(std::string(_Name) + "�������� �ʴ� ������ �ı��Ҽ��� �����ϴ�");
	}

	DestroyLevelName.push_back(UpperName);
}

void UEngineCore::ChangeLevel(std::string_view _Name)
{
	std::string UpperName = UEngineString::ToUpper(_Name);

	if (false == AllLevel.contains(UpperName))
	{
		MsgBoxAssert(std::string(_Name) + "��� �������� �ʴ� ������ ü���� �Ϸ��� �߽��ϴ�");
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

	if (1.0f / 60.0f <= DeltaTime)
	{
		DeltaTime = 1.0f / 60.0f;
	}

	UEngineInput::KeyCheckTick(DeltaTime);

	// �� ������ ���� ������ ���� ������ �ʰ�,
	// �������� ������ �� Level�� ��ȭ �Ѵ�.
	// �̸� ��Ű�� ���ؼ� Release �������� ����Ǵ� ���ȿ�
	// ����� ������ �ٲ��� �ʴ´�.
	if (nullptr != NextLevel)
	{
		// ���ʿ��� ���� ������ �������� �ʴ´�. = �ٲ� ������ �ִ�.
		if (nullptr != CurLevel)
		{
			// �������� �����ٴ� ���� �˸���.
			CurLevel->LevelEnd(NextLevel);
		}

		NextLevel->LevelStart(CurLevel);
		CurLevel = NextLevel;
		NextLevel = nullptr;
		MainTimer.TimeCheckStart();
		DeltaTime = MainTimer.TimeCheck();
		CurFrameTime = 0.0f;
	}

	for (size_t i = 0; i < DestroyLevelName.size(); i++)
	{
		std::string UpperName = UEngineString::ToUpper(DestroyLevelName[i]);

		ULevel* Level = AllLevel[UpperName];
		if (nullptr != Level)
		{
			delete Level;
			Level = nullptr;
		}

		AllLevel.erase(DestroyLevelName[i]);
	}
	DestroyLevelName.clear();

	if (nullptr == CurLevel)
	{
		MsgBoxAssert("������ ������ ������ �������� �ʾҽ��ϴ� ġ������ �����Դϴ�");
	}

	GEngine->Tick(DeltaTime);

	// ������ ���� ƽ�� ������
	CurLevel->Tick(DeltaTime);
	// Actor�� �ΰ����� Object���� Tick�� ������. => �ൿ�Ѵ�.
	CurLevel->LevelTick(DeltaTime);

	// Renderer ���� Render�� ���ؼ� ȭ�鿡 �׸��� �׸���.
	MainWindow.ScreenClear();
	CurLevel->LevelRender(DeltaTime);
	MainWindow.ScreenUpdate();

	// �����Ѵ�.(�ı��� ������Ʈ���� �� �ı��Ѵ�.)
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

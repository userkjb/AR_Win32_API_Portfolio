#include "EngineCore.h"
#include <Windows.h>
#include "Level.h"

EngineCore::EngineCore()
{
}

EngineCore::~EngineCore()
{
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
{
}

void EngineCore::Tick(float _DeltaTime)
{
}

void EngineCore::End()
{
}

void EngineCore::ChangeLevel(std::string_view _Name)
{
	std::string Name = std::string(_Name);

	if (false == AllLevel.contains(Name))
	{
		MsgBoxAssert(std::string(_Name) + "��� �������� �ʴ� ������ ü���� �Ϸ��� �߽��ϴ�");
	}

	CurLevel = AllLevel[Name];
}

void EngineCore::LevelInit(ULevel* _Level)
{
	_Level->BeginPlay();
}

#pragma once
#include <EngineBase/EngineDebug.h>
#include <EnginePlatform/EngineWindow.h>
#include <EngineBase/EngineString.h>
#include <map>

class ULevel;

class EngineCore
{
public :
	// constructer destructer
	
	~EngineCore();

	// delete Function
	EngineCore(const EngineCore& _Other) = delete;
	EngineCore(EngineCore&& _Other) noexcept = delete;
	EngineCore& operator=(const EngineCore) = delete;
	EngineCore& operator=(EngineCore&& _Other) noexcept = delete;

	EngineWindow MainWindow;

	static void EngineStart(HINSTANCE _hInstance, EngineCore* _UserCore);

	void CoreInit(HINSTANCE _HINSTANCE);

	virtual void BeginPlay();
	virtual void Tick(float _DeltaTime);
	virtual void End();

	template<typename LevelType>
	void CreateLevel(std::string_view _Name)
	{
		std::string UpperName = EngineString::ToUpper(_Name);
		if (true == AllLevel.contains(UpperName))
		{
			MsgBoxAssert(std::string(UpperName) + "이라는 이름의 Level을 또 만들려고 했습니다");
		}
		LevelType* NewLevel = new LevelType();
		LevelInit(NewLevel);
		AllLevel.insert(std::pair<std::string, ULevel*>(UpperName, NewLevel));
	}

	void ChangeLevel(std::string_view _Name);

protected :
	EngineCore();

private :
	bool EngineInit = false;

	std::map<std::string, ULevel*> AllLevel;
	ULevel* CurLevel = nullptr;

	static void EngineTick();
	static void EngineEnd();

	void LevelInit(ULevel* _Level);
};

extern EngineCore* GEngine;

#define ENGINESTART(USERCORE) \
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, \
	_In_opt_ HINSTANCE hPrevInstance, \
	_In_ LPWSTR    lpCmdLine, \
	_In_ int       nCmdShow) \
{ \
	LeakCheck; \
	USERCORE NewUserCore; \
	USERCORE NewUserCore = USERCORE(); \
	EngineCore::EngineStart(hInstance, &NewUserCore); \
}
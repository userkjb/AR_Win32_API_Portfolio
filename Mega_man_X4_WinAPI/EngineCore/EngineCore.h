#pragma once
#include <EngineBase/EngineDebug.h>
#include <EnginePlatform/EngineWindow.h>
#include <EngineBase/EngineString.h>
#include <EngineBase/EngineTime.h>
#include <EnginePlatform/EngineInput.h>
#include <vector>
#include <map>

class ULevel;

class UEngineCore
{
public :
	// constructer destructer
	
	~UEngineCore();

	// delete Function
	UEngineCore(const UEngineCore& _Other) = delete;
	UEngineCore(UEngineCore&& _Other) noexcept = delete;
	UEngineCore& operator=(const UEngineCore) = delete;
	UEngineCore& operator=(UEngineCore&& _Other) noexcept = delete;

	UEngineWindow MainWindow;
	EngineTime MainTimer;

	void EngineStart(HINSTANCE _hInstance);

	void CoreInit(HINSTANCE _HINSTANCE);

	virtual void BeginPlay();
	virtual void Tick(float _DeltaTime);

	template<typename LevelType>
	void CreateLevel(std::string_view _Name)
	{
		std::string UpperName = UEngineString::ToUpper(_Name);
		if (true == AllLevel.contains(UpperName))
		{
			MsgBoxAssert(std::string(UpperName) + "이라는 이름의 Level을 또 만들려고 했습니다");
		}
		LevelType* NewLevel = new LevelType();
		LevelInit(NewLevel, _Name);
		AllLevel.insert(std::pair<std::string, ULevel*>(UpperName, NewLevel));
	}

	virtual void WindowOpen(std::string& _OutWindowTitle, std::string& _SmallIconPath) {}

	/// <summary>
	/// Level 파괴
	/// </summary>
	/// <param name="_Name"></param>
	void DestroyLevel(std::string_view _Name);

	/// <summary>
	/// Level 바꾸기 함수.
	/// </summary>
	/// <param name="_Name"></param>
	void ChangeLevel(std::string_view _Name);

	/// <summary>
	/// 프래임 설정 함수.
	/// </summary>
	/// <param name="_Frame"></param>
	void SetFrame(int _Frame)
	{
		Frame = _Frame;
		FrameTime = 1 / static_cast<float>(Frame);
	}

	static bool IsDebug()
	{
		return IsDebugValue;
	}

	static void EngineDebugSwitch()
	{
		IsDebugValue = !IsDebugValue;
	}

protected :
	UEngineCore();

private :
	static bool IsDebugValue;

	int Frame = -1;
	float FrameTime = 0.0f;
	float CurFrameTime = 0.0f;
	bool EngineInit = false;

	std::string WindowTitle;
	std::string WindowIconPath;

	/// <summary>
	/// Level 들을 담는 Map 선언.
	/// </summary>
	std::map<std::string, ULevel*> AllLevel;
	ULevel* CurLevel = nullptr;
	ULevel* NextLevel = nullptr; // TODO

	std::vector<std::string> DestroyLevelName;

	static void EngineTick();
	void CoreTick();
	static void EngineEnd();

	void LevelInit(ULevel* _Level, std::string_view _Name);
};

// 모든 프로젝트, 파일에서 접근할 수 있도록 extern 사용.
extern UEngineCore* GEngine;

#define ENGINESTART(USERCORE) \
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, \
	_In_opt_ HINSTANCE hPrevInstance, \
	_In_ LPWSTR    lpCmdLine, \
	_In_ int       nCmdShow) \
{ \
	LeakCheck; \
	ContentsCore NewUserCore = ContentsCore(); \
	NewUserCore.EngineStart(hInstance); \
}
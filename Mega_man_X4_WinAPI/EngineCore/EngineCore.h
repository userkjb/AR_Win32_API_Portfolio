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
			MsgBoxAssert(std::string(UpperName) + "�̶�� �̸��� Level�� �� ������� �߽��ϴ�");
		}
		LevelType* NewLevel = new LevelType();
		LevelInit(NewLevel, _Name);
		AllLevel.insert(std::pair<std::string, ULevel*>(UpperName, NewLevel));
	}

	virtual void WindowOpen(std::string& _OutWindowTitle, std::string& _SmallIconPath) {}

	/// <summary>
	/// Level �ı�
	/// </summary>
	/// <param name="_Name"></param>
	void DestroyLevel(std::string_view _Name);

	/// <summary>
	/// Level �ٲٱ� �Լ�.
	/// </summary>
	/// <param name="_Name"></param>
	void ChangeLevel(std::string_view _Name);

	/// <summary>
	/// ������ ���� �Լ�.
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
	/// Level ���� ��� Map ����.
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

// ��� ������Ʈ, ���Ͽ��� ������ �� �ֵ��� extern ���.
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
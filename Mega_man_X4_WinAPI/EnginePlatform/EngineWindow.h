#pragma once
#include <Windows.h>
#include <string>

class UWindowImage;

class EngineWindow
{
public :
	// constructer destructer
	EngineWindow();
	~EngineWindow();

	// delete Function
	EngineWindow(const EngineWindow& _Other) = delete;
	EngineWindow(EngineWindow&& _Other) noexcept = delete;
	EngineWindow& operator=(const EngineWindow) = delete;
	EngineWindow& operator=(EngineWindow&& _Other) noexcept = delete;

	void Open(std::string_view _Title = "Title");

	static void Init(HINSTANCE _hInst);
	static unsigned __int64 WindowMessageLoop(void(*_Update)(), void(*_End)());

	UWindowImage* GetWindowImage()
	{
		return WindowImage;
	}

	HDC GetWindowDC()
	{
		return hDC;
	}

protected :

private :
	static bool WindowLive;
	static HINSTANCE hInstance;
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	HWND hWnd = nullptr;
	HDC hDC = nullptr;

	// 윈도우와 직접적으로 연결된 DC를 가지고 있는 최종 이미지.
	UWindowImage* WindowImage = nullptr;
	// -> 윈도우를 오직 1번만 갱신.


};


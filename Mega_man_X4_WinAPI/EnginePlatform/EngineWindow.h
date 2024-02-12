#pragma once
#include <Windows.h>
#include <string>

class UWindowImage;

/// <summary>
/// 엔진에서 사용하는 Window.
/// </summary>
class UEngineWindow
{
public :
	// constructer destructer
	UEngineWindow();
	~UEngineWindow();

	// delete Function
	UEngineWindow(const UEngineWindow& _Other) = delete;
	UEngineWindow(UEngineWindow&& _Other) noexcept = delete;
	UEngineWindow& operator=(const UEngineWindow) = delete;
	UEngineWindow& operator=(UEngineWindow&& _Other) noexcept = delete;

	void Open(std::string_view _Title = "Title");

	static void Init(HINSTANCE _hInst);
	static unsigned __int64 WindowMessageLoop(void(*_Update)(), void(*_End)());

	UWindowImage* GetWindowImage()
	{
		return WindowImage;
	}

	UWindowImage* GetBackBufferImage()
	{
		return BackBufferImage;
	}

	// Player에서 호출하는데 이거 삭제 예정.
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

	// 더블 버퍼링을 위한 WindowImage.
	UWindowImage* BackBufferImage = nullptr;
};


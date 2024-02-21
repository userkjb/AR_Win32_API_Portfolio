#pragma once
#include <Windows.h>
#include <string>
#include <EngineBase\EngineMath.h>


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

	void SetWindowPosition(const FVector& _Pos);
	void SetWindowScale(const FVector& _Scale);

	void ScreenClear();
	void ScreenUpdate();

	FVector GetWindowScale()
	{
		return Scale;
	}

	void SetClearColor(Color8Bit _Color)
	{
		_Color.A = 0;
		ClearColor = _Color;
	}

	/// <summary>
	/// X4 에서 사용 안함.
	/// </summary>
	/// <returns></returns>
	FVector GetMousePosition();

	void Off()
	{
		WindowLive = false;
	}

protected :

private :
	static bool WindowLive;
	static HINSTANCE hInstance;
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	// 클리어 색상.
	Color8Bit ClearColor = Color8Bit::WhiteA;

	HWND hWnd = nullptr;
	//HDC hDC = nullptr;

	// 윈도우와 직접적으로 연결된 DC를 가지고 있는 최종 이미지.
	UWindowImage* WindowImage = nullptr;
	// -> 윈도우를 오직 1번만 갱신.

	// 더블 버퍼링을 위한 WindowImage.
	UWindowImage* BackBufferImage = nullptr;

	// Back Buffer Image는 WindowImage의 위치와 크기를 알아야 한다.
	FVector Scale;
	FVector Position;
};


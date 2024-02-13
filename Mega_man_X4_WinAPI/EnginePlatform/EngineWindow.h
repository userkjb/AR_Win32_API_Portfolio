#pragma once
#include <Windows.h>
#include <string>

class UWindowImage;

/// <summary>
/// �������� ����ϴ� Window.
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

	// Player���� ȣ���ϴµ� �̰� ���� ����. [TODO]
	HDC GetWindowDC()
	{
		return hDC;
	}

	void SetWindowPosition(const FVector& _Pos);
	void SetWindowScale(const FVector& _Scale);

protected :

private :
	static bool WindowLive;
	static HINSTANCE hInstance;
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	HWND hWnd = nullptr;
	HDC hDC = nullptr;

	// ������� ���������� ����� DC�� ������ �ִ� ���� �̹���.
	UWindowImage* WindowImage = nullptr;
	// -> �����츦 ���� 1���� ����.

	// ���� ���۸��� ���� WindowImage.
	UWindowImage* BackBufferImage = nullptr;

	// Back Buffer Image�� WindowImage�� ��ġ�� ũ�⸦ �˾ƾ� �Ѵ�.
	FVector Scale;
	FVector Position;
};


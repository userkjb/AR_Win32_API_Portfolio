#include "EngineWindow.h"
#include <EngineBase/EngineDebug.h>
#include "WindowImage.h"

bool UEngineWindow::WindowLive = true;
HINSTANCE UEngineWindow::hInstance;

void UEngineWindow::Init(HINSTANCE _hInst)
{
	hInstance = _hInst;
}

unsigned __int64 UEngineWindow::WindowMessageLoop(void(*_Update)(), void(*_End)())
{
	MSG msg = {};

	while (WindowLive)
	{
		// 기본 메시지 루프.
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			// Update 함수 호출하지 않고 바로 루프를 나감.
			if (false == WindowLive)
			{
				break;
			}
		}

		// 메시지 루프의 '데드 타임' 이라는 곳에서 실행.
		if (nullptr != _Update)
		{
			_Update();
		}
	}

	if (nullptr != _End)
	{
		_End();
	}

	return msg.wParam;
}

void UEngineWindow::SetWindowPosition(const FVector& _Pos)
{
	Position = _Pos;

	::SetWindowPos(hWnd, nullptr, Position.iX(), Position.iY(), 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

void UEngineWindow::SetWindowScale(const FVector& _Scale)
{
	Scale = _Scale;

	// 방어코드
	if (nullptr != BackBufferImage)
	{
		delete BackBufferImage;
		BackBufferImage = nullptr;
	}

	// Window 크기만한 이미지를 만들어야 한다.
	BackBufferImage = new UWindowImage();
	BackBufferImage->Create(WindowImage, Scale);

	// 만들어진 윈도우의 메뉴 크기 까지 고려해야 한다.
	// Ex) 1000, 1000 윈도우 만들어 줘 -> 1000, 1100 수치를 적어줘야 한다.
	// -> 즉, 윈도우의 부가요소 크기가지 다 포함해서 내부크기가 1000, 1000이 될 수 있도록 해야 한다.
	RECT Rc = { 0, 0, _Scale.iX(), _Scale.iY() };
	AdjustWindowRect(&Rc, WS_OVERLAPPEDWINDOW, FALSE);

	// SWP_NOMOVE 현재 위치를 유지합니다(X 및 Y 매개 변수 무시).
	// 크기 조절기능 + 위치조절 다들어가 있다.
	::SetWindowPos(hWnd, nullptr, 0, 0, Rc.right - Rc.left, Rc.bottom - Rc.top, SWP_NOZORDER | SWP_NOMOVE);
}

void UEngineWindow::ScreenClear()
{
	HBRUSH myBrush = (HBRUSH)CreateSolidBrush(ClearColor.Color);
	HBRUSH oldBrush = (HBRUSH)SelectObject(BackBufferImage->ImageDC, myBrush);
	Rectangle(BackBufferImage->ImageDC, -1, -1, Scale.iX() + 1, Scale.iY() + 1);
	SelectObject(BackBufferImage->ImageDC, oldBrush);
	DeleteObject(myBrush);
}

void UEngineWindow::ScreenUpdate()
{
	FTransform CopyTrans;
	CopyTrans.SetPosition({ Scale.ihX(), Scale.ihY() });
	CopyTrans.SetScale({ Scale.iX(), Scale.iY() });

	WindowImage->BitCopy(BackBufferImage, CopyTrans);
}

FVector UEngineWindow::GetMousePosition()
{
	POINT MousePoint;
	GetCursorPos(&MousePoint);
	ScreenToClient(hWnd, &MousePoint);

	return FVector(MousePoint.x, MousePoint.y);
}

LRESULT CALLBACK UEngineWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		WindowLive = false;
		break;
	default :
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}




UEngineWindow::UEngineWindow()
{
}

UEngineWindow::~UEngineWindow()
{
	if (nullptr != BackBufferImage)
	{
		delete BackBufferImage;
		BackBufferImage = nullptr;
	}

	if (nullptr != WindowImage)
	{
		delete WindowImage;
		WindowImage = nullptr;
	}
}



void UEngineWindow::Open(std::string_view _Title)
{
	WNDCLASSEXA wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = nullptr;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = "DefaultWindow";
	wcex.hIconSm = nullptr;

	RegisterClassExA(&wcex);

	int Style = WS_OVERLAPPED |
		WS_CAPTION |
		WS_SYSMENU |
		WS_THICKFRAME |
		WS_MINIMIZEBOX |
		WS_MAXIMIZEBOX;

	hWnd = CreateWindowA("DefaultWindow", _Title.data(), Style,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		MsgBoxAssert("윈도우 생성에 실패했습니다.");
		return;
	}

	HDC hDC = GetDC(hWnd);
	//hDC = GetDC(hWnd);

	if (nullptr == WindowImage)
	{
		WindowImage = new UWindowImage();
		WindowImage->Create(hDC);
	}

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
}

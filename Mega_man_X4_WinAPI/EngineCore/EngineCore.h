#pragma once
#include <EnginePlatform/EngineWindow.h>

class EngineCore
{
public :
	// constructer destructer
	EngineCore();
	virtual ~EngineCore();

	// delete Function
	EngineCore(const EngineCore& _Other) = delete;
	EngineCore(EngineCore&& _Other) noexcept = delete;
	EngineCore& operator=(const EngineCore) = delete;
	EngineCore& operator=(EngineCore&& _Other) noexcept = delete;

	EngineWindow MainWindow;

	void CoreInit(HINSTANCE _HINSTANCE);

	virtual void EngineStart();
	virtual void EngineUpdate();
	virtual void EngineEnd();

protected :

private :
	bool EngineInit = false;
};

#define ENGINESTART(USERCORE) \
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, \
	_In_opt_ HINSTANCE hPrevInstance, \
	_In_ LPWSTR    lpCmdLine, \
	_In_ int       nCmdShow) \
{ \
	USERCORE NewUserCore; \
	EngineCore* Ptr = &NewUserCore; \
	Ptr->CoreInit(hInstance); \
	Ptr->EngineStart(); \
	EngineWindow::WindowMessageLoop(); \
}
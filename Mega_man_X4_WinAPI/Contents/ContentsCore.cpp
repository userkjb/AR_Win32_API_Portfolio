#include "ContentsCore.h"
//#include "TitleLevel.h"
#include "IntroLevel.h"
#include "TitleLevel.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>


ContentsCore::ContentsCore()
{
}

ContentsCore::~ContentsCore()
{
}

/// <summary>
/// EngineCore -- 
/// Game Start
/// </summary>
void ContentsCore::BeginPlay()
{
	// Main Buffer Image 설정
	//MainWindow.SetWindowScale({ 256 * 4, 240 * 4 });
	MainWindow.SetWindowPosition({ 500, 100 }); // 출력 위치.
	MainWindow.SetWindowScale({ 800, 600 }); // 해상도

	// 부모 BeginPlay.
	UEngineCore::BeginPlay();

	// ==========================================================================

	// === Level ===
	CreateLevel<UTitleLevel>("TitleLevel");
	CreateLevel<UIntroLevel>("IntroLevel");

	ChangeLevel("IntroLevel");
}

void ContentsCore::Tick(float _DeltaTime)
{
	UEngineCore::Tick(_DeltaTime);
}

void ContentsCore::End()
{
	UEngineCore::End();
}

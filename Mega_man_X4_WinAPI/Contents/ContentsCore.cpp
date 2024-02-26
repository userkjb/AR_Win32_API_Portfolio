#include "ContentsCore.h"
#include "time.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>
#include <EnginePlatform/EngineInput.h>

//#include "TitleLevel.h"
#include "IntroLevel.h"
#include "TitleLevel.h"
#include "CyberSpaceLevel.h"

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
	// Main Buffer Image ����
	//MainWindow.SetWindowScale({ 256 * 4, 240 * 4 });
	MainWindow.SetWindowPosition({ 500, 100 }); // �����쿡�� ��� ��ġ.
	MainWindow.SetWindowScale({ 800, 600 }); // �ػ�
	//MainWindow.SetWindowPosition({ 0, 0 }); // �����쿡�� ��� ��ġ.
	//MainWindow.SetWindowScale({ 1920, 1080 }); // �ػ�

	// �θ� BeginPlay.
	UEngineCore::BeginPlay();

	srand(time(0));

	// ==========================================================================

	// === Level ===
	CreateLevel<UTitleLevel>("TitleLevel");
	CreateLevel<UIntroLevel>("IntroLevel");
	CreateLevel<UCyberSpaceLevel>("CyberSpaceLevel");

	ChangeLevel("CyberSpaceLevel");
}

void ContentsCore::Tick(float _DeltaTime)
{
	UEngineCore::Tick(_DeltaTime);

	if (true == UEngineInput::IsDown(VK_F2))
	{
		GEngine->EngineDebugSwitch();
	}
}

void ContentsCore::End()
{
	UEngineCore::End();
}

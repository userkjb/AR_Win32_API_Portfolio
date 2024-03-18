#include "ContentsCore.h"
//#include "time.h"
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/EngineResourcesManager.h>
#include <EnginePlatform/EngineInput.h>
#include <EnginePlatform/EngineSound.h>

//#include "TitleLevel.h"
#include "IntroLevel.h"
#include "TitleLevel.h"
#include "CyberSpaceLevel.h"
#include "CyberSpaceBossLevel.h"

ContentsCore::ContentsCore()
{
}

ContentsCore::~ContentsCore()
{
}

void ContentsCore::WindowOpen(std::string& _OutWindowTitle, std::string& _SmallIconPath)
{
	UEngineDirectory NewDir;
	NewDir.MoveToSearchChild("ContentsResources");

	_OutWindowTitle = "TestMyTitle";

	std::string IconPath = NewDir.AppendPath("Icon.ico");
	_SmallIconPath = IconPath;
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

	// ���콺 Ŀ�� ����.
	//MainWindow.CursorOff();

	// �θ� BeginPlay.
	UEngineCore::BeginPlay();

	//srand(static_cast<unsigned int>(time(0)));

	// �⺻ ���� ����
	UEngineSound::SetGlobalVolume(0.5f);

	// ==========================================================================

	UEngineDirectory NewDir;
	NewDir.MoveParent();
	NewDir.Move("ContentsResources\\Sound");

	std::list<UEngineFile> NewList = NewDir.AllFile({ ".wav", ".mp3" }, true);

	for (UEngineFile& File : NewList)
	{
		UEngineSound::Load(File.GetFullPath());
	}


	// === Level ===
	CreateLevel<UTitleLevel>("TitleLevel");
	CreateLevel<UIntroLevel>("IntroLevel");
	CreateLevel<UCyberSpaceLevel>("CyberSpaceLevel");
	CreateLevel<UCyberSpaceBossLevel>("CyberSpaceBossLevel");

	ChangeLevel("TitleLevel");
	//ChangeLevel("CyberSpaceBossLevel");
}

void ContentsCore::Tick(float _DeltaTime)
{
	UEngineCore::Tick(_DeltaTime);

	if (true == UEngineInput::IsDown(VK_F2))
	{
		GEngine->EngineDebugSwitch();
	}
}


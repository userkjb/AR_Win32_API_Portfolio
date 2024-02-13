#include "ContentsCore.h"
//#include "TitleLevel.h"
#include "IntroLevel.h"
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
	// BackBufferImage ����
	MainWindow.SetWindowScale({ 256 * 4, 240 * 4 });
	MainWindow.SetWindowPosition({ 800, 0 });

	// �θ� BeginPlay.
	UEngineCore::BeginPlay();

	// Resource ���� ��� ����.
	UEngineDirectory NewDir;
	NewDir.MoveParent();
	NewDir.Move("ContentsResources");

	std::list<UEngineFile> NewList = NewDir.AllFile({ ".png", ".bmp" }, true);

	for (UEngineFile& File : NewList)
	{
		UEngineResourcesManager::GetInst().LoadImg(File.GetFullPath());
	}




	CreateLevel<UIntroLevel>("IntroLevel");

	ChangeLevel("IntroLevel");
}

void ContentsCore::Tick(float _DeltaTime)
{
}

void ContentsCore::End()
{
}

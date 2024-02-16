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
	MainWindow.SetWindowScale({ 800, 600 });
	MainWindow.SetWindowPosition({ 500, 100 });

	// 부모 BeginPlay.
	UEngineCore::BeginPlay();

	// Resource 폴더 경로 설정.
	UEngineDirectory NewDir;
	NewDir.MoveParent();
	NewDir.Move("ContentsResources");

	std::list<UEngineFile> NewList = NewDir.AllFile({ ".png", ".bmp" }, true);

	for (UEngineFile& File : NewList)
	{
		UEngineResourcesManager::GetInst().LoadImg(File.GetFullPath());
	}

	// Title Map
	UEngineResourcesManager::GetInst().CuttingImage("Menu.png", 3, 1);

	// Intro Map
	UEngineResourcesManager::GetInst().CuttingImage("x_Idle_Right.png", 5, 1);
	UEngineResourcesManager::GetInst().CuttingImage("x_Idle_Left.png", 5, 1);
	UEngineResourcesManager::GetInst().CuttingImage("x_Move_Right.png", 16, 1);
	UEngineResourcesManager::GetInst().CuttingImage("x_Move_Left.png", 16, 1);
	//UEngineResourcesManager::GetInst().CuttingImage("x_Jump_Right.png", 11, 1);
	//UEngineResourcesManager::GetInst().CuttingImage("x_Jump_Left.png", 11, 1);

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

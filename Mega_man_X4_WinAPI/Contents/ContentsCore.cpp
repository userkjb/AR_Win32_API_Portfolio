#include "ContentsCore.h"
//#include "TitleLevel.h"
#include "IntroLevel.h"


ContentsCore::ContentsCore()
{
}

ContentsCore::~ContentsCore()
{
}

/// <summary>
/// Game Start
/// </summary>
void ContentsCore::BeginPlay()
{
	CreateLevel<UIntroLevel>("IntroLevel");

	ChangeLevel("IntroLevel");
}

void ContentsCore::Tick(float _DeltaTime)
{
}

void ContentsCore::End()
{
}

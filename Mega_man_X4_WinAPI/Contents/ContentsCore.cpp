#include "ContentsCore.h"
#include "TitleLevel.h"


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
	CreateLevel<UTitleLevel>("TitleLevel");
}

void ContentsCore::Tick(float _DeltaTime)
{
}

void ContentsCore::End()
{
}

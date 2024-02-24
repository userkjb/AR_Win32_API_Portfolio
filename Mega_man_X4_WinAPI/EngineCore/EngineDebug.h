#pragma once
#include <EngineBase/EngineDebug.h>
#include <EnginePlatform/EngineWindow.h>

/// <summary>
/// 게임 화면에 Text를 출력하는 Debug
/// </summary>
namespace UEngineDebug
{
	void DebugTextPrint(std::string_view _Text, float _Size);
	void PrintDebugText(UWindowImage* _Image);
}


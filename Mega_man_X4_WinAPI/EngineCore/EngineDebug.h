#pragma once
#include <EngineBase/EngineDebug.h>
#include <EnginePlatform/EngineWindow.h>

/// <summary>
/// ���� ȭ�鿡 Text�� ����ϴ� Debug
/// </summary>
namespace UEngineDebug
{
	void DebugTextPrint(std::string_view _Text, float _Size);
	void PrintDebugText(UWindowImage* _Image);
}


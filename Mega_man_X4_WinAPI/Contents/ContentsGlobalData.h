#pragma once

#include <EngineBase/EngineMath.h>
#include <EngineCore/EngineCore.h>
#include <EngineCore/ImageRenderer.h>

/// <summary>
/// Player ����. [FSM]
/// </summary>
enum class EPlayerState
{
	None,
	Idle,
	Run,
	Jump,
};


class UContentsGlobalData
{
public :

private :
	UContentsGlobalData();
	~UContentsGlobalData();
};


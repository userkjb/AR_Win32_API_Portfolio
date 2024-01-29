#pragma once
class EngineInput
{
public :
	// constructer destructer
	EngineInput();
	virtual ~EngineInput();

	// delete Function
	EngineInput(const EngineInput& _Other) = delete;
	EngineInput(EngineInput&& _Other) noexcept = delete;
	EngineInput& operator=(const EngineInput) = delete;
	EngineInput& operator=(EngineInput&& _Other) noexcept = delete;

protected :

private :
	static void InputInit();
};


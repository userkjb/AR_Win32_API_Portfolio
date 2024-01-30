#pragma once

#include <map>
#include <Windows.h>

class EngineInput
{

private:
	class EngineKey
	{
	public :
		EngineKey()
		{}
		EngineKey(int _Key)
			: Key(_Key)
		{}
		bool Down = false; // 누른 순간
		bool Press = false; // 계속 누르면
		bool Up = false; // 떼어진 순간
		bool Free = true; // 누리지 않으면

		int Key = -1; // 기본 키 값.

		void KeyCheck();
	};


public :
	// constructer destructer
	EngineInput();
	virtual ~EngineInput();

	// delete Function
	EngineInput(const EngineInput& _Other) = delete;
	EngineInput(EngineInput&& _Other) noexcept = delete;
	EngineInput& operator=(const EngineInput) = delete;
	EngineInput& operator=(EngineInput&& _Other) noexcept = delete;

	static bool IsDown(int _Key);
	static bool IsPress(int _Key);
	static bool IsUp(int _Key);
	static bool IsFree(int _Key);

	static void KeyCheckTick(float _DeltaTime);
protected :
	static std::map<int, EngineKey> AllKeys;
private :
	static void InputInit();
};


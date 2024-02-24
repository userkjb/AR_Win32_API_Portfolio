#pragma once

#include <map>
#include <Windows.h>
#include <EngineBase\EngineDebug.h>

class UEngineInput
{
	// �ʱ�ȭ�� ���� class friend.
	friend class InputInitCreator;

private:
	class EngineKey
	{
	public :
		EngineKey()
		{}
		EngineKey(int _Key)
			: Key(_Key)
		{}
		bool Down = false; // ���� ����
		bool Press = false; // ��� ������
		bool Up = false; // ������ ����
		bool Free = true; // ������ ������

		float PressTime = 0.0f;
		float UpTime = 0.0f;

		int Key = -1; // �⺻ Ű ��.

		void KeyCheck(float _DeltaTime);
	};


public :
	// constructer destructer
	UEngineInput();
	virtual ~UEngineInput();

	// delete Function
	UEngineInput(const UEngineInput& _Other) = delete;
	UEngineInput(UEngineInput&& _Other) noexcept = delete;
	UEngineInput& operator=(const UEngineInput) = delete;
	UEngineInput& operator=(UEngineInput&& _Other) noexcept = delete;

	// ===== �ܺο��� ����� �Լ���. =====

	static bool IsDoubleClick(int _Key, float _ClickTime)
	{
		if (false == AllKeys.contains(_Key))
		{
			MsgBoxAssert("�Է� ������ �������� �ʴ� Ű �Դϴ�.");
		}

		bool Value = AllKeys[_Key].Down;
		float Time = AllKeys[_Key].UpTime;

		if (true == AllKeys[_Key].Down && AllKeys[_Key].UpTime < _ClickTime)
		{
			return true;
		}

		return false;
	}

	static bool IsDown(int _Key)
	{
		if (AllKeys.contains(_Key) == false)
		{
			MsgBoxAssert("�Է¼����� �������� �ʴ� Ű �Դϴ�");
		}

		return AllKeys[_Key].Down;
	}

	static float GetPressTime(int _Key)
	{
		if (false == AllKeys.contains(_Key))
		{
			MsgBoxAssert("�Է¼����� �������� �ʴ� Ű �Դϴ�.");
		}

		return AllKeys[_Key].PressTime;
	}

	static bool IsPress(int _Key)
	{
		if (false == AllKeys.contains(_Key))
		{
			MsgBoxAssert("�Է¼����� �������� �ʴ� Ű �Դϴ�");
		}

		return AllKeys[_Key].Press;
	}
	static bool IsUp(int _Key)
	{
		if (false == AllKeys.contains(_Key))
		{
			MsgBoxAssert("�Է¼����� �������� �ʴ� Ű �Դϴ�");
		}

		return AllKeys[_Key].Up;
	}
	static bool IsFree(int _Key)
	{
		if (false == AllKeys.contains(_Key))
		{
			MsgBoxAssert("�Է¼����� �������� �ʴ� Ű �Դϴ�");
		}

		return AllKeys[_Key].Free;
	}

	static bool IsAnykeyDown()
	{
		return AnykeyDown;
	}
	static bool IsAnykeyPress()
	{
		return AnykeyPress;
	}
	static bool IsAnykeyUp()
	{
		return AnykeyUp;
	}
	static bool IsAnykeyFree()
	{
		return AnykeyFree;
	}

	static void KeyCheckTick(float _DeltaTime);
protected :
	//              'A'  ���� �?
	static std::map<int, EngineKey> AllKeys;

	static bool AnykeyDown;
	static bool AnykeyPress;
	static bool AnykeyUp;
	static bool AnykeyFree;

private :
	static void InputInit();
};


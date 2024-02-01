#pragma once

#include <map>
#include <Windows.h>
#include <EngineBase\EngineDebug.h>

class EngineInput
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

		int Key = -1; // �⺻ Ű ��.

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

	// �ܺο��� ����� �Լ���.
	static bool IsDown(int _Key)
	{
		if (AllKeys.contains(_Key) == false)
		{
			MsgBoxAssert("�Է¼����� �������� �ʴ� Ű �Դϴ�");
		}

		return AllKeys[_Key].Down;
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

	static void KeyCheckTick(float _DeltaTime);
protected :
	static std::map<int, EngineKey> AllKeys;
private :
	static void InputInit();
};

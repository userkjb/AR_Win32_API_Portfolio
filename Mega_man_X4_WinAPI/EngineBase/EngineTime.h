#pragma once
#include <Windows.h>

class EngineTime
{
public :
	// constrcuter destructer
	EngineTime();
	~EngineTime();

	// delete Function
	EngineTime(const EngineTime& _Other) = delete;
	EngineTime(EngineTime&& _Other) noexcept = delete;
	EngineTime& operator=(const EngineTime& _Other) = delete;
	EngineTime& operator=(EngineTime&& _Other) noexcept = delete;

	// �ð� ���� ����.
	void TimeCheckStart();

	// �󸶳� �������� Check
	float TimeCheck();

	double GetDeltaTime()
	{
		return DeltaTime;
	}

protected :

private :
	LARGE_INTEGER Count;
	LARGE_INTEGER PrevTime;
	LARGE_INTEGER CurTime;
	double dCount;
	double DeltaTime;
};

//typedef union _LARGE_INTEGER {
//    struct {
//        DWORD LowPart;
//        LONG HighPart;
//    } DUMMYSTRUCTNAME;
//    struct {
//        DWORD LowPart;
//        LONG HighPart;
//    } u;
//    LONGLONG QuadPart;
//} LARGE_INTEGER;
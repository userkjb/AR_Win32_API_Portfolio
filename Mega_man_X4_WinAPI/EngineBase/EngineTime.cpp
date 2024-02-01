#include "EngineTime.h"

EngineTime::EngineTime()
{
	TimeCheckStart();
}

EngineTime::~EngineTime()
{
}

void EngineTime::TimeCheckStart()
{
	// �ʴ� �� �� �ִ� ��
	QueryPerformanceFrequency(&Count);
	dCount = static_cast<double>(Count.QuadPart);

	// ȣ�� ���� ���� ���� �� ��
	QueryPerformanceCounter(&PrevTime);
}

float EngineTime::TimeCheck()
{
	QueryPerformanceCounter(&CurTime);
	__int64 Tick = (CurTime.QuadPart - PrevTime.QuadPart);

	// ���е��� ���̱� ���� float�� �ƴ� double�� ���.
	double dTick = static_cast<double>(Tick);
	DeltaTime = dTick / dCount;
	PrevTime.QuadPart = CurTime.QuadPart;
	return static_cast<float>(DeltaTime);
}

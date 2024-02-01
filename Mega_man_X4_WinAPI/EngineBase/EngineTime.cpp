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
	// 초당 샐 수 있는 값
	QueryPerformanceFrequency(&Count);
	dCount = static_cast<double>(Count.QuadPart);

	// 호출 기준 현재 까지 샌 값
	QueryPerformanceCounter(&PrevTime);
}

float EngineTime::TimeCheck()
{
	QueryPerformanceCounter(&CurTime);
	__int64 Tick = (CurTime.QuadPart - PrevTime.QuadPart);

	// 정밀도를 높이기 위해 float이 아닌 double로 계산.
	double dTick = static_cast<double>(Tick);
	DeltaTime = dTick / dCount;
	PrevTime.QuadPart = CurTime.QuadPart;
	return static_cast<float>(DeltaTime);
}

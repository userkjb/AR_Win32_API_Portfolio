#include "Actor.h"

AActor::AActor()
{
}

AActor::~AActor()
{
}

void AActor::Destroy(float _DestroyTime)
{
	// Actor �� �ڽ��� ���δ�.
	UTickObject::Destroy(_DestroyTime);

}

void AActor::DestroyUpdate(float _DeltaTime)
{
	UTickObject::DestroyUpdate(_DeltaTime);
}

void AActor::Tick(float _DeltaTime)
{
	UTickObject::Tick(_DeltaTime);
	DestroyUpdate(_DeltaTime);
}

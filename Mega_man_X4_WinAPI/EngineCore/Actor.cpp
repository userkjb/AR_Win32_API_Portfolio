#include "Actor.h"
#include "ImageRenderer.h"


AActor::AActor()
{
}

AActor::~AActor()
{
}

UImageRenderer* AActor::CreateImageRenderer(int Order)
{
	UImageRenderer* NewRenderer = new UImageRenderer();
	UActorComponent* ActorCom = NewRenderer;
	ActorCom->SetOwner(this);
	ActorCom->SetOrder(Order);
	ActorCom->BeginPlay();
	Renderers.push_back(NewRenderer);
	return NewRenderer;
}

void AActor::Destroy(float _DestroyTime)
{
	// Actor 는 자신을 죽인다.
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

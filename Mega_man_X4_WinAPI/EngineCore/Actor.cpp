#include "Actor.h"
#include "ImageRenderer.h"


AActor::AActor()
{
}

AActor::~AActor()
{
	for (UImageRenderer* ImageRenderer : Renderers)
	{
		if (nullptr == ImageRenderer)
		{
			// TickObject
			MsgBoxAssert("이미지 랜더러가 nullptr인 상황이 있었습니다");
		}

		delete ImageRenderer;
		ImageRenderer = nullptr;
	}

	Renderers.clear();

	// Collision도 지워줘야지
	for (UCollision* Collision : Collisions)
	{
		if (nullptr == Collision)
		{
			MsgBoxAssert("콜리전이 nullptr인 상황이었습니다.");
		}

		delete Collision;
		Collision = nullptr;
	}

	Collisions.clear();
}

UImageRenderer* AActor::CreateImageRenderer(int Order)
{
	UImageRenderer* Component = new UImageRenderer();
	UActorComponent* ActorCom = Component;
	ActorCom->SetOwner(this);
	ActorCom->SetOrder(Order);
	ActorCom->BeginPlay();
	Renderers.push_back(Component);
	return Component;
}

UCollision* AActor::CreateCollision(int _Order)
{
	UCollision* Component = new UCollision();
	UActorComponent* ActorCom = Component;
	ActorCom->SetOwner(this);
	ActorCom->SetOrder(_Order);
	ActorCom->BeginPlay();
	Collisions.push_back(Component);
	return Component;
}

void AActor::Destroy(float _DestroyTime)
{
	// Actor 는 자신을 죽인다.
	UTickObject::Destroy(_DestroyTime);

	// 자신에게 붙어있는 렌더러들도 죽인다.
	for (UImageRenderer* Renderer : Renderers)
	{
		Renderer->Destroy(_DestroyTime);
	}

	// 콜리전도 다 죽인다.
	for (UCollision* Collision : Collisions)
	{
		Collision->Destroy(_DestroyTime);
	}
}

void AActor::DestroyUpdate(float _DeltaTime)
{
	UTickObject::DestroyUpdate(_DeltaTime);

	for (UImageRenderer* Renderer : Renderers)
	{
		Renderer->DestroyUpdate(_DeltaTime);
	}

	for (UCollision* Collision : Collisions)
	{
		Collision->DestroyUpdate(_DeltaTime);
	}
}

void AActor::SetActive(bool _Active, float _ActiveTime)
{
	UTickObject::SetActive(_Active, _ActiveTime);

	for (UImageRenderer* Renderer : Renderers)
	{
		Renderer->SetActive(_Active, _ActiveTime);
	}

	for (UCollision* Collision : Collisions)
	{
		Collision->SetActive(_Active, _ActiveTime);
	}
}

void AActor::ActiveUpdate(float _DeltaTime)
{
	// 부모 호출.
	UTickObject::ActiveUpdate(_DeltaTime);

	for (UImageRenderer* Renderer : Renderers)
	{
		Renderer->ActiveUpdate(_DeltaTime);
	}

	for (UCollision* Collision : Collisions)
	{
		Collision->ActiveUpdate(_DeltaTime);
	}
}

void AActor::CheckReleaseChild()
{
	// Render
	{
		std::list<UImageRenderer*>::iterator StartIter = Renderers.begin();
		std::list<UImageRenderer*>::iterator EndIter = Renderers.end();

		for (; StartIter != EndIter;)
		{
			UImageRenderer* Renderer = StartIter.operator*();

			if (nullptr == Renderer)
			{
				MsgBoxAssert("Renderer가 nullptr인 경우가 존재했습니다");
				return;
			}

			if (false == Renderer->IsDestroy())
			{
				++StartIter;
				continue;
			}

			delete Renderer;
			Renderer = nullptr;
			StartIter = Renderers.erase(StartIter);
		}
	}

	// Collicion
	{
		std::list<UCollision*>::iterator StartIter = Collisions.begin();
		std::list<UCollision*>::iterator EndIter = Collisions.end();

		for (; StartIter != EndIter;)
		{
			UCollision* Collision = StartIter.operator*();

			if (nullptr == Collision)
			{
				MsgBoxAssert("Collision가 nullptr인 경우가 존재했습니다");
				return;
			}

			if (false == Collision->IsDestroy())
			{
				++StartIter;
				continue;
			}

			delete Collision;
			Collision = nullptr;
			StartIter = Collisions.erase(StartIter);
		}
	}
}

void AActor::AllRenderersActiveOff()
{
	for (UImageRenderer* Renderer : Renderers)
	{
		Renderer->ActiveOff();
	}
}

void AActor::AllRenderersActiveOn()
{
	for (UImageRenderer* Renderer : Renderers)
	{
		Renderer->ActiveOn();
	}
}

void AActor::Tick(float _DeltaTime)
{
	UTickObject::Tick(_DeltaTime);
	DestroyUpdate(_DeltaTime);
}

void AActor::ChildTick(float _DeltaTime)
{
	for (UImageRenderer* Renderer : Renderers)
	{
		Renderer->Tick(_DeltaTime);
	}
}

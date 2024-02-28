#include "Level.h"
#include "Actor.h"
#include <EngineBase/EngineDebug.h>
#include "EngineDebug.h"
#include "EngineCore.h"

ULevel::ULevel()
{
}

ULevel::~ULevel()
{
	for (std::pair<const int, std::list<AActor*>>& OrderListPair : AllActor)
	{
		std::list<AActor*>& ActorList = OrderListPair.second;
		for (AActor* Actor : ActorList)
		{
			if (nullptr == Actor)
			{
				continue;
			}

			delete Actor;
			Actor = nullptr;
		}
	}
	AllActor.clear();
}

void ULevel::ActorInit(AActor* _NewActor)
{
	_NewActor->SetWorld(this);
	_NewActor->BeginPlay();
}

void ULevel::LevelTick(float _DeltaTime)
{
	for (std::pair<const int, std::list<AActor*>>& OrderListPair : AllActor)
	{
		int Order = OrderListPair.first;

		if (false == TimeScale.contains(Order))
		{
			TimeScale[Order] = 1.0f;
		}

		float OrderTime = _DeltaTime * TimeScale[Order];

		std::list<AActor*>& ActorList = OrderListPair.second;
		for (AActor* Actor : ActorList)
		{
			Actor->ActiveUpdate(_DeltaTime);
			Actor->DestroyUpdate(_DeltaTime);
			if (false == Actor->IsActive())
			{
				continue;
			}

			Actor->Tick(OrderTime);
			Actor->ChildTick(OrderTime);
		}
	}
}

void ULevel::LevelRender(float _DeltaTime)
{
	for (std::pair<const int, std::list<UImageRenderer*>>& OrderListPair : Renderers)
	{
		std::list<UImageRenderer*>& RendererList = OrderListPair.second;
		for (UImageRenderer* Renderer : RendererList)
		{
			if (false == Renderer->IsActive())
			{
				continue;
			}

			Renderer->Render(_DeltaTime);
		}
	}
	
	// 디버깅 모드이면 Collision이 보이도록.
	if (true == GEngine->IsDebug()) // include EngineCore
	{
		for (std::pair<const int, std::list<UCollision*>>& OrderListPair : Collisions)
		{
			std::list<UCollision*>& CollisionList = OrderListPair.second;
			for (UCollision* Collision : CollisionList)
			{
				if (false == Collision->IsActive())
				{
					continue;
				}

				Collision->DebugRender(CameraPos);
			}
		}
		UEngineDebug::PrintDebugText(GEngine->MainWindow.GetBackBufferImage());
	}
}

void ULevel::LevelRelease(float _DeltaTime)
{
	// Collision Release
	for (std::pair<const int, std::list<UCollision*>>& OrderListPair : Collisions)
	{
		std::list<UCollision*>& List = OrderListPair.second;

		std::list<UCollision*>::iterator StartIter = List.begin();
		std::list<UCollision*>::iterator EndIter = List.end();

		for (; StartIter != EndIter; )
		{
			UCollision* Collision = StartIter.operator*();

			if (false == Collision->IsDestroy())
			{
				++StartIter;
				continue;
			}

			StartIter = List.erase(StartIter);
		}
	}

	// Render Release
	for (std::pair<const int, std::list<UImageRenderer*>>& OrderListPair : Renderers)
	{
		std::list<UImageRenderer*>& List = OrderListPair.second;

		std::list<UImageRenderer*>::iterator StartIter = List.begin();
		std::list<UImageRenderer*>::iterator EndIter = List.end();

		// 삭제는 절대로 Ranged for로 하면 안되다.
		// for (UImageRenderer* Renderer : RendererList)
		for (; StartIter != EndIter; )
		{
			UImageRenderer* Renderer = StartIter.operator*();

			if (false == Renderer->IsDestroy())
			{
				++StartIter;
				continue;
			}

			StartIter = List.erase(StartIter);
		}
	}

	// Actor Release
	for (std::pair<const int, std::list<AActor*>>& OrderListPair : AllActor)
	{
		std::list<AActor*>& ActorList = OrderListPair.second;

		std::list<AActor*>::iterator StartIter = ActorList.begin();
		std::list<AActor*>::iterator EndIter = ActorList.end();

		for (; StartIter != EndIter;)
		{
			AActor* Actor = StartIter.operator*();

			if (nullptr == Actor)
			{
				MsgBoxAssert("Actor가 nullptr인 경우가 존재했습니다");
				return;
			}

			if (false == Actor->IsDestroy())
			{
				Actor->CheckReleaseChild();
				++StartIter;
				continue;
			}

			delete Actor;
			Actor = nullptr;
			// Actor가 지워서 이미 없는데 Level은 그 사실을 모른다.
			// 그래서 delete 가 아니라 erase를 사용해서 청소를 하는 것이다.
			StartIter = ActorList.erase(StartIter);
		}
	}
}

#include "Collision.h"
#include "Level.h"
#include "Actor.h"

UCollision::UCollision()
{
}

UCollision::~UCollision()
{
}

void UCollision::SetOrder(int _Order)
{
	AActor* Owner = GetOwner();
	ULevel* Level = Owner->GetWorld();

	std::map<int, std::list<UCollision*>>& Collisions = Level->Collisions;

	// 내 오더가 바뀌기 전에 내가 어떤 그룹에 속해 있다면,
	// 그 그룹에서 빠져나온다.
	Collisions[GetOrder()].remove(this);

	// 내 오더 바꾸기
	UTickObject::SetOrder(_Order);

	Collisions[GetOrder()].push_back(this);
}

bool UCollision::CollisionCheck(int _Order, std::vector<UCollision*>& _Result)
{
	if(false == IsActive())
	{
		return false;
	}

	// 나와 특정 개수의 Collision과 충돌해야 한다.
	AActor* Owner = GetOwner();
	ULevel* Level = Owner->GetWorld();

	std::list<UCollision*>& Collisions = Level->Collisions[_Order];

	FTransform ThisTransform = GetActorBaseTransform();

	for (UCollision* _OtherCollision : Collisions)
	{
		if (this == _OtherCollision)
		{
			continue;
		}

		if (false == _OtherCollision->IsActive())
		{
			continue;
		}

		FTransform OtherTransform = _OtherCollision->GetActorBaseTransform();

		// 
		if (true == ThisTransform.Collision(ColType, _OtherCollision->ColType, OtherTransform))
		{
			_Result.push_back(_OtherCollision);
		}
	}

	return false == _Result.empty();
}

void UCollision::BeginPlay()
{
	USceneComponent::BeginPlay();
}

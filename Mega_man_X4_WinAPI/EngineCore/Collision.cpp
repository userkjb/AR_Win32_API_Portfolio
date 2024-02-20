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

	// �� ������ �ٲ�� ���� ���� � �׷쿡 ���� �ִٸ�,
	// �� �׷쿡�� �������´�.
	Collisions[GetOrder()].remove(this);

	// �� ���� �ٲٱ�
	UTickObject::SetOrder(_Order);

	Collisions[GetOrder()].push_back(this);
}

bool UCollision::CollisionCheck(int _Order, std::vector<UCollision*>& _Result)
{
	if(false == IsActive())
	{
		return false;
	}

	// ���� Ư�� ������ Collision�� �浹�ؾ� �Ѵ�.
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

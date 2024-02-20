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

void UCollision::BeginPlay()
{
	USceneComponent::BeginPlay();
}

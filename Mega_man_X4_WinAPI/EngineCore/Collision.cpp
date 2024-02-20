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

void UCollision::BeginPlay()
{
	USceneComponent::BeginPlay();
}

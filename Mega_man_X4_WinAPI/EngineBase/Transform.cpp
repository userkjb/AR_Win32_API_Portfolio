#include "Transform.h"
#include "EngineDebug.h"

bool (*FTransform::CollisionFunction[static_cast<int>(ECollisionType::Max)][static_cast<int>(ECollisionType::Max)])(const FTransform& _Left, const FTransform& _Right);

FTransform::FTransform()
{
}

FTransform::~FTransform()
{
}

bool FTransform::Collision(ECollisionType _ThisType, ECollisionType _OtherType, const FTransform& _Other)
{
	if (nullptr == CollisionFunction[static_cast<int>(_ThisType)][static_cast<int>(_OtherType)])
	{
		MsgBoxAssert("���� �浹 �Լ��� ������ ���� �浹�Դϴ�");
	}

	return CollisionFunction[static_cast<int>(_ThisType)][static_cast<int>(_OtherType)](*this, _Other);
}
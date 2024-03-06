#pragma once
#include <EngineCore/Actor.h>
#include "ContentsGlobalData.h"

class ABuster;

class AMiruTorearu : public AActor
{
public:
	// constrcuter destructer
	AMiruTorearu();
	~AMiruTorearu();

	// delete Function
	AMiruTorearu(const AMiruTorearu& _Other) = delete;
	AMiruTorearu(AMiruTorearu&& _Other) noexcept = delete;
	AMiruTorearu& operator=(const AMiruTorearu& _Other) = delete;
	AMiruTorearu& operator=(AMiruTorearu&& _Other) noexcept = delete;

protected :
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private :

	void BeginRender();


	UImageRenderer* MiruTorearuRender = nullptr; // ��ü �̹���
	UCollision* MiruTorearuCollision = nullptr; // ��ü �ݸ���
};


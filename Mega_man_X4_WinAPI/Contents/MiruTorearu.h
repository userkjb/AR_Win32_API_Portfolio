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


	UImageRenderer* MiruTorearuRender = nullptr; // 본체 이미지
	UCollision* MiruTorearuCollision = nullptr; // 본체 콜리전
};


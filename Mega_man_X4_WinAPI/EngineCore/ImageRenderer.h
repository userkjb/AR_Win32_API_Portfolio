#pragma once
#include <string_view>
#include "SceneComponent.h"
#include <EnginePlatform/WindowImage.h>

class UImageRenderer : public USceneComponent
{
public:
	// constrcuter destructer
	UImageRenderer();
	~UImageRenderer();

	// delete Function
	UImageRenderer(const UImageRenderer& _Other) = delete;
	UImageRenderer(UImageRenderer&& _Other) noexcept = delete;
	UImageRenderer& operator=(const UImageRenderer& _Other) = delete;
	UImageRenderer& operator=(UImageRenderer&& _Other) noexcept = delete;

	void SetImage(std::string_view _Name, int _InfoIndex = 0);

	void Render(float _DeltaTime);

protected :


private :
	UWindowImage* Image = nullptr;

};

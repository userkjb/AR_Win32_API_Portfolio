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

	void SetOrder(int _Order) override;
	void SetImage(std::string_view _Name, int _InfoIndex = 0);

	void SetTransform(const FTransform& _Value)
	{
		USceneComponent::SetTransform(_Value);
	}

	/// <summary>
	/// Level -> LevelRender�Լ����� ȣ���Ѵ�.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void Render(float _DeltaTime);

	void SetTransColor(Color8Bit _Color)
	{
		TransColor = _Color;
	}

	void SetImageCuttingTransform(const FTransform& _Value)
	{
		ImageCuttingTransform = _Value;
	}

	UWindowImage* GetImage() const
	{
		return Image;
	}

protected :
	/// <summary>
	/// USceneComponent
	/// </summary>
	void BeginPlay() override;


private :
	int InfoIndex = 0; // Index
	UWindowImage* Image = nullptr; // �̹���
	FTransform ImageCuttingTransform; // �̹������� �ڸ� ��ġ. / Set
	Color8Bit TransColor; // �ڸ� ����. / Set

};

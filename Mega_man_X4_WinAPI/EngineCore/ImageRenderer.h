#pragma once
#include <string_view>
#include "SceneComponent.h"
#include <EnginePlatform/WindowImage.h>
#include <map>

/// <summary>
/// Animation 기본 정의 Class
/// </summary>
class UAnimationInfo
{
public :
	// 애니메이션을 구성하는 것은 이미지이고
	// 이 이미지는 1장부터 시작한다.
	UWindowImage* Image = nullptr;  // 애니메이션을 구성하는 이미지
	std::string Name;				// 이미지 이름.
	int CurFrame = 0;				// 애니메이션의 현재 프레임.
	float CurTime = 0.0f;			// 애니메이션의 현재 시간.
	bool Loop = false;				// 반복할 것인지?
	bool IsEnd = false;				// 애니메이션의 상태.
	std::vector<float> Times;
	std::vector<int> Indexs;

	int Update(float _DeltaTime);
};

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
	/// Level -> LevelRender함수에서 호출한다.
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

	// ==== Animation ====

	/// <summary>
	/// <para>원하는 이미지의 시작과 끝를 가지고 애니메이션을 만든다.</para>
	/// <para>1, 3 을 넣으면 123 / 123 / 123 순으로 동작한다.</para>
	/// </summary>
	/// <param name="_AnimationName"></param>
	/// <param name="_ImageName"></param>
	/// <param name="_Start"></param>
	/// <param name="_End"></param>
	/// <param name="_Inter"></param>
	/// <param name="_Loop"></param>
	void CreateAnimation(
		std::string_view _AnimationName,
		std::string_view _ImageName,
		int _Start,
		int _End,
		float _Inter,
		bool _Loop = true
	);

	/// <summary>
	/// <para>원하는 이미지의 위치 숫자를 Vector로 받아 애니메이션을 만든다.</para>
	/// <para>{ 1, 2, 3 } 을 넣으면 123 / 123 로 동작한다.</para>
	/// <para>{ 1, 2, 3, 2 } 을 넣으면 1232 / 1232 로 동작한다.</para>
	/// </summary>
	/// <param name="_AnimationName"></param>
	/// <param name="_ImageName"></param>
	/// <param name="_Indexs"></param>
	/// <param name="_Inter"></param>
	/// <param name="_Loop"></param>
	void CreateAnimation(
		std::string_view _AnimationName,
		std::string_view _ImageName,
		std::vector<int> _Indexs,
		float _Inter,
		bool _Loop = true
	);

	void ChangeAnimation(std::string_view _AnimationName, bool _IsForce = false, int _StartIndex = 0, float _Time = -1.0f);
	void AnimationReset();

	bool IsCurAnimationEnd() const
	{
		return CurAnimation->IsEnd;
	}

	int GetCurAnimationFrame() const
	{
		return CurAnimation->CurFrame;
	}

	int GetCurAnimationImageFrame() const
	{
		const std::vector<int>& Indexs = CurAnimation->Indexs;
		return Indexs[CurAnimation->CurFrame];
	}

	float GetCurAnimationTime() const
	{
		return CurAnimation->CurTime;
	}

	FTransform GetRenderTransForm();

protected :
	/// <summary>
	/// USceneComponent
	/// </summary>
	void BeginPlay() override;


private :
	int InfoIndex = 0; // Index
	UWindowImage* Image = nullptr; // 이미지
	FTransform ImageCuttingTransform; // 이미지에서 자를 위치. / Set
	Color8Bit TransColor; // 자를 색상. / Set

	bool CameraEffect = true;

	// === Animation ===
	std::map<std::string, UAnimationInfo> AnimationInfos;
	UAnimationInfo* CurAnimation = nullptr;
};

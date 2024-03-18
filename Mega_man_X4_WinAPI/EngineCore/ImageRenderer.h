#pragma once
#include <string_view>
#include "SceneComponent.h"
#include <EnginePlatform/WindowImage.h>
#include <map>

/// <summary>
/// 이미지 정렬을 위한 enum
/// </summary>
enum class EImageSortType
{
	Center,
	Left,
	Right,
};

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

class AActor;
//class UWindowImage;

class UImageRenderer : public USceneComponent
{
	friend AActor;

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

	bool IsAnimation(std::string_view _Name);

	void SetImageIndex(int _InfoIndex)
	{
		InfoIndex = _InfoIndex;
	}

	int GetImageIndex()
	{
		return InfoIndex;
	}

	void SetTransform(const FTransform& _Value)
	{
		USceneComponent::SetTransform(_Value);
	}

	/// <summary>
	/// Level -> LevelRender함수에서 호출한다.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void Render(float _DeltaTime);

	void SetAngle(float _Angle)
	{
		Angle = _Angle;
	}

	void SetTransColor(Color8Bit _Color)
	{
		TransColor = _Color;
	}

	void SetImageCuttingTransform(const FTransform& _Value)
	{
		ImageCuttingTransform = _Value;
	}

	// 0~1.0f
	void SetAlpha(float _Alpha)
	{
		if (0.0f >= _Alpha)
		{
			_Alpha = 0.0f;
		}

		if (1.0f <= _Alpha)
		{
			_Alpha = 1.0f;
		}

		TransColor.A = static_cast<char>(_Alpha * 255.0f);
	}

	UWindowImage* GetImage() const
	{
		return Image;
	}

	void CameraEffectOff()
	{
		CameraEffect = false;
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

	/// <summary>
	/// 
	/// </summary>
	/// <param name="_AnimationName"></param>
	/// <param name="_ImageName"></param>
	/// <param name="_Indexs"></param>
	/// <param name="_Inters"></param>
	/// <param name="_Loop"></param>
	void CreateAnimation(
		std::string_view _AnimationName,
		std::string_view _ImageName,
		std::vector<int> _Indexs,
		std::vector<float> _Inters,
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

	UAnimationInfo* GetCurAnimation() const
	{
		return CurAnimation;
	}

	void TextRender(float _DeltaTime);

	/// <summary>
	/// 기존의 Render함수에 있던 내용을 옮겨 담음.
	/// </summary>
	/// <param name="_DeltaTime"></param>
	void ImageRender(float _DeltaTime);

	void SetText(std::string_view _Text)
	{
		Text = _Text;
	}

	void SetFont(std::string_view _Font)
	{
		Font = _Font;
	}
	void SetTextSize(float _Value)
	{
		Size = _Value;
	}
	void SetTextColor(Color8Bit _Color)
	{
		TextColor = _Color;
	}


	/// <summary>
	/// 카메라 비율 설정 함수.
	/// </summary>
	/// <param name="_Ratio"></param>
	void SetCameraRatio(float _Ratio)
	{
		CameraRatio = _Ratio;
	}

	/// <summary>
	/// 원하는 렌더의(이미지, 텍스트) 위치, 크기를 가져온다.
	/// </summary>
	/// <returns></returns>
	FTransform GetRenderTransForm();

	/// <summary>
	/// Text 효과 함수.
	/// </summary>
	/// <param name="_Effect"></param>
	void SetTextEffect(int _Effect = 0)
	{
		TextEffect = _Effect;
	}

	void SetTextSortOption(Gdiplus::StringAlignment _SortOption1, Gdiplus::StringAlignment _SortOption2)
	{
		SortOption1 = _SortOption1;
		SortOption2 = _SortOption2;
	}

	/// <summary>
	/// 이미지 정렬 인터페이스.
	/// </summary>
	/// <param name="_SortType"></param>
	void SetSortType(EImageSortType _SortType)
	{
		SortType = _SortType;
	}

	/// <summary>
	/// 이미지 크기 자동 설정 실행.
	/// </summary>
	void AutoImageScale(float _AutoScale = 1.0f)
	{
		AutoImageScaleValue = true;
		AutoImageScaleRatio = _AutoScale;
	}

protected :
	/// <summary>
	/// USceneComponent
	/// </summary>
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;


private :
	int InfoIndex = 0; // Index
	UWindowImage* Image = nullptr; // 이미지
	FTransform ImageCuttingTransform; // 이미지에서 자를 위치. / Set
	Color8Bit TransColor; // 자를 색상. / Set

	bool CameraEffect = true;
	float CameraRatio = 1.0f;

	// === Animation ===
	std::map<std::string, UAnimationInfo> AnimationInfos;
	UAnimationInfo* CurAnimation = nullptr;

	// 회전 변수.
	float Angle = 0.0f;

	// === Text ===
	std::string Text = "";
	std::string Font = "궁서";
	float Size = 10.0f;
	Color8Bit TextColor = Color8Bit::BlackA;
	Color8Bit TextColor2 = Color8Bit::BlackA; //추가
	int TextEffect = 0;

	bool AutoImageScaleValue = false;
	float AutoImageScaleRatio = 1.0f;

	EImageSortType SortType = EImageSortType::Center;

	Gdiplus::StringAlignment SortOption1 = Gdiplus::StringAlignment::StringAlignmentCenter;
	Gdiplus::StringAlignment SortOption2 = Gdiplus::StringAlignment::StringAlignmentCenter;
};

#pragma once
#include <string_view>
#include "SceneComponent.h"
#include <EnginePlatform/WindowImage.h>
#include <map>

/// <summary>
/// �̹��� ������ ���� enum
/// </summary>
enum class EImageSortType
{
	Center,
	Left,
	Right,
};

/// <summary>
/// Animation �⺻ ���� Class
/// </summary>
class UAnimationInfo
{
public :
	// �ִϸ��̼��� �����ϴ� ���� �̹����̰�
	// �� �̹����� 1����� �����Ѵ�.
	UWindowImage* Image = nullptr;  // �ִϸ��̼��� �����ϴ� �̹���
	std::string Name;				// �̹��� �̸�.
	int CurFrame = 0;				// �ִϸ��̼��� ���� ������.
	float CurTime = 0.0f;			// �ִϸ��̼��� ���� �ð�.
	bool Loop = false;				// �ݺ��� ������?
	bool IsEnd = false;				// �ִϸ��̼��� ����.
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
	/// Level -> LevelRender�Լ����� ȣ���Ѵ�.
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
	/// <para>���ϴ� �̹����� ���۰� ���� ������ �ִϸ��̼��� �����.</para>
	/// <para>1, 3 �� ������ 123 / 123 / 123 ������ �����Ѵ�.</para>
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
	/// <para>���ϴ� �̹����� ��ġ ���ڸ� Vector�� �޾� �ִϸ��̼��� �����.</para>
	/// <para>{ 1, 2, 3 } �� ������ 123 / 123 �� �����Ѵ�.</para>
	/// <para>{ 1, 2, 3, 2 } �� ������ 1232 / 1232 �� �����Ѵ�.</para>
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
	/// ������ Render�Լ��� �ִ� ������ �Ű� ����.
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
	/// ī�޶� ���� ���� �Լ�.
	/// </summary>
	/// <param name="_Ratio"></param>
	void SetCameraRatio(float _Ratio)
	{
		CameraRatio = _Ratio;
	}

	/// <summary>
	/// ���ϴ� ������(�̹���, �ؽ�Ʈ) ��ġ, ũ�⸦ �����´�.
	/// </summary>
	/// <returns></returns>
	FTransform GetRenderTransForm();

	/// <summary>
	/// Text ȿ�� �Լ�.
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
	/// �̹��� ���� �������̽�.
	/// </summary>
	/// <param name="_SortType"></param>
	void SetSortType(EImageSortType _SortType)
	{
		SortType = _SortType;
	}

	/// <summary>
	/// �̹��� ũ�� �ڵ� ���� ����.
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
	UWindowImage* Image = nullptr; // �̹���
	FTransform ImageCuttingTransform; // �̹������� �ڸ� ��ġ. / Set
	Color8Bit TransColor; // �ڸ� ����. / Set

	bool CameraEffect = true;
	float CameraRatio = 1.0f;

	// === Animation ===
	std::map<std::string, UAnimationInfo> AnimationInfos;
	UAnimationInfo* CurAnimation = nullptr;

	// ȸ�� ����.
	float Angle = 0.0f;

	// === Text ===
	std::string Text = "";
	std::string Font = "�ü�";
	float Size = 10.0f;
	Color8Bit TextColor = Color8Bit::BlackA;
	Color8Bit TextColor2 = Color8Bit::BlackA; //�߰�
	int TextEffect = 0;

	bool AutoImageScaleValue = false;
	float AutoImageScaleRatio = 1.0f;

	EImageSortType SortType = EImageSortType::Center;

	Gdiplus::StringAlignment SortOption1 = Gdiplus::StringAlignment::StringAlignmentCenter;
	Gdiplus::StringAlignment SortOption2 = Gdiplus::StringAlignment::StringAlignmentCenter;
};

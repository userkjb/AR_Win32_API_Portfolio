#pragma once

#include <Windows.h>
#include <EngineBase/PathObject.h>
#include <EngineBase/Transform.h>
#include <EngineBase/EngineDebug.h>

#include <objidl.h>
#include <gdiplus.h>

/// <summary>
/// �̹����� �о�� Type.
/// ������ ������ �� ���.
/// </summary>
enum class EImageLoadType
{
	IMG_Folder,
	IMG_Cutting,
};

enum class EWindowImageType
{
	IMG_NONE,
	IMG_BMP,
	IMG_PNG
};

class UImageInfo
{
public:
	HBITMAP hBitMap = nullptr;
	HDC ImageDC = nullptr;
	FTransform CuttingTrans;
	UImageInfo* RotationMaskImage = nullptr;
	EWindowImageType ImageType = EWindowImageType::IMG_NONE;
};

class UEngineWindow;
/// <summary>
/// DC å����.
/// </summary>
class UWindowImage : public UPathObject
{
	friend UEngineWindow;

public:
	// constrcuter destructer
	UWindowImage();
	~UWindowImage();

	// delete Function
	UWindowImage(const UWindowImage& _Other) = delete;
	UWindowImage(UWindowImage&& _Other) noexcept = delete;
	UWindowImage& operator=(const UWindowImage& _Other) = delete;
	UWindowImage& operator=(UWindowImage&& _Other) noexcept = delete;

	FVector GetScale();

	EWindowImageType GetImageType()
	{
		return ImageType;
	}

	bool Load(UWindowImage* _Image);

	/// <summary>
	/// �� ���� �̹����� �ƴ� �������� �̹������� �ϳ��� �������� �����ϴ� ��� ���.
	/// </summary>
	/// <param name="_Image"></param>
	/// <returns></returns>
	bool LoadFolder(UWindowImage* _Image);

	/// <summary>
	/// UWindowImage �̹����� FTransform �� ��ġ�� ũ��� ������ ī��.
	/// �̹����� �̹��� ũ���� �� �׸� �� �ִ�.
	/// </summary>
	/// <param name="_CopyImage"></param>
	/// <param name="_Trans"></param>
	void BitCopy(UWindowImage* _CopyImage, const FTransform& _Trans);

	/// <summary>
	/// ����� bmp ������ Ư�� ���� �����.
	/// </summary>
	/// <param name="_CopyImage"></param>
	/// <param name="_Trans">���� �̹���</param>
	/// <param name="_Index"></param>
	/// <param name="_Color">�⺻ Black</param>
	void TransCopy(UWindowImage* _CopyImage, const FTransform& _Trans, int _Index, Color8Bit _Color = Color8Bit::Black);

	/// <summary>
	/// ����� png ������ alpha ���� �����.
	/// </summary>
	/// <param name="_CopyImage"></param>
	/// <param name="_Trans"></param>
	/// <param name="_Index"></param>
	/// <param name="_Color"></param>
	void AlphaCopy(UWindowImage* _CopyImage, const FTransform& _Trans, int _Index, Color8Bit _Color = Color8Bit::Black);

	/// <summary>
	/// ȸ���� ���� Copy
	/// </summary>
	/// <param name="_CopyImage"></param>
	/// <param name="_Trans"></param>
	/// <param name="_Index"></param>
	/// <param name="_RadAngle"></param>
	void PlgCopy(UWindowImage* _CopyImage, const FTransform& _Trans, int _Index, float _RadAngle);

	/// <summary>
	/// �ؽ�Ʈ ����� ���� ������ Copy
	/// </summary>
	/// <param name="_Text">����ϰ��� �ϴ� string</param>
	/// <param name="_Font">Text ��Ʈ</param>
	/// <param name="_Size">���� Size</param>
	/// <param name="_Trans">���� ��� ��ġ</param>
	/// <param name="_Color">���� ��</param>
	void TextCopy(const std::string& _Text, const std::string& _Font, float _Size, const FTransform& _Trans, Color8Bit _Color);

	/// <summary>
	/// �ƿ� ������ �����ϴ� Text.
	/// </summary>
	/// <param name="_Text"></param>
	/// <param name="_Font"></param>
	/// <param name="_Size"></param>
	/// <param name="_Trans"></param>
	/// <param name="_OutLineColor"></param>
	/// <param name="_FillColor"></param>
	void TextCopy(const std::string& _Text, const std::string& _Font, float _Size, const FTransform& _Trans, Color8Bit _OutLineColor, Color8Bit _FillColor);

	/// <summary>
	/// Text ����.
	/// </summary>
	/// <param name="_Text"></param>
	/// <param name="_Font"></param>
	/// <param name="_Size"></param>
	/// <param name="_Trans"></param>
	/// <param name="_Color"></param>
	void TextCopyBold(const std::string& _Text, const std::string& _Font, float _Size, const FTransform& _Trans, Color8Bit _Color /*= Color8Bit::Black*/);

	/// <summary>
	/// ����� ���.
	/// </summary>
	/// <param name="_Text"></param>
	/// <param name="_Font"></param>
	/// <param name="stringFormat"></param>
	/// <param name="_Size"></param>
	/// <param name="_Trans"></param>
	/// <param name="_Color"></param>
	void TextCopyFormat(const std::string& _Text, const std::string& _Font, const Gdiplus::StringFormat& stringFormat, float _Size, const FTransform& _Trans, Color8Bit _Color /*= Color8Bit::Black*/);

	/// <summary>
	/// ��� ����.
	/// </summary>
	/// <param name="_Text"></param>
	/// <param name="_Pos"></param>
	void TextPrint(std::string_view _Text, FVector _Pos);

	const UImageInfo& ImageInfo(int _Index)
	{
		return Infos[_Index];
	}

	/// <summary>
	/// BackBufferImage ����� �Լ�.
	/// </summary>
	/// <param name="_Image"></param>
	/// <param name="_Scale"></param>
	/// <returns></returns>
	bool Create(UWindowImage* _Image, const FVector& _Scale);

	/// <summary>
	/// �̹����� �߶��ִ� �Լ�.
	/// </summary>
	/// <param name="_X"></param>
	/// <param name="_Y"></param>
	void Cutting(int _X, int _Y);


	void SetCuttingTransform(const FTransform& _CuttingTrans, int _Index = 0);

	/// <summary>
	/// Collision ����� ��.
	/// </summary>
	/// <param name="_Trans"></param>
	void DrawRectangle(const FTransform& _Trans);
	/// <summary>
	/// Collision ����� ��.
	/// </summary>
	/// <param name="_Trans"></param>
	void DrawEllipse(const FTransform& _Trans);



	/// <summary>
	/// �ش� ��ǥ�� ���� �����´�.
	/// </summary>
	/// <param name="_X"></param>
	/// <param name="_Y"></param>
	/// <param name="_DefaultColor"></param>
	/// <returns></returns>
	Color8Bit GetColor(int _X, int _Y, Color8Bit _DefaultColor);

	/// <summary>
	/// <para>�ܺο��� ����� �Լ�.</para>
	/// <para>UWindowImage* Rot = UEngineResourcesManager::GetInst().FindImg("image")</para>
	/// <para>UWindowImage* Mask = UEngineResourcesManager::GetInst().FindImg("image")</para>
	/// <para>Rot->SetRotationMaskImage(0, Mask, 0)</para>
	/// </summary>
	/// <param name="_Index"></param>
	/// <param name="_RotationMaskImage"></param>
	/// <param name="_MaskIndex"></param>
	void SetRotationMaskImage(int _Index, UWindowImage* _RotationMaskImage, int _MaskIndex)
	{
		UImageInfo& Ref = _RotationMaskImage->Infos[_MaskIndex];
		Infos[_Index].RotationMaskImage = &Ref;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="_RotationMaskImage"></param>
	void SetRotationMaskImageFolder(UWindowImage* _RotationMaskImage)
	{
		if (Infos.size() != _RotationMaskImage->Infos.size())
		{
			MsgBoxAssert("�̹��������� ũ�Ⱑ �ٸ� �̹��� ���� ��Ī�� �Ҽ��� �����ϴ�.");
			return;
		}

		for (int i = 0; i < static_cast<int>(Infos.size()); i++)
		{
			SetRotationMaskImage(i, _RotationMaskImage, i);
		}
	}

private :
	// �о�� �̹����� ��� Vector
	std::vector<UImageInfo> Infos;

	// ��Ʈ��
	HBITMAP hBitMap = 0;
	// DC
	HDC ImageDC = 0;
	// ��Ʈ���� ��� ����ü
	BITMAP BitMapInfo = BITMAP();

	//	�̹��� Type
	EWindowImageType ImageType = EWindowImageType::IMG_NONE;

	// DC�� ����� �Լ�
	bool Create(HDC _MainDC);

	// �о�� ���, �⺻�� Cutting
	EImageLoadType LoadType = EImageLoadType::IMG_Cutting;

	// ȸ���� ���� ������ ������ �̹���.
	UWindowImage* RotationMaskImage = nullptr;
};


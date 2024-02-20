#pragma once

#include <Windows.h>
#include <EngineBase/PathObject.h>
#include <EngineBase/Transform.h>

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
	/// 
	/// </summary>
	/// <param name="_X"></param>
	/// <param name="_Y"></param>
	/// <param name="_DefaultColor"></param>
	/// <returns></returns>
	Color8Bit GetColor(int _X, int _Y, Color8Bit _DefaultColor);

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
};


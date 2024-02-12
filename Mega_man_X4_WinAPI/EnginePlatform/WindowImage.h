#pragma once

#include <Windows.h>
#include <EngineBase/PathObject.h>
#include <EngineBase/Transform.h>

enum class EWindowImageType
{
	IMG_NONE,
	IMG_BMP,
	IMG_PNG
};

class ImageInfo
{
public:
	HBITMAP hBitMap = nullptr;
	HDC ImageDC = nullptr;
	FTransform CuttingTrans;
	EWindowImageType ImageType = EWindowImageType::IMG_NONE;
};

class UEngineWindow;
/// <summary>
/// DC 책임자.
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
	/// 사용한 bmp 파일의 특정 색을 지운다.
	/// </summary>
	/// <param name="_CopyImage"></param>
	/// <param name="_Trans">읽은 이미지</param>
	/// <param name="_Index"></param>
	/// <param name="_Color">기본 Black</param>
	void TransCopy(UWindowImage* _CopyImage, const FTransform& _Trans, int _Index, Color8Bit _Color = Color8Bit::Black);

	/// <summary>
	/// 사용한 png 파일의 alpha 값을 지운다.
	/// </summary>
	/// <param name="_CopyImage"></param>
	/// <param name="_Trans"></param>
	/// <param name="_Index"></param>
	/// <param name="_Color"></param>
	void AlphaCopy(UWindowImage* _CopyImage, const FTransform& _Trans, int _Index, Color8Bit _Color = Color8Bit::Black);

private :
	// 읽어온 이미지를 담는 Vector
	std::vector<ImageInfo> Infos;

	// 비트맵
	HBITMAP hBitMap = 0;
	// DC
	HDC ImageDC = 0;
	// 비트맵을 담는 구조체
	BITMAP BitMapInfo = BITMAP();

	//	이미지 Type
	EWindowImageType ImageType = EWindowImageType::IMG_NONE;

	// DC를 만드는 함수
	bool Create(HDC _MainDC);
};


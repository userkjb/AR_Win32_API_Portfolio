#pragma once

#include <Windows.h>
#include <EngineBase/PathObject.h>
#include <EngineBase/Transform.h>

/// <summary>
/// 이미지를 읽어온 Type.
/// 데이터 정리할 때 사용.
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
	/// UWindowImage 이미지를 FTransform 이 위치와 크기로 나에게 카피.
	/// 이미지를 이미지 크기대로 만 그릴 수 있다.
	/// </summary>
	/// <param name="_CopyImage"></param>
	/// <param name="_Trans"></param>
	void BitCopy(UWindowImage* _CopyImage, const FTransform& _Trans);

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

	/// <summary>
	/// BackBufferImage 만드는 함수.
	/// </summary>
	/// <param name="_Image"></param>
	/// <param name="_Scale"></param>
	/// <returns></returns>
	bool Create(UWindowImage* _Image, const FVector& _Scale);

	/// <summary>
	/// 이미지를 잘라주는 함수.
	/// </summary>
	/// <param name="_X"></param>
	/// <param name="_Y"></param>
	void Cutting(int _X, int _Y);

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

	// 읽어온 방식, 기본은 Cutting
	EImageLoadType LoadType = EImageLoadType::IMG_Cutting;
};


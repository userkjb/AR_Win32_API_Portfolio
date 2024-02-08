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

	bool Load(UWindowImage* _Image);

private :
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


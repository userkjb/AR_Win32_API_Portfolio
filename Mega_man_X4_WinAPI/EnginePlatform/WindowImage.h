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
	HBITMAP hBitMap;
	HDC ImageDC = nullptr;
	FTransform CuttingTrans;
};

/// <summary>
/// DC å����.
/// </summary>
class UWindowImage : public UPathObject
{
public:
	// constrcuter destructer
	UWindowImage();
	~UWindowImage();

	// delete Function
	UWindowImage(const UWindowImage& _Other) = delete;
	UWindowImage(UWindowImage&& _Other) noexcept = delete;
	UWindowImage& operator=(const UWindowImage& _Other) = delete;
	UWindowImage& operator=(UWindowImage&& _Other) noexcept = delete;

private :
	std::vector<ImageInfo> Infos;
};


#pragma once


enum class EWindowImageType
{
	IMG_NONE,
	IMG_BMP,
	IMG_PNG
};

/// <summary>
/// DC √•¿”¿⁄.
/// </summary>
class UWindowImage
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
};


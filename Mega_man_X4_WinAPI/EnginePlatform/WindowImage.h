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

	bool Load(UWindowImage* _Image);

private :
	std::vector<ImageInfo> Infos;

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
};


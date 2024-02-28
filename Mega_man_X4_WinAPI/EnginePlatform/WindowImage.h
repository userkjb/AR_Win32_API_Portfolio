#pragma once

#include <Windows.h>
#include <EngineBase/PathObject.h>
#include <EngineBase/Transform.h>
#include <EngineBase/EngineDebug.h>

#include <objidl.h>
#include <gdiplus.h>

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
	UImageInfo* RotationMaskImage = nullptr;
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
	/// 한 장의 이미지가 아닌 여러장의 이미지들을 하나의 폴더에서 관리하는 경우 사용.
	/// </summary>
	/// <param name="_Image"></param>
	/// <returns></returns>
	bool LoadFolder(UWindowImage* _Image);

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
	/// 회전을 위한 Copy
	/// </summary>
	/// <param name="_CopyImage"></param>
	/// <param name="_Trans"></param>
	/// <param name="_Index"></param>
	/// <param name="_RadAngle"></param>
	void PlgCopy(UWindowImage* _CopyImage, const FTransform& _Trans, int _Index, float _RadAngle);

	/// <summary>
	/// 텍스트 출력을 위한 데이터 Copy
	/// </summary>
	/// <param name="_Text">출력하고자 하는 string</param>
	/// <param name="_Font">Text 폰트</param>
	/// <param name="_Size">글자 Size</param>
	/// <param name="_Trans">글자 출력 위치</param>
	/// <param name="_Color">글자 색</param>
	void TextCopy(const std::string& _Text, const std::string& _Font, float _Size, const FTransform& _Trans, Color8Bit _Color);

	/// <summary>
	/// 아웃 라인을 설정하는 Text.
	/// </summary>
	/// <param name="_Text"></param>
	/// <param name="_Font"></param>
	/// <param name="_Size"></param>
	/// <param name="_Trans"></param>
	/// <param name="_OutLineColor"></param>
	/// <param name="_FillColor"></param>
	void TextCopy(const std::string& _Text, const std::string& _Font, float _Size, const FTransform& _Trans, Color8Bit _OutLineColor, Color8Bit _FillColor);

	/// <summary>
	/// Text 강조.
	/// </summary>
	/// <param name="_Text"></param>
	/// <param name="_Font"></param>
	/// <param name="_Size"></param>
	/// <param name="_Trans"></param>
	/// <param name="_Color"></param>
	void TextCopyBold(const std::string& _Text, const std::string& _Font, float _Size, const FTransform& _Trans, Color8Bit _Color /*= Color8Bit::Black*/);

	/// <summary>
	/// 출력을 담당.
	/// </summary>
	/// <param name="_Text"></param>
	/// <param name="_Font"></param>
	/// <param name="stringFormat"></param>
	/// <param name="_Size"></param>
	/// <param name="_Trans"></param>
	/// <param name="_Color"></param>
	void TextCopyFormat(const std::string& _Text, const std::string& _Font, const Gdiplus::StringFormat& stringFormat, float _Size, const FTransform& _Trans, Color8Bit _Color /*= Color8Bit::Black*/);

	/// <summary>
	/// 사용 안함.
	/// </summary>
	/// <param name="_Text"></param>
	/// <param name="_Pos"></param>
	void TextPrint(std::string_view _Text, FVector _Pos);

	const UImageInfo& ImageInfo(int _Index)
	{
		return Infos[_Index];
	}

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


	void SetCuttingTransform(const FTransform& _CuttingTrans, int _Index = 0);

	/// <summary>
	/// Collision 디버깅 용.
	/// </summary>
	/// <param name="_Trans"></param>
	void DrawRectangle(const FTransform& _Trans);
	/// <summary>
	/// Collision 디버깅 용.
	/// </summary>
	/// <param name="_Trans"></param>
	void DrawEllipse(const FTransform& _Trans);



	/// <summary>
	/// 해당 좌표의 색을 가져온다.
	/// </summary>
	/// <param name="_X"></param>
	/// <param name="_Y"></param>
	/// <param name="_DefaultColor"></param>
	/// <returns></returns>
	Color8Bit GetColor(int _X, int _Y, Color8Bit _DefaultColor);

	/// <summary>
	/// <para>외부에서 사용할 함수.</para>
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
			MsgBoxAssert("이미지정보의 크기가 다른 이미지 끼리 매칭을 할수가 없습니다.");
			return;
		}

		for (int i = 0; i < static_cast<int>(Infos.size()); i++)
		{
			SetRotationMaskImage(i, _RotationMaskImage, i);
		}
	}

private :
	// 읽어온 이미지를 담는 Vector
	std::vector<UImageInfo> Infos;

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

	// 회전을 위한 검정색 바탕의 이미지.
	UWindowImage* RotationMaskImage = nullptr;
};


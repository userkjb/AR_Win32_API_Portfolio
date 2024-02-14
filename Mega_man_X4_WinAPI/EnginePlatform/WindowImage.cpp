#include "WindowImage.h"
#include <EngineBase/EngineString.h>
#include <EngineBase/EngineDebug.h>

// �̹��� ó��.
#pragma comment(lib, "Msimg32.lib")
#include <objidl.h>

// png ���� ó��.
#pragma comment(lib, "Gdiplus.lib")
#include <gdiplus.h>

UWindowImage::UWindowImage()
{
}

UWindowImage::~UWindowImage()
{
    DeleteObject(hBitMap);
    DeleteDC(ImageDC);
}

FVector UWindowImage::GetScale()
{
    return FVector(BitMapInfo.bmWidth, BitMapInfo.bmHeight);
}

/// <summary>
/// UEngineResourcesManager LoadImg ���� ȣ��.
/// </summary>
/// <param name="_Image"></param>
/// <returns></returns>
bool UWindowImage::Load(UWindowImage* _Image)
{
    LoadType = EImageLoadType::IMG_Cutting;

    // �̹��� ���� ���
    UEnginePath Path = GetEnginePath();
    
    // ������ ���Ͽ� ���� Ȯ����
    std::string UpperExt = UEngineString::ToUpper(Path.GetExtension());

    // Ȯ���ڰ� BMP �̸�,
    if (".BMP" == UpperExt)
    {
        // ��Ʈ�� ���� �ڵ�(�׸��� �ڵ��� �ƴ�. �ε��ϴ� �ڵ�.)
        HANDLE ImageHandle = LoadImageA(nullptr, Path.GetFullPath().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        hBitMap = reinterpret_cast<HBITMAP>(ImageHandle);
        ImageType = EWindowImageType::IMG_BMP;
    }
    else if(".PNG" == UpperExt)
    {
        ULONG_PTR gdiplusToken = 0;

        Gdiplus::GdiplusStartupInput gdistartupinput;
        Gdiplus::GdiplusStartup(&gdiplusToken, &gdistartupinput, nullptr);

        std::wstring wPath = UEngineString::AnsiToUniCode(Path.GetFullPath());

        // ���ڿ� ������ Ȯ�� �ڵ�
        Gdiplus::Image* pImage = Gdiplus::Image::FromFile(wPath.c_str());
        Gdiplus::Bitmap* pBitMap = reinterpret_cast<Gdiplus::Bitmap*>(pImage->Clone());
        Gdiplus::Status stat = pBitMap->GetHBITMAP(Gdiplus::Color(0, 0, 0, 0), &hBitMap);

        if (Gdiplus::Status::Ok != stat)
        {
            MsgBoxAssert("Png ���� ���ҽ� �ε忡 �����Ͽ����ϴ�.");
        }

        ImageType = EWindowImageType::IMG_PNG;
    }

    // DC ����.
    
    // ImageDC�� ����� ���ο��� 1,1ũ���� HBITMAP�� �����.
    ImageDC = CreateCompatibleDC(_Image->ImageDC);

    if (nullptr == ImageDC)
    {
        MsgBoxAssert("�̹��� ������ �����߽��ϴ�.");
        return false;
    }

    
    // DC, �⺻ BITMAP, ������ BITMAP �� �ִ� ��Ȳ.
    // DC�� ������ �ִ� �⺻ BITMAP�� ���� ����� �� BITMAP�� �־���� �ϴ�,
    // reinterpret_cast<HBITMAP>(SelectObject(ImageDC, hBitMap)) �ڵ带 ����ؼ� �ٲ��ش�.
    // �� �ڵ��� ���� ������ �޴� ���� ������ BITMAP �̴�.
    // ������ �⺻ HBITMAP �� ���� ������ �����
    // �� ������ ���� ���� �޵��� �ڵ带 ����� �ָ� ������ ����.
    HBITMAP OldBitMap = reinterpret_cast<HBITMAP>(SelectObject(ImageDC, hBitMap));
    // ���� ���� BITMAP�� �ʿ� ������ �����ش�.
    DeleteObject(OldBitMap);
    
    // ���� DC�� ����� ��Ʈ�� �ڵ� ���� ���;� �Ѵ�. -> BITMAP �� ���� ����.(������ ����)
    // hBitMap�� �ִ� ������ sizeof(BITMAP) ��ŭ �����ͼ� &BitMapInfo�� �־��.
    GetObject(hBitMap, sizeof(BITMAP), &BitMapInfo);

    UImageInfo Info;
    Info.hBitMap = hBitMap;
    Info.ImageDC = ImageDC;
    Info.CuttingTrans.SetPosition({ 0,0 });
    Info.CuttingTrans.SetScale(GetScale());
    Info.ImageType = ImageType;
    Infos.push_back(Info);

    return false;
}

void UWindowImage::BitCopy(UWindowImage* _CopyImage, const FTransform& _Trans)
{
    // ��� �ڵ�
    if (nullptr == _CopyImage)
    {
        MsgBoxAssert("nullptr �� �̹����� ������ �� �����ϴ�");
    }

    // �̹��� �׸���

    // ������
    HDC hdc = ImageDC;

    // �̹���
    HDC hdcSrc = _CopyImage->ImageDC;

    // ����
    BitBlt(
        hdc,
        _Trans.iLeft(),
        _Trans.iTop(),
        _Trans.GetScale().iX(),
        _Trans.GetScale().iY(),
        hdcSrc,
        0,
        0,
        SRCCOPY // �̹��� �״�� ��� ���縦 �ض�.
    );
}

void UWindowImage::TransCopy(UWindowImage* _CopyImage, const FTransform& _Trans, int _Index, Color8Bit _Color)
{
    // ��� �ڵ�
    if (nullptr == _CopyImage)
    {
        MsgBoxAssert("nullptr �� �̹����� ������ �� �����ϴ�.");
    }
    
    // ��� �ڵ�
	if (_Index >= _CopyImage->Infos.size())
    {
        MsgBoxAssert(GetName() + "�̹��� ������ �ε����� �����Ͽ� ����߽��ϴ�");
    }

    // ���� �̹��� ���� ��������. -> ��ǥ ��������.
    FTransform& ImageTrans = _CopyImage->Infos[_Index].CuttingTrans;

    // ���� ���� �ʿ�.[TODO]
    // ���� ��ǥ.
    int RenderLeft = _Trans.iLeft();
    int RenderTop = _Trans.iTop();
    int RenderScaleX = _Trans.GetScale().iX();
    int RenderScaleY = _Trans.GetScale().iY();

    // �̹��� ��ǥ.
    int ImageLeft = ImageTrans.GetPosition().iX();
    int ImageTop = ImageTrans.GetPosition().iY();
    int ImageScaleX = ImageTrans.GetScale().iX();
    int ImageScaleY = ImageTrans.GetScale().iY();

    // �׷��� Main DC
    HDC hdc = ImageDC;

    // �̹��� DC ����
    HDC hdcSrc = _CopyImage->Infos[_Index].ImageDC;
    
    // ����.
    TransparentBlt(
        hdc,
        RenderLeft,
        RenderTop,
        RenderScaleX,
        RenderScaleY,
        hdcSrc,
        ImageLeft,
        ImageTop,
        ImageScaleX,
        ImageScaleY,
        _Color.Color
    );
}

void UWindowImage::AlphaCopy(UWindowImage* _CopyImage, const FTransform& _Trans, int _Index, Color8Bit _Color)
{
    // ��� �ڵ�
    if (nullptr == _CopyImage)
    {
        MsgBoxAssert("nullptr �� �̹����� ������ �� �����ϴ�.");
    }

    // ��� �ڵ�
    if (_Index >= _CopyImage->Infos.size())
    {
        MsgBoxAssert(GetName() + "�̹��� ������ �ε����� �����Ͽ� ����߽��ϴ�");
    }

    // ���� �̹��� ���� ��������. -> ��ǥ ��������.
    FTransform& ImageTrans = _CopyImage->Infos[_Index].CuttingTrans;

    // ���� ���� �ʿ�.[TODO]
    // ���� ��ǥ.
    int RenderLeft = _Trans.iLeft();
    int RenderTop = _Trans.iTop();
    int RenderScaleX = _Trans.GetScale().iX();
    int RenderScaleY = _Trans.GetScale().iY();

    // �̹��� ��ǥ.
    int ImageLeft = ImageTrans.GetPosition().iX();
    int ImageTop = ImageTrans.GetPosition().iY();
    int ImageScaleX = ImageTrans.GetScale().iX();
    int ImageScaleY = ImageTrans.GetScale().iY();

    // �׷��� Main DC
    HDC hdc = ImageDC;

    // �̹��� DC ����
    HDC hdcSrc = _CopyImage->Infos[_Index].ImageDC;

    // Alpha �� �����.
    BLENDFUNCTION Function;
    Function.BlendOp = AC_SRC_OVER;
    Function.BlendFlags = 0; // 0~255
    Function.SourceConstantAlpha = _Color.A;
    Function.AlphaFormat = AC_SRC_ALPHA;

    // ����.
    AlphaBlend(
        hdc,
        RenderLeft,
        RenderTop,
        RenderScaleX,
        RenderScaleY,
        hdcSrc,
        ImageLeft,
        ImageTop,
        ImageScaleX,
        ImageScaleY,
        Function
    );
}

bool UWindowImage::Create(HDC _MainDC)
{
    ImageDC = _MainDC;

    if (nullptr == ImageDC)
    {
        return false;
    }
    return true;
}

bool UWindowImage::Create(UWindowImage* _Image, const FVector& _Scale)
{
    // HBITMAP ��Ʈ�� �̹����� �޸𸮱���
    HANDLE ImageHandle = CreateCompatibleBitmap(_Image->ImageDC, _Scale.iX(), _Scale.iY());

    if (nullptr == ImageHandle)
    {
        MsgBoxAssert("�̹��� ������ �����߽��ϴ�");
        return false;
    }

    // �ڵ� ��������
    hBitMap = reinterpret_cast<HBITMAP>(ImageHandle);

    // ���� �̹����� DC ��������
    ImageDC = CreateCompatibleDC(_Image->ImageDC);
    
    if (nullptr == ImageDC)
    {
        MsgBoxAssert("�̹��� ������ �����߽��ϴ�");
        return false;
    }

    HBITMAP OldBitMap = reinterpret_cast<HBITMAP>(SelectObject(ImageDC, hBitMap));
    DeleteObject(OldBitMap);

    // hBitMap���� �����ڴ�.
    GetObject(hBitMap, sizeof(BITMAP), &BitMapInfo);

    return true;
}

void UWindowImage::Cutting(int _X, int _Y)
{
    // �� �� Ŭ���� ���ش�.
    Infos.clear();

    // �̹����� �ش� ũ�� ��ŭ �ڸ� ���̴�.
    FVector CuttingScale = { GetScale().X / _X, GetScale().Y / _Y };
    // ���� ������ ����.
    FVector CuttingPos = { 0, 0 };

    for (int i = 0; i < _Y; i++)
    {
        for (int k = 0; k < _X; k++)
        {
            UImageInfo Info;
            Info.ImageDC = ImageDC;
            Info.CuttingTrans.SetPosition(CuttingPos);
            Info.CuttingTrans.SetScale(CuttingScale);
            Infos.push_back(Info);

            CuttingPos.X += CuttingScale.X;
        }

        CuttingPos.X = 0.0f;
        CuttingPos.Y += CuttingScale.Y;
    }
}

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

    ImageInfo Info;
    Info.hBitMap = hBitMap;
    Info.ImageDC = ImageDC;
    Info.CuttingTrans.SetPosition({ 0,0 });
    Info.CuttingTrans.SetScale(GetScale());
    Info.ImageType = ImageType;
    Infos.push_back(Info);

    return false;
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
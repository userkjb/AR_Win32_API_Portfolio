#include "WindowImage.h"
#include <EngineBase/EngineString.h>
#include <EngineBase/EngineDebug.h>
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>

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

bool UWindowImage::LoadFolder(UWindowImage* _Image)
{
    LoadType = EImageLoadType::IMG_Folder;

    UEnginePath EnginePath = GetEnginePath();

    if (false == EnginePath.IsDirectory())
    {
        MsgBoxAssert("���丮�� �ƴ� ��η� �����ε带 �Ϸ��� �߽��ϴ�");
    }

    UEngineDirectory Dir = EnginePath;

    std::list<UEngineFile> NewList = Dir.AllFile({ ".png", ".bmp" }, false);
    Infos.reserve(NewList.size());

    for (UEngineFile& File : NewList)
    {
        UEnginePath Path = File;

        std::string UpperExt = UEngineString::ToUpper(Path.GetExtension());

        if (".BMP" == UpperExt)
        {
            HANDLE ImageHandle = LoadImageA(nullptr, Path.GetFullPath().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            hBitMap = reinterpret_cast<HBITMAP>(ImageHandle);
            ImageType = EWindowImageType::IMG_BMP;
        }
        else if (".PNG" == UpperExt)
        {
            ULONG_PTR gdiplusToken = 0;
            Gdiplus::GdiplusStartupInput gdistartupinput;
            Gdiplus::GdiplusStartup(&gdiplusToken, &gdistartupinput, nullptr);
            std::wstring wPath = UEngineString::AnsiToUniCode(Path.GetFullPath());
            Gdiplus::Image* pImage = Gdiplus::Image::FromFile(wPath.c_str());
            Gdiplus::Bitmap* pBitMap = reinterpret_cast<Gdiplus::Bitmap*>(pImage->Clone());
            Gdiplus::Status stat = pBitMap->GetHBITMAP(Gdiplus::Color(0, 0, 0, 0), &hBitMap);
            if (Gdiplus::Status::Ok != stat)
            {
                MsgBoxAssert("Png ���� ���ҽ� �ε忡 �����߽��ϴ�.");
            }

            ImageType = EWindowImageType::IMG_PNG;
        }
        ImageDC = CreateCompatibleDC(_Image->ImageDC);

        if (nullptr == ImageDC)
        {
            MsgBoxAssert("�̹��� ������ �����߽��ϴ�");
            return false;
        }

        HBITMAP OldBitMap = reinterpret_cast<HBITMAP>(SelectObject(ImageDC, hBitMap));
        DeleteObject(OldBitMap);
        GetObject(hBitMap, sizeof(BITMAP), &BitMapInfo);

        UImageInfo Info;
        Info.hBitMap = hBitMap;
        Info.ImageDC = ImageDC;
        Info.CuttingTrans.SetPosition({ 0,0 });
        Info.CuttingTrans.SetScale(GetScale());
        Infos.push_back(Info);
    }
    return true;
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
        MsgBoxAssert(GetName() + "�̹��� ������ �ε����� �����Ͽ� ����߽��ϴ�. Bit");
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
    // ����� ��ҿ� ������ ���
    // �̹����� ������ �ڸ����� ������ �� ������ ���� �ִ´�.
    TransparentBlt(
        hdc,          // �̹����� ����� �ڵ�
        RenderLeft,   // ����� X ��ǥ
        RenderTop,    // ����� Y ��ǥ
        RenderScaleX, // ������ ���α���(���ڸ� �����ϸ� ����.)
        RenderScaleY, // ������ ���α���.
        hdcSrc,       // ���� �̹���
        ImageLeft,    // ���ϴ� �̹��� ������ X
        ImageTop,     // ���ϴ� �̹��� ������ Y
        ImageScaleX,  // ���ϴ� �̹��� �ڸ� ��ŭ�� ���� ����.
        ImageScaleY,  // ���ϴ� �̹��� �ڸ� ��ŭ�� ���� ����.
        _Color.Color  // Ư�� ���� �������� ó��.
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
        MsgBoxAssert(GetName() + "�̹��� ������ �ε����� �����Ͽ� ����߽��ϴ�. Alpha");
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

void UWindowImage::PlgCopy(UWindowImage* _CopyImage, const FTransform& _Trans, int _Index, float _RadAngle)
{
    if (nullptr == _CopyImage)
    {
        MsgBoxAssert("nullptr �� �̹����� ������ �� �����ϴ�");
    }

    if (_Index >= _CopyImage->Infos.size())
    {
        MsgBoxAssert(GetName() + "�̹��� ������ �ε����� �����Ͽ� ����߽��ϴ�");
    }

    UImageInfo& CurInfo = _CopyImage->Infos[_Index];

    FTransform& ImageTrans = _CopyImage->Infos[_Index].CuttingTrans;

    POINT Arr[3];
    {
        FTransform Trans = FTransform(float4::Zero, _Trans.GetScale());

        FVector LeftTop = Trans.LeftTop();
        FVector RightTop = Trans.RightTop();
        FVector LeftBot = Trans.LeftBottom();

        LeftTop.RotationZToRad(_RadAngle);
        RightTop.RotationZToRad(_RadAngle);
        LeftBot.RotationZToRad(_RadAngle);

        LeftTop += _Trans.GetPosition();
        RightTop += _Trans.GetPosition();
        LeftBot += _Trans.GetPosition();

        Arr[0] = LeftTop.ConvertToWinApiPOINT();
        Arr[1] = RightTop.ConvertToWinApiPOINT();
        Arr[2] = LeftBot.ConvertToWinApiPOINT();
    }

    int ImageLeft = ImageTrans.GetPosition().iX();
    int ImageTop = ImageTrans.GetPosition().iY();
    int ImageScaleX = ImageTrans.GetScale().iX();
    int ImageScaleY = ImageTrans.GetScale().iY();

    // ���ϴ� ������ŭ ȸ�� ��Ų ���� ������ �Ѵ�.

    if (nullptr == CurInfo.RotationMaskImage)
    {
        MsgBoxAssert("�̹����� ȸ����Ű���� �ߴµ� �̹����� �����ϴ�.");
    }

    // ���� �׸��� �ȴ�.
    HDC hdc = ImageDC;
    //// �̹���
    HDC hdcSrc = _CopyImage->Infos[_Index].ImageDC;

    PlgBlt(
        hdc, 							  // HDC hdc, // 
        Arr,
        hdcSrc,							// HDC hdcSrc, 
        ImageLeft,   							// int y1, 
        ImageTop,   							// int x1,  
        ImageScaleX, 							// int y1, 
        ImageScaleY, 							// int y1, 
        CurInfo.RotationMaskImage->hBitMap, // ������ ������ �׸�.
        ImageLeft,   							// int y1, 
        ImageTop   							// int x1,  
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

void UWindowImage::TextCopy(const std::string& _Text, const std::string& _Font, float _Size, const FTransform& _Trans, Color8Bit _Color)
{
    Gdiplus::StringFormat stringFormat;
    stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
    stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);

    TextCopyFormat(_Text, _Font, stringFormat, _Size, _Trans, _Color); //���
}

void UWindowImage::TextCopy(const std::string& _Text, const std::string& _Font, float _Size, const FTransform& _Trans, Color8Bit _OutLineColor, Color8Bit _FillColor)
{
    Gdiplus::Graphics graphics(ImageDC);
    std::wstring WFont = UEngineString::AnsiToUniCode(_Font);
    Gdiplus::Font fnt(WFont.c_str(), _Size, Gdiplus::FontStyleBold | Gdiplus::FontStyleItalic, Gdiplus::UnitPixel);

    // �׵θ��� �귯�� ����
    Gdiplus::SolidBrush OutLineBrush(Gdiplus::Color(_OutLineColor.R, _OutLineColor.G, _OutLineColor.B));

    // ���� ä���� �귯�� ����
    Gdiplus::SolidBrush fillBrush(Gdiplus::Color(_FillColor.R, _FillColor.G, _FillColor.B));

    FVector Pos = _Trans.GetPosition();
    Gdiplus::RectF rectF(Pos.X, Pos.Y, 0, 0);

    Gdiplus::StringFormat stringFormat;
    stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
    stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);
    std::wstring WText = UEngineString::AnsiToUniCode(_Text);

    // �׵θ� ȿ���� ���� �ؽ�Ʈ�� ���� �������� �׸��ϴ�.
    float offsetsX[] = { -3.f, 3.f }; // �׵θ��� �β��� �����Ϸ��� �� ���� ����.
    float offsetsY[] = { -2.f, 2.f }; // �׵θ��� �β��� �����Ϸ��� �� ���� ����.
    for (float dx : offsetsX)
    {
        for (float dy : offsetsY)
        {
            Gdiplus::RectF borderRect = rectF;
            borderRect.X += dx;
            borderRect.Y += dy;
            graphics.DrawString(WText.c_str(), -1, &fnt, borderRect, &stringFormat, &OutLineBrush);
        }
    }
    float offsets_X[] = { -2.f, 2.f }; // ������ �β��� �����Ϸ��� �� ���� ����.
    float offsets_Y[] = { -1.f, 1.f }; // ������ �β��� �����Ϸ��� �� ���� ����.
    for (float dx : offsets_X)
    {
        for (float dy : offsets_Y)
        {
            Gdiplus::RectF borderRect = rectF;
            borderRect.X += dx;
            borderRect.Y += dy;
            graphics.DrawString(WText.c_str(), -1, &fnt, borderRect, &stringFormat, &fillBrush);
        }
    }
}

void UWindowImage::TextCopyBold(const std::string& _Text, const std::string& _Font, float _Size, const FTransform& _Trans, Color8Bit _Color)
{
    Gdiplus::StringFormat stringFormat;
    stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
    stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);

    Gdiplus::Graphics graphics(ImageDC);
    std::wstring WFont = UEngineString::AnsiToUniCode(_Font);
    Gdiplus::Font fnt(WFont.c_str(), _Size, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
    Gdiplus::SolidBrush hB(Gdiplus::Color(_Color.R, _Color.G, _Color.B));
    FVector Pos = _Trans.GetPosition();
    Gdiplus::RectF  rectF(_Trans.GetPosition().X, _Trans.GetPosition().Y, 0, 0);

    std::wstring WText = UEngineString::AnsiToUniCode(_Text);
    graphics.DrawString(WText.c_str(), -1, &fnt, rectF, &stringFormat, &hB);  //���
}

void UWindowImage::TextCopyFormat(const std::string& _Text, const std::string& _Font, const Gdiplus::StringFormat& stringFormat, float _Size, const FTransform& _Trans, Color8Bit _Color)
{
    // GDI+ ��� ���̺귯���� ����ؾ� �Ѵ�.
    Gdiplus::Graphics graphics(ImageDC);

    std::wstring WFont = UEngineString::AnsiToUniCode(_Font);
    Gdiplus::Font fnt(WFont.c_str(), _Size, 0, Gdiplus::UnitPixel);

    Gdiplus::SolidBrush hB(Gdiplus::Color(_Color.R, _Color.G, _Color.B));

    //FVector Pos = _Trans.GetPosition();
    //Gdiplus::PointF ptf(Pos.X, Pos.Y);
    Gdiplus::RectF rectF(_Trans.GetPosition().X, _Trans.GetPosition().Y, 0, 0);

    std::wstring WText = UEngineString::AnsiToUniCode(_Text);

    //graphics.DrawString(WText.c_str(), -1, &fnt, ptf, &hB);  //���
    graphics.DrawString(WText.c_str(), -1, &fnt, rectF, &stringFormat, &hB);  //���
}

void UWindowImage::TextPrint(std::string_view _Text, FVector _Pos)
{
    TextOutA(ImageDC, _Pos.iX(), _Pos.iY(), _Text.data(), static_cast<int>(_Text.size()));
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

void UWindowImage::SetCuttingTransform(const FTransform& _CuttingTrans, int _Index)
{
    Infos[_Index].CuttingTrans = _CuttingTrans;
}

void UWindowImage::DrawRectangle(const FTransform& _Trans)
{
    Rectangle(ImageDC, _Trans.iLeft(), _Trans.iTop(), _Trans.iRight(), _Trans.iBottom());
}

void UWindowImage::DrawEllipse(const FTransform& _Trans)
{
    Ellipse(ImageDC, _Trans.iLeft(), _Trans.iTop(), _Trans.iRight(), _Trans.iBottom());

}

Color8Bit UWindowImage::GetColor(int _X, int _Y, Color8Bit _DefaultColor)
{
    if (0 > _X)
    {
        return _DefaultColor;
    }

    if (0 > _Y)
    {
        return _DefaultColor;
    }

    if (GetScale().iX() <= _X)
    {
        return _DefaultColor;
    }

    if (GetScale().iY() <= _Y)
    {
        return _DefaultColor;
    }

    Color8Bit Color;

    Color.Color = ::GetPixel(ImageDC, _X, _Y);

    return Color;
}

#include "WindowImage.h"
#include <EngineBase/EngineString.h>
#include <EngineBase/EngineDebug.h>
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineFile.h>

// 이미지 처리.
#pragma comment(lib, "Msimg32.lib")
#include <objidl.h>

// png 파일 처리.
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
/// UEngineResourcesManager LoadImg 에서 호출.
/// </summary>
/// <param name="_Image"></param>
/// <returns></returns>
bool UWindowImage::Load(UWindowImage* _Image)
{
    LoadType = EImageLoadType::IMG_Cutting;

    // 이미지 파일 경로
    UEnginePath Path = GetEnginePath();
    
    // 가져온 파일에 대한 확장자
    std::string UpperExt = UEngineString::ToUpper(Path.GetExtension());

    // 확장자가 BMP 이면,
    if (".BMP" == UpperExt)
    {
        // 비트맵 제어 핸들(그리는 핸들이 아님. 로드하는 핸들.)
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

        // 문자열 데이터 확인 코드
        Gdiplus::Image* pImage = Gdiplus::Image::FromFile(wPath.c_str());
        Gdiplus::Bitmap* pBitMap = reinterpret_cast<Gdiplus::Bitmap*>(pImage->Clone());
        Gdiplus::Status stat = pBitMap->GetHBITMAP(Gdiplus::Color(0, 0, 0, 0), &hBitMap);

        if (Gdiplus::Status::Ok != stat)
        {
            MsgBoxAssert("Png 파일 리소스 로드에 실패하였습니다.");
        }

        ImageType = EWindowImageType::IMG_PNG;
    }

    // DC 설정.
    
    // ImageDC를 만들면 내부에서 1,1크기의 HBITMAP을 만든다.
    ImageDC = CreateCompatibleDC(_Image->ImageDC);

    if (nullptr == ImageDC)
    {
        MsgBoxAssert("이미지 생성에 실패했습니다.");
        return false;
    }

    
    // DC, 기본 BITMAP, 만들어둔 BITMAP 이 있는 상황.
    // DC가 가지고 있는 기본 BITMAP에 위에 만들어 둔 BITMAP을 넣어줘야 하니,
    // reinterpret_cast<HBITMAP>(SelectObject(ImageDC, hBitMap)) 코드를 사용해서 바꿔준다.
    // 위 코드의 리턴 값으로 받는 것이 기존의 BITMAP 이다.
    // 때문에 기본 HBITMAP 을 받을 변수를 만들고
    // 이 변수에 리턴 값을 받도록 코드를 만들어 주면 다음과 같다.
    HBITMAP OldBitMap = reinterpret_cast<HBITMAP>(SelectObject(ImageDC, hBitMap));
    // 리턴 받은 BITMAP은 필요 없으니 지워준다.
    DeleteObject(OldBitMap);
    
    // 현재 DC와 연결된 비트맵 핸들 값을 얻어와야 한다. -> BITMAP 에 대한 정보.(사이즈 같은)
    // hBitMap에 있는 정보를 sizeof(BITMAP) 만큼 가져와서 &BitMapInfo에 넣어라.
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
        MsgBoxAssert("디렉토리가 아닌 경로로 폴더로드를 하려고 했습니다");
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
                MsgBoxAssert("Png 형식 리소스 로드에 실패했습니다.");
            }

            ImageType = EWindowImageType::IMG_PNG;
        }
        ImageDC = CreateCompatibleDC(_Image->ImageDC);

        if (nullptr == ImageDC)
        {
            MsgBoxAssert("이미지 생성에 실패했습니다");
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
    // 방어 코드
    if (nullptr == _CopyImage)
    {
        MsgBoxAssert("nullptr 인 이미지를 복사할 수 없습니다");
    }

    // 이미지 그리기

    // 윈도우
    HDC hdc = ImageDC;

    // 이미지
    HDC hdcSrc = _CopyImage->ImageDC;

    // 설정
    BitBlt(
        hdc,
        _Trans.iLeft(),
        _Trans.iTop(),
        _Trans.GetScale().iX(),
        _Trans.GetScale().iY(),
        hdcSrc,
        0,
        0,
        SRCCOPY // 이미지 그대로 고속 복사를 해라.
    );
}

void UWindowImage::TransCopy(UWindowImage* _CopyImage, const FTransform& _Trans, int _Index, Color8Bit _Color)
{
    // 방어 코드
    if (nullptr == _CopyImage)
    {
        MsgBoxAssert("nullptr 인 이미지를 복사할 수 없습니다.");
    }
    
    // 방어 코드
	if (_Index >= _CopyImage->Infos.size())
    {
        MsgBoxAssert(GetName() + "이미지 정보의 인덱스를 오버하여 사용했습니다. Bit");
    }

    // 받은 이미지 설정 가져오기. -> 좌표 가져오기.
    FTransform& ImageTrans = _CopyImage->Infos[_Index].CuttingTrans;

    // 설명 보충 필요.[TODO]
    // 기존 좌표.
    int RenderLeft = _Trans.iLeft();
    int RenderTop = _Trans.iTop();
    int RenderScaleX = _Trans.GetScale().iX();
    int RenderScaleY = _Trans.GetScale().iY();

    // 이미지 좌표.
    int ImageLeft = ImageTrans.GetPosition().iX();
    int ImageTop = ImageTrans.GetPosition().iY();
    int ImageScaleX = ImageTrans.GetScale().iX();
    int ImageScaleY = ImageTrans.GetScale().iY();

    // 그려줄 Main DC
    HDC hdc = ImageDC;

    // 이미지 DC 설정
    HDC hdcSrc = _CopyImage->Infos[_Index].ImageDC;
    
    // 설정.
    // 출력할 장소에 공간을 잡고
    // 이미지를 가져와 자른다음 마련해 둔 공간에 맞춰 넣는다.
    TransparentBlt(
        hdc,          // 이미지를 출력할 핸들
        RenderLeft,   // 출력할 X 좌표
        RenderTop,    // 출력할 Y 좌표
        RenderScaleX, // 공간의 가로길이(엑자를 생각하면 쉽다.)
        RenderScaleY, // 공간의 세로길이.
        hdcSrc,       // 원본 이미지
        ImageLeft,    // 원하는 이미지 시작점 X
        ImageTop,     // 원하는 이미지 시작점 Y
        ImageScaleX,  // 원하는 이미지 자를 만큼의 가로 길이.
        ImageScaleY,  // 원하는 이미지 자를 만큼의 세로 길이.
        _Color.Color  // 특정 색을 투명으로 처리.
    );
}

void UWindowImage::AlphaCopy(UWindowImage* _CopyImage, const FTransform& _Trans, int _Index, Color8Bit _Color)
{
    // 방어 코드
    if (nullptr == _CopyImage)
    {
        MsgBoxAssert("nullptr 인 이미지를 복사할 수 없습니다.");
    }

    // 방어 코드
    if (_Index >= _CopyImage->Infos.size())
    {
        MsgBoxAssert(GetName() + "이미지 정보의 인덱스를 오버하여 사용했습니다. Alpha");
    }

    // 받은 이미지 설정 가져오기. -> 좌표 가져오기.
    FTransform& ImageTrans = _CopyImage->Infos[_Index].CuttingTrans;

    // 설명 보충 필요.[TODO]
    // 기존 좌표.
    int RenderLeft = _Trans.iLeft();
    int RenderTop = _Trans.iTop();
    int RenderScaleX = _Trans.GetScale().iX();
    int RenderScaleY = _Trans.GetScale().iY();

    // 이미지 좌표.
    int ImageLeft = ImageTrans.GetPosition().iX();
    int ImageTop = ImageTrans.GetPosition().iY();
    int ImageScaleX = ImageTrans.GetScale().iX();
    int ImageScaleY = ImageTrans.GetScale().iY();

    // 그려줄 Main DC
    HDC hdc = ImageDC;

    // 이미지 DC 설정
    HDC hdcSrc = _CopyImage->Infos[_Index].ImageDC;

    // Alpha 값 지우기.
    BLENDFUNCTION Function;
    Function.BlendOp = AC_SRC_OVER;
    Function.BlendFlags = 0; // 0~255
    Function.SourceConstantAlpha = _Color.A;
    Function.AlphaFormat = AC_SRC_ALPHA;

    // 설정.
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
        MsgBoxAssert("nullptr 인 이미지를 복사할 수 없습니다");
    }

    if (_Index >= _CopyImage->Infos.size())
    {
        MsgBoxAssert(GetName() + "이미지 정보의 인덱스를 오버하여 사용했습니다");
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

    // 원하는 각도만큼 회전 시킨 값을 만들어야 한다.

    if (nullptr == CurInfo.RotationMaskImage)
    {
        MsgBoxAssert("이미지를 회전시키려고 했는데 이미지가 없습니다.");
    }

    // 이제 그리면 된다.
    HDC hdc = ImageDC;
    //// 이미지
    HDC hdcSrc = _CopyImage->Infos[_Index].ImageDC;

    PlgBlt(
        hdc, 							  // HDC hdc, // 
        Arr,
        hdcSrc,							// HDC hdcSrc, 
        ImageLeft,   							// int y1, 
        ImageTop,   							// int x1,  
        ImageScaleX, 							// int y1, 
        ImageScaleY, 							// int y1, 
        CurInfo.RotationMaskImage->hBitMap, // 검정색 바탕의 그림.
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

    TextCopyFormat(_Text, _Font, stringFormat, _Size, _Trans, _Color); //출력
}

void UWindowImage::TextCopy(const std::string& _Text, const std::string& _Font, float _Size, const FTransform& _Trans, Color8Bit _OutLineColor, Color8Bit _FillColor)
{
    Gdiplus::Graphics graphics(ImageDC);
    std::wstring WFont = UEngineString::AnsiToUniCode(_Font);
    Gdiplus::Font fnt(WFont.c_str(), _Size, Gdiplus::FontStyleBold | Gdiplus::FontStyleItalic, Gdiplus::UnitPixel);

    // 테두리용 브러시 설정
    Gdiplus::SolidBrush OutLineBrush(Gdiplus::Color(_OutLineColor.R, _OutLineColor.G, _OutLineColor.B));

    // 내부 채우기용 브러시 설정
    Gdiplus::SolidBrush fillBrush(Gdiplus::Color(_FillColor.R, _FillColor.G, _FillColor.B));

    FVector Pos = _Trans.GetPosition();
    Gdiplus::RectF rectF(Pos.X, Pos.Y, 0, 0);

    Gdiplus::StringFormat stringFormat;
    stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
    stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);
    std::wstring WText = UEngineString::AnsiToUniCode(_Text);

    // 테두리 효과를 위해 텍스트를 여러 방향으로 그립니다.
    float offsetsX[] = { -3.f, 3.f }; // 테두리의 두께를 조절하려면 이 값을 조정.
    float offsetsY[] = { -2.f, 2.f }; // 테두리의 두께를 조절하려면 이 값을 조정.
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
    float offsets_X[] = { -2.f, 2.f }; // 내부의 두께를 조절하려면 이 값을 조정.
    float offsets_Y[] = { -1.f, 1.f }; // 내부의 두께를 조절하려면 이 값을 조정.
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
    graphics.DrawString(WText.c_str(), -1, &fnt, rectF, &stringFormat, &hB);  //출력
}

void UWindowImage::TextCopyFormat(const std::string& _Text, const std::string& _Font, const Gdiplus::StringFormat& stringFormat, float _Size, const FTransform& _Trans, Color8Bit _Color)
{
    // GDI+ 라는 라이브러리를 사용해야 한다.
    Gdiplus::Graphics graphics(ImageDC);

    std::wstring WFont = UEngineString::AnsiToUniCode(_Font);
    Gdiplus::Font fnt(WFont.c_str(), _Size, 0, Gdiplus::UnitPixel);

    Gdiplus::SolidBrush hB(Gdiplus::Color(_Color.R, _Color.G, _Color.B));

    //FVector Pos = _Trans.GetPosition();
    //Gdiplus::PointF ptf(Pos.X, Pos.Y);
    Gdiplus::RectF rectF(_Trans.GetPosition().X, _Trans.GetPosition().Y, 0, 0);

    std::wstring WText = UEngineString::AnsiToUniCode(_Text);

    //graphics.DrawString(WText.c_str(), -1, &fnt, ptf, &hB);  //출력
    graphics.DrawString(WText.c_str(), -1, &fnt, rectF, &stringFormat, &hB);  //출력
}

void UWindowImage::TextPrint(std::string_view _Text, FVector _Pos)
{
    TextOutA(ImageDC, _Pos.iX(), _Pos.iY(), _Text.data(), static_cast<int>(_Text.size()));
}

bool UWindowImage::Create(UWindowImage* _Image, const FVector& _Scale)
{
    // HBITMAP 비트맵 이미지의 메모리권한
    HANDLE ImageHandle = CreateCompatibleBitmap(_Image->ImageDC, _Scale.iX(), _Scale.iY());

    if (nullptr == ImageHandle)
    {
        MsgBoxAssert("이미지 생성에 실패했습니다");
        return false;
    }

    // 핸들 가져오기
    hBitMap = reinterpret_cast<HBITMAP>(ImageHandle);

    // 기존 이미지의 DC 가져오기
    ImageDC = CreateCompatibleDC(_Image->ImageDC);
    
    if (nullptr == ImageDC)
    {
        MsgBoxAssert("이미지 생성에 실패했습니다");
        return false;
    }

    HBITMAP OldBitMap = reinterpret_cast<HBITMAP>(SelectObject(ImageDC, hBitMap));
    DeleteObject(OldBitMap);

    // hBitMap에서 얻어오겠다.
    GetObject(hBitMap, sizeof(BITMAP), &BitMapInfo);

    return true;
}

void UWindowImage::Cutting(int _X, int _Y)
{
    // 한 번 클리어 해준다.
    Infos.clear();

    // 이미지를 해당 크기 만큼 자를 것이다.
    FVector CuttingScale = { GetScale().X / _X, GetScale().Y / _Y };
    // 컷팅 시작점 설정.
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

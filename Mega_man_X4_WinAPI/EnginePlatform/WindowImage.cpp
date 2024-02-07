#include "WindowImage.h"
#include <EngineBase/EngineString.h>
#include <EngineBase/EngineDebug.h>

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
}

bool UWindowImage::Load(UWindowImage* _Image)
{
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
    }

    // DC 설정.
    
    // ImageDC를 만들면 내부에서 1,1크기의 HBITMAP을 만든다.
    ImageDC = CreateCompatibleDC(_Image->ImageDC);

    if (nullptr == ImageDC)
    {
        MsgBoxAssert("이미지 생성에 실패했습니다.");
        return false;
    }

    
    // DC, HBITMAP, HBITMAP 이런 상황.
    // 때문에 일회용 HBITMAP 을 만들고
    HBITMAP OldBitMap = reinterpret_cast<HBITMAP>(SelectObject(ImageDC, hBitMap));
    // 지워준다.
    DeleteObject(OldBitMap);

    

    // 현재 DC와 연결된 비트맵 핸들 값을 얻어와야 한다.
    // hBitMap에서 



    return false;
}

#include "WindowImage.h"
#include <EngineBase/EngineString.h>

#pragma comment(lib, "Msimg32.lib")

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

    }

    return false;
}

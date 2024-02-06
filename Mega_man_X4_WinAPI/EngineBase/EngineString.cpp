#include "EngineString.h"
#include "EngineDebug.h"
#include <Windows.h>

std::string UEngineString::ToUpper(std::string_view View)
{
    std::string Name = View.data();

    for (char& _Ch : Name)
    {
        _Ch = std::toupper(_Ch);
    }
    
    return Name;
}

std::wstring UEngineString::AnsiToUniCode(std::string_view _View)
{
    // Windows 가 지원.
    // UINT CodePage        = CP_ACP 현재 window에 설정된 초기 문자열로 인코딩 처리.
    // DWORD dwFlags        = 추가 정보 유무. 아니오 0 / 기본 0으로 설정.
    // LPCCH lpMultiByteStr = 변환할 문자열.
    // int cbMultiByte      = 변환할 문자열의 길이.
    // LPWSTR lpWideCharStr = wide 바이트 문자열. nullptr을 넣으면 크기를 리턴.
    // int cchWideChar      = wide 바이트 문자열의 크기.
    int Size = MultiByteToWideChar(CP_ACP, 0, _View.data(), static_cast<int>(_View.size()), nullptr, 0);
    
    if (0 == Size)
    {
        MsgBoxAssert("문자열 변환에 실패했거나 크기가 0인 문자열이 들어왔습니다.");
        return L"";
    }

    std::wstring Result;
    Result.resize(Size);

    // 위에서 wide 문자열의 크기를 얻었으니 Result에 Size만큼 담음.
    Size = MultiByteToWideChar(CP_ACP, 0, _View.data(), static_cast<int>(_View.size()), &Result[0], Size);
    if (0 == Size)
    {
        MsgBoxAssert("문자열 변환에 실패했거나 크기가 0인 문자열이 들어왔습니다.");
        return L"";
    }

    return Result;
}

UEngineString::UEngineString()
{
}

UEngineString::~UEngineString()
{
}

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
    // Windows �� ����.
    // UINT CodePage        = CP_ACP ���� window�� ������ �ʱ� ���ڿ��� ���ڵ� ó��.
    // DWORD dwFlags        = �߰� ���� ����. �ƴϿ� 0 / �⺻ 0���� ����.
    // LPCCH lpMultiByteStr = ��ȯ�� ���ڿ�.
    // int cbMultiByte      = ��ȯ�� ���ڿ��� ����.
    // LPWSTR lpWideCharStr = wide ����Ʈ ���ڿ�. nullptr�� ������ ũ�⸦ ����.
    // int cchWideChar      = wide ����Ʈ ���ڿ��� ũ��.
    int Size = MultiByteToWideChar(CP_ACP, 0, _View.data(), static_cast<int>(_View.size()), nullptr, 0);
    
    if (0 == Size)
    {
        MsgBoxAssert("���ڿ� ��ȯ�� �����߰ų� ũ�Ⱑ 0�� ���ڿ��� ���Խ��ϴ�.");
        return L"";
    }

    std::wstring Result;
    Result.resize(Size);

    // ������ wide ���ڿ��� ũ�⸦ ������� Result�� Size��ŭ ����.
    Size = MultiByteToWideChar(CP_ACP, 0, _View.data(), static_cast<int>(_View.size()), &Result[0], Size);
    if (0 == Size)
    {
        MsgBoxAssert("���ڿ� ��ȯ�� �����߰ų� ũ�Ⱑ 0�� ���ڿ��� ���Խ��ϴ�.");
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

#pragma once
#include <string>
#include <string_view>

class UEngineString
{
public :
	static std::string ToUpper(std::string_view View);

	/// <summary>
	/// string �� wstring ���� �ٲٴ� �Լ�.
	/// </summary>
	/// <param name="View"></param>
	/// <returns></returns>
	static std::wstring AnsiToUniCode(std::string_view _View);

protected :

private :
	// constructer destructer
	UEngineString();
	~UEngineString();

	// delete Function
	UEngineString(const UEngineString& _Other) = delete;
	UEngineString(UEngineString&& _Other) noexcept = delete;
	UEngineString& operator=(const UEngineString) = delete;
	UEngineString& operator=(UEngineString&& _Other) noexcept = delete;
};


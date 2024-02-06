#pragma once
#include <string>
#include <string_view>

class UEngineString
{
public :
	static std::string ToUpper(std::string_view View);

	/// <summary>
	/// string 을 wstring 으로 바꾸는 함수.
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


#pragma once
#include <string>
#include <string_view>

class UEngineString
{
public :
	static std::string ToUpper(std::string_view View);

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


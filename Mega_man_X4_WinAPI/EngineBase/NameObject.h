#pragma once
#include <string>

/// <summary>
/// 이름만 가지는 Object
/// </summary>
class UNameObject
{
public :
	// constructer destructer
	UNameObject();
	~UNameObject();

	// delete Function
	UNameObject(const UNameObject& _Other) = delete;
	UNameObject(UNameObject&& _Other) noexcept = delete;
	UNameObject& operator=(const UNameObject) = delete;
	UNameObject& operator=(UNameObject&& _Other) noexcept = delete;

	void SetName(std::string_view _View)
	{
		Name = _View;
	}

	std::string GetName()
	{
		return Name;
	}

protected :

private :
	std::string Name;
};
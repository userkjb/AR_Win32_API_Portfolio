#pragma once
#include <string>
class NameObject
{
public :
	// constructer destructer
	NameObject();
	~NameObject();

	// delete Function
	NameObject(const NameObject& _Other) = delete;
	NameObject(NameObject&& _Other) noexcept = delete;
	NameObject& operator=(const NameObject) = delete;
	NameObject& operator=(NameObject&& _Other) noexcept = delete;

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
#pragma once

#include "NameObject.h"
#include "EnginePath.h"

/// <summary>
/// 이름과 경로를 갖는 Object
/// </summary>
class UPathObject : public UNameObject
{
public :
	// constructer destructer
	UPathObject();
	~UPathObject();

	// delete Function
	UPathObject(const UPathObject& _Other) = delete;
	UPathObject(UPathObject&& _Other) noexcept = delete;
	UPathObject& operator=(const UPathObject) = delete;
	UPathObject& operator=(UPathObject&& _Other) noexcept = delete;

	void SetPath(std::string_view _View)
	{
		Path = _View;
	}

	std::string GetPath()
	{
		return Path;
	}

	UEnginePath GetEnginePath()
	{
		return UEnginePath(std::filesystem::path(Path));
	}

protected :

private :
	std::string Path;
};


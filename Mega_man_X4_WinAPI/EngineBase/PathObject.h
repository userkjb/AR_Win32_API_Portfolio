#pragma once


class UPathObject
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

protected :

private :

};


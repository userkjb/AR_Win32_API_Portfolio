#pragma once
class NameObject
{
public :
	// constructer destructer
	NameObject();
	virtual ~NameObject();

	// delete Function
	NameObject(const NameObject& _Other) = delete;
	NameObject(NameObject&& _Other) noexcept = delete;
	NameObject& operator=(const NameObject) = delete;
	NameObject& operator=(NameObject&& _Other) noexcept = delete;

protected :

private :

};


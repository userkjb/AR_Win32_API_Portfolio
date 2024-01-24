#pragma once
class EngineString
{
public :
	// constructer destructer
	EngineString();
	virtual ~EngineString();

	// delete Function
	EngineString(const EngineString& _Other) = delete;
	EngineString(EngineString&& _Other) noexcept = delete;
	EngineString& operator=(const EngineString) = delete;
	EngineString& operator=(EngineString&& _Other) noexcept = delete;

protected :

private :

};


#pragma once
#include <EngineCore/EngineCore.h>

class ContentsCore : public UEngineCore
{
public :
	// constructer destructer
	ContentsCore();
	~ContentsCore();

	// delete Function
	ContentsCore(const ContentsCore& _Other) = delete;
	ContentsCore(ContentsCore&& _Other) noexcept = delete;
	ContentsCore& operator=(const ContentsCore) = delete;
	ContentsCore& operator=(ContentsCore&& _Other) noexcept = delete;

protected :
	/// <summary>
	/// EngineCore에 있는 WindowOpen 가상 함수.
	/// </summary>
	/// <param name="_OutWindowTitle"></param>
	/// <param name="_SmallIconPath"></param>
	void WindowOpen(std::string& _OutWindowTitle, std::string& _SmallIconPath) override;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private :

};


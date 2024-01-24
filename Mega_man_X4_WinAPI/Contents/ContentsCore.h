#pragma once
#include <EngineCore/EngineCore.h>

class ContentsCore : public EngineCore
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
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void End() override;

private :

};


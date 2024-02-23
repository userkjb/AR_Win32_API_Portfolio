#pragma once
#include <EngineCore/Actor.h>

class ACyberSpaceMap : public AActor
{
public :
	// constrcuter destructer
	ACyberSpaceMap();
	~ACyberSpaceMap();

	// delete Function
	ACyberSpaceMap(const ACyberSpaceMap& _Other) = delete;
	ACyberSpaceMap(ACyberSpaceMap&& _Other) noexcept = delete;
	ACyberSpaceMap& operator=(const ACyberSpaceMap& _Other) = delete;
	ACyberSpaceMap& operator=(ACyberSpaceMap&& _Other) noexcept = delete;

	// Map
	void SetMapImage(std::string_view _MapImageName);

	void SetColMapImage(std::string_view _MapImageName);


	void SwitchDebug();

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private :
	UImageRenderer* MapRenderer = nullptr;	// Map �̹���
	UImageRenderer* ColRenderer = nullptr;	// ColMap �̹���
};

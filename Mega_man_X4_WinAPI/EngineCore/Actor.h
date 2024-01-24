#pragma once
class AActor
{
public :
	// constructer destructer
	AActor();
	~AActor();

	// delete Function
	AActor(const AActor& _Other) = delete;
	AActor(AActor&& _Other) noexcept = delete;
	AActor& operator=(const AActor) = delete;
	AActor& operator=(AActor&& _Other) noexcept = delete;

protected :

private :

};


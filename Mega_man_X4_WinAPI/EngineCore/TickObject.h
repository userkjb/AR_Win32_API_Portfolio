#pragma once
class UTickObject
{
public :
	// constructer destructer
	UTickObject();
	virtual ~UTickObject();

	// delete Function
	UTickObject(const UTickObject& _Other) = delete;
	UTickObject(UTickObject&& _Other) noexcept = delete;
	UTickObject& operator=(const UTickObject) = delete;
	UTickObject& operator=(UTickObject&& _Other) noexcept = delete;

	void ActiveOn()
	{
		IsActiveValue = true;
	}

	void ActiveOff()
	{
		IsActiveValue = false;
	}

	void SetActive(bool _Active)
	{
		IsActiveValue = _Active;
	}

	bool IsActive()
	{
		return IsActiveValue && IsDestroyValue == false;
	}


	void Destroy()
	{
		IsDestroyValue = true;
	}

	bool IsDestroy()
	{
		return IsDestroyValue;
	}

	virtual void BeginPlay();
	virtual void Tick(float _DeltaTime);

protected :

private :
	bool IsActiveValue = true;
	bool IsDestroyValue = false;
};


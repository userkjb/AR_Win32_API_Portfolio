#pragma once
class UpdateObject
{
public :
	// constructer destructer
	UpdateObject();
	virtual ~UpdateObject();

	// delete Function
	UpdateObject(const UpdateObject& _Other) = delete;
	UpdateObject(UpdateObject&& _Other) noexcept = delete;
	UpdateObject& operator=(const UpdateObject) = delete;
	UpdateObject& operator=(UpdateObject&& _Other) noexcept = delete;

	void On()
	{
		IsUpdateValue = true;
	}

	void Off()
	{
		IsUpdateValue = false;
	}

	bool IsOn()
	{
		return IsUpdateValue;
	}


	void Death()
	{
		IsDeathValue = true;
	}

	bool IsDeath()
	{
		return IsDeathValue;
	}

protected :

private :
	bool IsUpdateValue = true;
	bool IsDeathValue = false;
};


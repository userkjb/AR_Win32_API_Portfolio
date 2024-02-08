#pragma once

#include <cmath>

struct float4
{
public :
	static const float4 Zero;
	static const float4 Left;
	static const float4 Right;
	static const float4 Up;
	static const float4 Down;

	float X; // 2D
	float Y; // 2D
	float Z; // 3D
	float W; // 3D

	// 생성자를 한번 만들게 되면 리스트 이니셜라이저가 동작하지 않아서
	// 개인이 생성하는 방식을 다 정의해야 합니다.
	float4()
		: X(0.0f), Y(0.0f), Z(0.0f), W(1.0f)
	{

	}
	
	float4(float _X, float _Y, float _Z, float _W)
		: X(_X), Y(_Y), Z(_Z), W(_W)
	{

	}

	// 사용하고자 하는 함수 추가.
	float4(long _X, long _Y)
		: X(static_cast<float>(_X)), Y(static_cast<float>(_Y)), Z(0.0f), W(1.0f)
	{

	}

public:
	float hX()
	{
		return X * 0.5f;
	}

	float hY()
	{
		return Y * 0.5f;
	}


	int ihY()
	{
		return static_cast<int>(hY());
	}

	int ihX()
	{
		return static_cast<int>(hX());
	}

	float4 operator+(const float4& _Other)
	{
		float4 Result = *this;
		Result.X += _Other.X;
		Result.Y += _Other.Y;
		Result.Z += _Other.Z;
		return Result;
	}

	float4& operator+=(const float4& _Other)
	{
		X += _Other.X;
		Y += _Other.Y;
		Z += _Other.Z;

		return *this;
	}

	float4 operator*(float _Value) const
	{
		float4 Result = *this;
		Result.X *= _Value;
		Result.Y *= _Value;
		Result.Z *= _Value;
		return Result;
	}

	float4& operator*=(float _Value)
	{
		X *= _Value;
		Y *= _Value;
		Z *= _Value;

		return *this;
	}

	float4 operator*(const float4& _Other) const
	{
		float4 Result = *this;
		Result.X *= _Other.X;
		Result.Y *= _Other.Y;
		Result.Z *= _Other.Z;
		return Result;
	}

	float4& operator*=(const float4& _Other)
	{
		X *= _Other.X;
		Y *= _Other.Y;
		Z *= _Other.Z;

		return *this;
	}

	float4 operator-(const float4& _Other)
	{
		float4 Result = *this;
		Result.X -= _Other.X;
		Result.Y -= _Other.Y;
		Result.Z -= _Other.Z;
		return Result;
	}

	float4& operator-=(const float4& _Other)
	{
		X -= _Other.X;
		Y -= _Other.Y;
		Z -= _Other.Z;

		return *this;
	}
};

using FVector = float4;

class EngineMath
{
	EngineMath();
	~EngineMath();

	// delete Function
	EngineMath(const EngineMath& _Other) = delete;
	EngineMath(EngineMath&& _Other) noexcept = delete;
	EngineMath& operator=(const EngineMath& _Other) = delete;
	EngineMath& operator=(EngineMath&& _Other) noexcept = delete;
};


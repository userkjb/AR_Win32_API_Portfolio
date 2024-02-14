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

	// �����ڸ� �ѹ� ����� �Ǹ� ����Ʈ �̴ϼȶ������� �������� �ʾƼ�
	// ������ �����ϴ� ����� �� �����ؾ� �մϴ�.
	float4()
		: X(0.0f), Y(0.0f), Z(0.0f), W(1.0f)
	{

	}
	
	float4(float _X, float _Y, float _Z, float _W)
		: X(_X), Y(_Y), Z(_Z), W(_W)
	{

	}

	// ����ϰ��� �ϴ� �Լ� �߰�.
	float4(long _X, long _Y)
		: X(static_cast<float>(_X)), Y(static_cast<float>(_Y)), Z(0.0f), W(1.0f)
	{

	}
	
	float4(int _X, int _Y)
		: X(static_cast<float>(_X)), Y(static_cast<float>(_Y)), Z(0.0f), W(1.0f)
	{

	}

	float4(float _X, float _Y)
		: X(_X), Y(_Y), Z(0.0f), W(1.0f)
	{

	}
	
public:
	float hX() const
	{
		return X * 0.5f;
	}

	float hY() const
	{
		return Y * 0.5f;
	}

	float4 Half2D()
	{
		return { hX(), hY() };
	}

	int iX() const
	{
		return static_cast<int>(X);
	}
	int iY() const
	{
		return static_cast<int>(Y);
	}


	int ihY()
	{
		return static_cast<int>(hY());
	}

	int ihX()
	{
		return static_cast<int>(hX());
	}

	// -- operator --

	float4 operator+(const float4& _Other) const
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

	float4 operator-(const float4& _Other) const
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
};

class Color8Bit
{
public :
	static const Color8Bit Black;
	static const Color8Bit Red;
	static const Color8Bit Green;
	static const Color8Bit Blue;
	static const Color8Bit White;
	static const Color8Bit Magenta;

	static const Color8Bit BlackA;
	static const Color8Bit RedA;
	static const Color8Bit GreenA;
	static const Color8Bit BlueA;
	static const Color8Bit WhiteA;
	static const Color8Bit MagentaA;

	// ���İ��� A�� �����ؾ� �ϴϱ�
	// �Ȱ��� �̸� 2���� ����ؾ� �ϹǷ�
	// union �� ����Ѵ�.
	union
	{
		struct
		{
			unsigned char R;
			unsigned char G;
			unsigned char B;
			unsigned char A;
		};

		unsigned char Arr1D[4] = { 0, 0, 0, 255 };
		unsigned int Color;
	};


	Color8Bit() {}

	Color8Bit(
		unsigned char _R,
		unsigned char _G,
		unsigned char _B,
		unsigned char _A
		) : R(_R), G(_G), B(_B), A(_A)
	{}

	// ���� �񱳸� ���� operator
	bool operator==(Color8Bit _Color)
	{
		return Color == _Color.Color;
	}

	Color8Bit ZeroAlphaColor() const
	{
		return Color8Bit{ R,G,B,0 };
	}
};

using FVector = float4;
using FColor = float4;

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


#pragma once

#include <cmath>
#include <Windows.h>

class UEngineMath
{
public:
	UEngineMath();
	~UEngineMath();

	// delete Function
	UEngineMath(const UEngineMath& _Other) = delete;
	UEngineMath(UEngineMath&& _Other) noexcept = delete;
	UEngineMath& operator=(const UEngineMath& _Other) = delete;
	UEngineMath& operator=(UEngineMath&& _Other) noexcept = delete;

	static const float PI;
	static const float PI2;

	// 디그리 -> 라디안
	static const float DToR;
	// 라디안 -> 디그리
	static const float RToD;
};

struct float4
{
public :
	static const float4 Zero;
	static const float4 Left;
	static const float4 Right;
	static const float4 Up;
	static const float4 Down;

	union
	{
		struct
		{
			float X; // 2D
			float Y; // 2D
			float Z; // 3D
			float W; // 3D
		};

		struct
		{
			float R;
			float G;
			float B;
			float A;
		};
	};
	

	// 생성자를 한번 만들게 되면 리스트 이니셜라이저가 동작하지 않아서
	// 개인이 생성하는 방식을 다 정의해야 합니다.
	float4()
		: X(0.0f), Y(0.0f), Z(0.0f), W(1.0f)
	{

	}
	
	// 사용하고자 하는 함수 추가.
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

	float4(int _X, int _Y, int _Z, int _W)
		: X(static_cast<float>(_X)), Y(static_cast<float>(_Y)), Z(static_cast<float>(_Z)), W(static_cast<float>(_W))
	{

	}

	float4(float _X, float _Y, float _Z, float _W)
		: X(_X), Y(_Y), Z(_Z), W(_W)
	{

	}
	
public:

	static float4 VectorRotationZToDeg(float4 _OriginVector, float _Angle)
	{
		return VectorRotationZToRad(_OriginVector, _Angle * UEngineMath::DToR);
	}

	static float4 VectorRotationZToRad(float4 _OriginVector, float _Angle)
	{
		float4 Result;
		Result.X = (_OriginVector.X * cosf(_Angle)) - (_OriginVector.Y * sinf(_Angle));
		Result.Y = (_OriginVector.X * sinf(_Angle)) + (_OriginVector.Y * cosf(_Angle));
		return Result;
	}

	static float4 DegToDir(float _Angle)
	{
		return RadToDir(_Angle * UEngineMath::DToR);
	}

	static float4 RadToDir(float _Angle)
	{
		// 특정 각도에 빗변의 길이가 1인 방향 벡터를 구해줍니다.
		return float4(cosf(_Angle), sinf(_Angle));
	}

	static float4 LerpClamp(float4 p1, float4 p2, float d1)
	{
		if (0.0f >= d1)
		{
			d1 = 0.0f;
		}

		if (1.0f <= d1)
		{
			d1 = 1.0f;
		}

		return Lerp(p1, p2, d1);
	}

	// p1 p2          d1의 비율로 간다.
	static float4 Lerp(float4 p1, float4 p2, float d1) 
	{
		return (p1 * (1.0f - d1)) + (p2 * d1);
	}

	/// <summary>
	/// sqrtf = 제곱근 리턴.
	/// </summary>
	/// <returns></returns>
	float Size2D()
	{
		return std::sqrtf((X * X) + (Y * Y));
	}

	void RotationZToDeg(float _Angle)
	{
		RotationZToRad(_Angle * UEngineMath::DToR);
	}

	void RotationZToRad(float _Angle)
	{
		*this = VectorRotationZToRad(*this, _Angle);
		return;
	}

	/// <summary>
	/// 자신의 길이를 1로 변경하는 함수.
	/// </summary>
	void Normalize2D()
	{
		float Size = Size2D();
		X /= Size;
		Y /= Size;
		Z = 0.0f;
		W = 0.0f;
	}

	/// <summary>
	/// 길이 1로 자신의 방향 벡터를 리턴하는 함수.
	/// </summary>
	/// <returns></returns>
	float4 Normalize2DReturn()
	{
		float4 Result = *this;
		Result.Normalize2D();
		return Result;
	}

	float4 Half2D()
	{
		return { hX(), hY() };
	}

	bool IsZeroVector2D() const
	{
		return X == 0.0f && Y == 0.0f;
	}

	float hX() const
	{
		return X * 0.5f;
	}

	float hY() const
	{
		return Y * 0.5f;
	}

	int iX() const
	{
		return std::lround(X);
	}
	int iY() const
	{
		return std::lround(Y);
	}


	int ihY()
	{
		return std::lround(hY());
	}

	int ihX()
	{
		return std::lround(hX());
	}

	// -- operator --

	float4& operator=(const float4& _Other)
	{
		X = _Other.X;
		Y = _Other.Y;
		Z = _Other.Z;
		W = _Other.W;
		return *this;
	}

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

	float4 operator-()
	{
		float4 Result;
		Result.X = -X;
		Result.Y = -Y;
		Result.Z = -Z;
		return Result;
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

	/// <summary>
	/// 해당 점(Point)의 좌표를 반환.
	/// </summary>
	/// <returns></returns>
	POINT ConvertToWinApiPOINT()
	{
		return { iX(),iY() };
	}
};

using FVector = float4;
using FColor = float4;

class Color8Bit
{
public :
	static const Color8Bit Black;
	static const Color8Bit Red;
	static const Color8Bit Green;
	static const Color8Bit Blue;
	static const Color8Bit Yellow;
	static const Color8Bit White;
	static const Color8Bit Magenta;
	static const Color8Bit Orange;

	static const Color8Bit BlackA;
	static const Color8Bit RedA;
	static const Color8Bit GreenA;
	static const Color8Bit BlueA;
	static const Color8Bit YellowA;
	static const Color8Bit WhiteA;
	static const Color8Bit MagentaA;
	static const Color8Bit OrangeA;

	// 알파값인 A를 생각해야 하니까
	// 똑같은 이름 2개를 사용해야 하므로
	// union 을 사용한다.
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

	// 색상 비교를 위한 operator
	bool operator==(Color8Bit _Color)
	{
		return Color == _Color.Color;
	}

	Color8Bit ZeroAlphaColor() const
	{
		return Color8Bit{ R,G,B,0 };
	}
};

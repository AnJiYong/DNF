#include "PreCompile.h"
#include "GameEngineMath.h"

const float GameEngineMath::PI = 3.14159265359f;
const float GameEngineMath::PI2 = 3.14159265359f * 2;
const float GameEngineMath::DegreeToRadian = GameEngineMath::PI / 180.f;
const float GameEngineMath::RadianToDegree = 180.f / GameEngineMath::PI;

const float4 float4::ZERO = { 0.f, 0.f, 0.f, 0.f };
const float4 float4::ONE = { 1.f, 1.f, 1.f, 1.f };
const float4 float4::LEFT = { -1.f, 0.f };
const float4 float4::RIGHT = { 1.f, 0.f };
const float4 float4::UP = { 0.f, 1.f };
const float4 float4::DOWN = { 0.f, -1.f };

const float4 float4::RED = { 1.f, 0.f, 0.f, 1.f };
const float4 float4::BLUE = { 0.f, 0.f, 1.f, 1.f };
const float4 float4::GREEN = { 0.f, 1.f, 0.f, 1.f };
const float4 float4::WHITE = { 1.f, 1.f, 1.f, 1.f };
const float4 float4::BLACK = { 0.f, 0.f, 0.f, 1.f };
const float4 float4::NONE = { 0.f, 0.f, 0.f, 0.f };

float4 float4::RotateYRadian(const float4& _OriginVector, float _Radian)
{
	float4x4 Rot;
	Rot.RotationYRad(_Radian);
	return _OriginVector * Rot;
}

float4 float4::RotateXRadian(const float4& _OriginVector, float _Radian)
{
	float4x4 Rot;
	Rot.RotationXRad(_Radian);
	return _OriginVector * Rot;
}

float4 float4::RotateZRadian(const float4& _OriginVector, float _Radian)
{
	float4x4 Rot;
	Rot.RotationZRad(_Radian);
	return _OriginVector * Rot;
}

float4 float4::DirZRadian(float _Radian)
{
	float4x4 Rot;
	Rot.RotationZRad(_Radian);
	return float4::RIGHT * Rot;
}

float4 float4::operator*(const float4x4& _Value) const
{
	return DirectX::XMVector4Transform(DirectVector, _Value.DirectMatrix);
}

float4& float4::operator*=(const float4x4& _Value)
{
	DirectVector = DirectX::XMVector4Transform(DirectVector, _Value.DirectMatrix);
	return *this;
}

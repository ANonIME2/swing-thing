#pragma once

class Vec2
{
public:
	Vec2(float x=0, float y=0);
	Vec2 operator+(const Vec2& a);
	Vec2 operator+=(const Vec2& a);
	Vec2 operator-=(const Vec2& a);
	Vec2 operator-(const Vec2& a);
	Vec2 operator*=(const Vec2& a);
	Vec2 operator*(const Vec2& a);
	Vec2 operator/=(const Vec2& a);
	Vec2 operator/(const Vec2& a);
	Vec2 operator*(const float a);
	Vec2 operator/(const float a);
	Vec2 operator-(const float a);
	Vec2 operator+(const float a);
	Vec2 operator*=(const float a);
	Vec2 operator/=(const float a);
	Vec2 operator-=(const float a);
	Vec2 operator+=(const float a);

	float first, second;
};


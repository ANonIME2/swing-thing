#include "Vec2.h"

Vec2::Vec2(float x, float y)
{
    first = x;
    second = y;
}

Vec2 Vec2::operator+(const Vec2& a)
{
    return Vec2(this->first + a.first, this->second + a.second);
}

Vec2 Vec2::operator+(const float a)
{
    return Vec2(this->first + a, this->second + a);
}

Vec2 Vec2::operator+=(const Vec2& a)
{
    this->first += a.first;
    this->second += a.second;
    return *this;
}


Vec2 Vec2::operator+=(const float a)
{
    this->first += a;
    this->second += a;
    return *this;
}

Vec2 Vec2::operator-(const Vec2& a)
{
    return Vec2(this->first - a.first, this->second - a.second);
}

Vec2 Vec2::operator-(const float a)
{
    return Vec2(this->first - a, this->second - a);
}

Vec2 Vec2::operator-=(const Vec2& a)
{
    this->first -= a.first;
    this->second -= a.second;
    return *this;
}


Vec2 Vec2::operator-=(const float a)
{
    this->first -= a;
    this->second -= a;
    return *this;
}

Vec2 Vec2::operator*(const Vec2& a)
{
    return Vec2(this->first * a.first, this->second * a.second);
}

Vec2 Vec2::operator*(const float a)
{
    return Vec2(this->first * a, this->second * a);
}

Vec2 Vec2::operator*=(const Vec2& a)
{
    this->first *= a.first;
    this->second *= a.second;
    return *this;
}

Vec2 Vec2::operator*=(const float a)
{
    this->first *= a;
    this->second *= a;
    return *this;
}


Vec2 Vec2::operator/(const Vec2& a)
{
    return Vec2(this->first / a.first, this->second / a.second);
}


Vec2 Vec2::operator/(const float a)
{
    return Vec2(this->first / a, this->second / a);
}

Vec2 Vec2::operator/=(const Vec2& a)
{
    this->first /= a.first;
    this->second /= a.second;
    return *this;
}


Vec2 Vec2::operator/=(const float a)
{
    this->first /= a;
    this->second /= a;
    return *this;
}
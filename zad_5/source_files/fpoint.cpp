#include "fpoint.h"

FPoint min(const FPoint& lf, const FPoint& rt)
{
    return FPoint(std::min(lf.x, rt.x), std::min(lf.y, rt.y));
}

FPoint max(const FPoint& lf, const FPoint& rt)
{
    return FPoint(std::max(lf.x, rt.x), std::max(lf.y, rt.y));
}

FPoint operator+(const FPoint& lpoint, float val)
{
    return FPoint(lpoint.x + val, lpoint.y + val);
}

FPoint operator-(const FPoint& lpoint, float val)
{
    return FPoint(lpoint.x - val, lpoint.y - val);
}

FPoint operator+(const FPoint& lpoint, const FPoint& rpoint)
{
    return FPoint(lpoint.x + rpoint.x, lpoint.y + rpoint.y);
}

FPoint operator*(const FPoint& lpoint, const FPoint& rpoint)
{
    return FPoint(lpoint.x * rpoint.x, lpoint.y * rpoint.y);
}

float distance(const FPoint& p1, const FPoint& p2)
{
    return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}
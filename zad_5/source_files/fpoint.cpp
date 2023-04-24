#include "fpoint.h"

FPoint operator*(const FPoint& lpoint, const FPoint& rpoint)
{
    return FPoint(lpoint.x * rpoint.x, lpoint.y * rpoint.y);
}
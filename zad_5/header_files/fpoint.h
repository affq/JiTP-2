#pragma once
#include <iostream>
#include <Graph.h>

class FPoint
{
public:
    float x,y;
    FPoint(float cx=0.0f, float cy=0.0f) : x(cx), y(cy) {}
    friend std::ostream& operator <<(std::ostream& os, const FPoint& p)
    {
        return os << '(' << p.x << ',' << p.y << ')';
    };
    friend std::istream& operator >>(std::istream& is, FPoint& p)
    {
        return is >> p.x >> p.y;
    }
    operator Graph_lib::Point() const
    {
        return Graph_lib::Point((int)x, (int)y);
    }
};

FPoint min(const FPoint& lf, const FPoint& rt);
FPoint max(const FPoint& lf, const FPoint& rt);
FPoint operator+(const FPoint& lpoint, float val);
FPoint operator-(const FPoint& lpoint, float val);
FPoint operator+(const FPoint& lpoint, const FPoint& rpoint);
FPoint operator*(const FPoint& lpoint, const FPoint& rpoint);
float distance(const FPoint& p1, const FPoint& p2);

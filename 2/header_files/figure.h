//Adrian Fabisiewicz (328935)

#pragma once
#include <iostream>
#include <vector>
#include <Graph.h>

class FPoint
{
public:
    float x, y;
    FPoint(float cx = 0.0f, float cy = 0.0f) : x(cx), y(cy) {};
    operator Graph_lib::Point() const { return Graph_lib::Point( int(this -> x), int(this -> y) ); };
};

std::ostream& operator<<(std::ostream& os, const FPoint& p);
std::istream& operator>>(std::istream& is, FPoint& p);

std::vector<FPoint> get_points(std::istream& is);
std::string get_id(std::istream& is);

class figure
{
    std::string id;
    std::vector<FPoint> fdef;
public:
    figure(const std::vector<FPoint>& fv) : fdef(fv) {};
    virtual ~figure() {};

    std::vector<FPoint> get_fdef() const { return fdef; };

    static std::string class_id() { return "Unknown";};
    virtual std::string get_id() const = 0;
    virtual Graph_lib::Shape* get_shape() const = 0;

    friend std::istream& operator>>(std::istream& is, figure& f);
    friend std::ostream& operator<<(std::ostream& os, const figure& f);
};

class Rect : public figure
{
    public:
    Rect(const std::vector<FPoint>& fv) : figure(fv) {
        if (fv.size() != 2)
            throw std::runtime_error("Invalid number of points for rectangle.");
    };
    virtual ~Rect() {};
    static std::string class_id() { return "Rect"; };
    virtual std::string get_id() const { return class_id(); };
    
    virtual Graph_lib::Shape* get_shape() const
    {
        return new Graph_lib::Rectangle(get_fdef()[0], get_fdef()[1]);
    };
};

float distance (const FPoint& p1, const FPoint& p2);

class Circ : public figure
{
    public:
    Circ(const std::vector<FPoint>& fv) : figure(fv) {
        if (fv.size() != 2)
            throw std::runtime_error("Invalid number of points for circle.");
    };
    virtual ~Circ() {};
    static std::string class_id() { return "Circ"; };
    virtual std::string get_id() const { return class_id(); };
    virtual Graph_lib::Shape* get_shape() const
    {
        const std::vector<FPoint>& fdef = get_fdef();
        return new Graph_lib::Circle(fdef[0], int(distance(fdef[0], fdef[1])));
    }
};

figure* get_figure(std::istream& is);
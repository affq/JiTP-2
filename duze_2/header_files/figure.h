//Adrian Fabisiewicz (328935)

#pragma once
#include <iostream>
#include <vector>
#include "Graph.h"

class FPoint {
public:
    float x, y;
    operator Graph_lib::Point() const { return Graph_lib::Point( int(this -> x), int(this -> y) ); };
    FPoint(float cx = 0.0f, float cy = 0.0f) : x(cx), y(cy) {}
};

std::ostream& operator<<(std::ostream& os, const FPoint& p);
std::istream& operator>>(std::istream& is, FPoint& p);
std::ostream& operator<<(std::ostream& os, const std::vector<FPoint>& db1);
std::ostream& operator<<(std::ostream& os, const std::pair<FPoint, FPoint>& db1);

std::vector<FPoint> get_points(std::istream& is);
std::string get_id(std::istream& is);

class figure {
protected:
    std::string id;
    std::vector<FPoint> fdef;
public:
    figure(const std::vector<FPoint>& fv) : fdef(fv) {}
    virtual ~figure() {}
    
    virtual std::pair<FPoint, FPoint> bbox() const;
    static std::string class_id() { return "Unknown"; };
    virtual std::string get_id() const = 0;
    virtual Graph_lib::Shape* get_shape( 
        const FPoint& scale = {1.0f, 1.0f},  
        const FPoint& trans = {0.0f, 0.0f}) const = 0;
    
    friend std::istream& operator>>(std::istream& is, figure& f);
    friend std::ostream& operator<<(std::ostream& os, const figure& f);
};


class Rect : public figure {
public:
    Rect(const std::vector<FPoint>& fv) : figure(fv) {
        if (fv.size() != 2)
            throw std::runtime_error("Invalid number of points for rectangle.");
    }
    virtual ~Rect() {};
    static std::string class_id() { return "Rect"; }
    std::string get_id() const { return class_id(); }

    Graph_lib::Shape* get_shape(const FPoint& p1, const FPoint& p2) const;

};

class Circ : public figure {
public:
    Circ(const std::vector<FPoint>& fv) : figure(fv) {
        if (fv.size() != 2)
            throw std::runtime_error("Invalid number of points for circle.");
    }
    ~Circ() {};
    
    static std::string class_id() { return "Circ"; };
    std::string get_id() const { return class_id(); };

    Graph_lib::Shape* get_shape(const FPoint& p1, const FPoint& p2) const;
};

class Line : public figure {
public:
    Line(const std::vector<FPoint>& fv) : figure(fv) {
        if (fv.size() < 2)
            throw std::runtime_error("Invalid number of points for line.");
    }
    ~Line() {};

    static std::string class_id() { return "Line"; };
    std::string get_id() const { return class_id(); };
    
    Graph_lib::Shape* get_shape(const FPoint& p1, const FPoint& p2) const;
};

string window_title();

float distance(const FPoint& p1, const FPoint& p2);

figure* get_figure(std::istream& is);

std::pair<FPoint, FPoint> get_transformation(
    const std::pair<FPoint, FPoint>& obj_bbox,
    const std::pair<FPoint, FPoint>& win_bbox);

std::pair<FPoint, FPoint> map_bbox(const vector<figure*>& vf);
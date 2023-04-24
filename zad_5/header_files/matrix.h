#pragma once
#include <array>
#include <iostream>
#include "fpoint.h"

template <typename T> class Matrix
{
    static const int size = 3;
    std::array<std::array<T, size>, size> cf;
    void setIdentity()
    {

    }
    void copyMx(const Matrix& rhm);
public:
    Matrix() 
    { 
        setIdentity(); 
    }
    Matrix(const Matrix& rhm) 
    { 
        copyMx(rhm); 
    }
    Matrix& operator = (const Matrix& rhm) { copyMx(rhm); return *this; }
    static Matrix scaleMx(T scaleX, T scaleY);
    static Matrix translateMx(T offsetX, T offsetY);
    static Matrix rotateMx(T angle);
    friend Matrix operator * (const Matrix& lhm, const Matrix& rhm);
    Matrix& operator *= (const Matrix& rhm);
    FPoint transform(const FPoint& pt) const;
    
    const T& operator() (unsigned int rowid, unsigned int colid) const
    {
        if (rowid >= size || colid >= size)
            throw std::invalid_argument("Invalid index in () operator");
        return cf[rowid][colid];
    }
    
    T& operator() (unsigned int rowid, unsigned int colid)
    {
        if (rowid >= size || colid >= size)
            throw std::invalid_argument("Invalid index in () operator");
        return cf[rowid][colid];
    }

    friend std::ostream& operator <<(std::ostream& os, const Matrix& mx)
    {
        return os << sizeof(mx(0,0));
    }
};
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
        for (int i = 0; i < size; ++i)
            for (int j = 0; j < size; ++j)
                if (i == j)
                    cf[i][j] = 1;
                else
                    cf[i][j] = 0;
    }

    void copyMx(const Matrix& rhm)
    {
        for (int i = 0; i < size; ++i)
            for (int j = 0; j < size; ++j)
                cf[i][j] = rhm(i, j);
    }

public:
    Matrix() 
    { 
        setIdentity(); 
    }
    
    Matrix(const Matrix& rhm) 
    { 
        copyMx(rhm); 
    }
    
    Matrix& operator = (const Matrix& rhm) 
    { 
        copyMx(rhm); 
        return *this; 
    }

    static Matrix scaleMx(T scaleX, T scaleY)
    {
        Matrix mx;
        mx(0,0) = scaleX;
        mx(1,1) = scaleY;
        return mx;
    }

    static Matrix translateMx(T offsetX, T offsetY)
    {
        Matrix mx;
        mx(0,2) = offsetX;
        mx(1,2) = offsetY;
        return mx;
    }

    static Matrix rotateMx(T angle)
    {
        Matrix mx;
        angle = angle * M_PI / 180.0f;
        mx(0, 0) = cos(angle);
        mx(0, 1) = -sin(angle);
        mx(1, 0) = sin(angle);
        mx(1, 1) = cos(angle);
        return mx;
    }

    friend Matrix operator * (const Matrix& lhm, const Matrix& rhm)
    {
        Matrix res;
        
        //zerowanie macierzy wynikowej
        for (int i = 0; i < size; ++i)
            for (int j = 0; j < size; ++j) 
                res(i, j) = 0;

        for (int i = 0; i < size; ++i)
            for (int j = 0; j < size; ++j)
                for (int k = 0; k < size; ++k)
                    res(i, j) += lhm(i, k) * rhm(k, j);

        return res;
    }

    Matrix& operator *= (const Matrix& rhm)
    {
        *this = *this * rhm;
        return *this;
    }

    FPoint transform(const FPoint& pt) const
    {
        FPoint res;
        res.x = pt.x * cf[0][0] + pt.y * cf[0][1] + cf[0][2];
        res.y = pt.x * cf[1][0] + pt.y * cf[1][1] + cf[1][2];
        return res;
    }
    
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
        for (int i = 0; i < size; ++i)
        {
            for (int j = 0; j < size; ++j)
                os << mx(i, j) << ' ';
            os << '\n';
        }
        return os;
    }
};
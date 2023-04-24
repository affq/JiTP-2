#include "matrix.h"
#include "fpoint.h"
#include <Graph.h>

//operator porownywania dla fpoint

bool approx_equal(float left, float right, float eps = 1.0e-7)
{
    return fabs(left - right) < eps;
}

int main()
{   
    //testy konstruktora i konwersji
    {
        FPoint pt (0.11117f, -34.23456f);
        
        if (pt.x != 0.11117f || pt.y != -34.23456f)
            cerr << "Bad values after constructor. \n";
        Graph_lib::Point gpt = (Graph_lib::Point)pt;
        
        if (gpt.x != 0 || gpt.y != -34)
            cerr << "Bad values after Point() operator. \n";
    }

    //testy mnozenia
    {
        FPoint pt(1.0f, 0.003f);
        FPoint rt(0.3f, 0.007f);
        FPoint res = pt * rt;

        if (res.x != 0.3f || !approx_equal(res.y, 0.000021f))
        {
            cerr << "Bad values after multiplication: \n";
            cerr << res << '\n';
        }
    }

    //testy konstruktora matrix
    {
        Matrix<float> mx;

        if (mx(0,0) != 1.0f)
            cerr << "Bad values after constructor. \n";
        
        // if (mx(3,3) != 1.0f)
        //     cerr << "Bad values after constructor. \n";

        cout << "Matrix<float> " << mx << '\n';

        Matrix<double> dmx;
        cout << "Matrix<double> " << dmx << '\n';
    }

    // testy dodawania
    {
        FPoint pt(1.0f, 0.003f);
        float val = 0.3f;

        FPoint res = pt + val;

        if (!approx_equal(res.x, 1.3f) || !approx_equal(res.y, 0.303f))
        {
            cerr << "Bad values after addition: \n";
            cerr << res << '\n';
        }
    }

    //testy odejmowania
    {
        FPoint pt(2.56f, 3.78f);
        float val = 0.76f;

        FPoint res = pt - val;

        if (!approx_equal(res.x, 1.8f) || !approx_equal(res.y, 3.02f))
        {
            cerr << "Bad values after subtraction: \n";
            cerr << res << '\n';
        }
    }

    //testy dodawania dwoch punktow
    {
        FPoint p1 (1.0f, 0.053f);
        FPoint p2 (0.006f, 0.37f);

        FPoint res = p1 + p2;

        if (!approx_equal(res.x, 1.006f) || !approx_equal(res.y, 0.423f))
        {
            cerr << "Bad values after addition of two points: \n";
            cerr << res << '\n';
        }
    }

    // testy dystansu
    {
        FPoint p1 (.0f, .0f);
        FPoint p2 (1.0f, 1.0f);

        float res = distance(p1, p2);

        if (!approx_equal(res, sqrt(2)))
        {
            cerr << "Bad values after distance: \n";
            cerr << res << '\n';
        }
    }

    //testy min
    {
        FPoint p1 (2.0f, 3.5f);
        FPoint p2 (6.1f, 0.7f);

        FPoint res = min(p1, p2);

        if (res.x != 2.0f || res.y != 0.7f)
        {
            cerr << "Bad values after min: \n";
            cerr << res << '\n';
        }
    }

    //testy set identity
    {
        Matrix <float> mx;
        cout << mx << endl;
    }

    //testy max
    {
        FPoint p1 (2.0f, 3.5f);
        FPoint p2 (6.1f, 0.7f);

        FPoint res = max(p1, p2);

        if (res.x != 6.1f || res.y != 3.5f)
        {
            cerr << "Bad values after max: \n";
            cerr << res << '\n';
        }
    }

    //testy  Matrix(const Matrix& rhm) 
    {
        Matrix <float> mx;
        mx(1,2) = 3.4f;
        Matrix <float> mx2(mx);
        cout << mx2 << endl;
    } 

    //testy Matrix& operator = (const Matrix& rhm)
    {
        Matrix <float> mx;
        mx(1,1) = 4.4f;
        Matrix <float> mx2;
        mx2 = mx;
        cout << mx2 << endl;
    }

    //testy static Matrix scaleMx(T scaleX, T scaleY)
    {
        Matrix <float> mx = Matrix<float>::scaleMx(2.0f, 3.0f);
        cout << mx << endl;
    }

    //testy static Matrix translateMx(T offsetX, T offsetY)
    {
        Matrix <float> mx = Matrix<float>::translateMx(2.0f, 3.0f);
        cout << mx << endl;
    }

    //testy static Matrix rotateMx(T angle)
    {
        Matrix <float> mx = Matrix<float>::rotateMx(90.0f);
        cout << mx << endl;
    }

    //testy friend Matrix operator * (const Matrix& lhm, const Matrix& rhm)
    {
        Matrix <float> mx;
        mx(0,0) = 2.0f;
        mx(1,1) = 4.5f;
        cout << mx << endl;
        Matrix <float> mx2;
        mx2(0,2) = 3.2f;
        mx2(0,0) = 1.1f;
        mx2(1,1) = 4.5f;
        mx2(2,2) = 3.4f;
        cout << mx2 << endl;
        Matrix <float> mx3 = mx * mx2;
        cout << mx3 << endl;
    }

    //testy Matrix& operator *= (const Matrix& rhm)
    {
        Matrix <float> mx;
        mx(0,0) = 2.0f;
        mx(1,1) = 4.5f;
        Matrix <float> mx2;
        mx2(0,2) = 3.2f;
        mx2(0,0) = 1.1f;
        mx2(1,1) = 4.5f;
        mx2(2,2) = 3.4f;
        mx*=mx2;
        cout << mx;
    }

    //testy FPoint transform(const FPoint& pt) const
    {
        FPoint pt(1,4);
        Matrix <float> mx;
        mx(1,2) = 3;
        FPoint xd = mx.transform(pt);
        cout << xd;
    }
    return 0;
}
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
    
    return 0;
}
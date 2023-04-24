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
    
    return 0;
}
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
    //testy fpoint.h
    {
        //testy konstruktora i konwersji
        {
            FPoint pt (0.11117f, -34.23456f);
            
            if (pt.x != 0.11117f || pt.y != -34.23456f)
                    cerr << "Bad values after constructor. The values are: " << pt << '\n';

            Graph_lib::Point gpt = (Graph_lib::Point)pt;
            
            if (gpt.x != 0 || gpt.y != -34)
                cerr << "Bad values after Point() operator. \n";
        }

        //test min
        {
            FPoint p1 (2.0f, 3.5f);
            FPoint p2 (6.1f, 0.7f);

            FPoint res = min(p1, p2);

            if (res.x != 2.0f || res.y != 0.7f)
                cerr << "Bad values after min: " << res << '\n';
        }

        //test max
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

        // test operatora wyprowadzania


        // test operatora dodawania
        {
            FPoint pt(3.333333f, 0.004203f);
            float val = 0.3f;

            FPoint res = pt + val;

            if (!approx_equal(res.x, 3.633333f) || !approx_equal(res.y, 0.304203f))
                cerr << "Bad values after addition: " << res << '\n';
        }

        //test operatora odejmowania
        {
            FPoint pt(2.56f, 3.78f);
            float val = 0.765435f;

            FPoint res = pt - val;

            if (!approx_equal(res.x, 1.794565f) || !approx_equal(res.y, 3.014565f))
                cerr << "Bad values after subtraction: " << res << '\n';
        }

        //test operatora dodawania dwoch fpunktow
        {
            FPoint p1 (1.0f, 0.053f);
            FPoint p2 (0.006f, 0.37f);

            FPoint res = p1 + p2;

            if (!approx_equal(res.x, 1.006f) || !approx_equal(res.y, 0.423f))
                cerr << "Bad values after addition of two points: " << res << '\n';
        }

        //test operatora mnozenia dwoch fpunktow
        {
            FPoint pt(0.777f, 0.11112f);
            FPoint rt(3.55421f, 0.3245f);
            FPoint res = pt * rt;

            if (res.x != 2.7616212f || !approx_equal(res.y, 0.0360584f))
                cerr << "Bad values after multiplication: " << res << '\n';
        }

        //test dystansu
        {
            FPoint p1 (.0f, .0f);
            FPoint p2 (1.0f, 1.0f);

            float res = distance(p1, p2);

            if (!approx_equal(res, sqrt(2)))
                cerr << "Bad values after distance: " << res << '\n';
        }
    }

    //testy matrix.h
    {
        //test konstruktora matrix (i setIdentity)
        {
            Matrix<float> mx;

            if (mx(0,0) != 1.0f || mx(0,1) != 0.0f || mx(0,2) != 0.0f 
            || mx(1,0) != 0.0f || mx(1,1) != 1.0f || mx(1,2) != 0.0f 
            || mx(2,0) != 0.0f || mx(2,1) != 0.0f || mx(2,2) != 1.0f)
                cerr << "Bad values after matrix constructor. \n";
        }

        //test konstruktora2 matrix
        {
            Matrix <float> mx;
            mx(1,2) = 3.4f;
            Matrix <float> mx2(mx);
            
            if (mx2(1,2) != 3.4f)
                cerr << "Bad values after copy constructor. \n";
        }

        //test operatora =
        {
            Matrix <float> mx;
            mx(1,1) = 4.4f;

            Matrix <float> mx2;
            mx2 = mx;
            
            if (mx2(1,1) != 4.4f)
                cerr << "Bad values after = operator. \n";
        }

        //test macierzy skali
        {
            Matrix <float> mx = Matrix<float>::scaleMx(2.0f, 3.0f);
            
            if (mx(0,0) != 2.0f || mx(1,1) != 3.0f)
                cerr << "Bad values after scaleMx. \n";
        }

        //test macierzy translacji
        {
            Matrix <float> mx = Matrix<float>::translateMx(2.0f, 3.0f);

            if (mx(0,2) != 2.0f || mx(1,2) != 3.0f)
                cerr << "Bad values after translateMx. \n";
        }

        //test macierzy rotacji
        {
            Matrix <float> mx = Matrix<float>::rotateMx(90.0f);
            
            if (!approx_equal(mx(0,0), 0.0f) || !approx_equal(mx(0,1), -1.0f)
            || !approx_equal(mx(1,0), 1.0f) || !approx_equal(mx(1,1), 0.0f))
                cerr << "Bad values after rotateMx. \n";
        }

        //test operatora *
        {
            Matrix <int> mx;
            mx(0,0) = 2;
            mx(1,1) = 4;
            
            Matrix <int> mx2;
            mx2(0,2) = 3;
            mx2(0,0) = 1;
            mx2(1,1) = 4;
            mx2(2,2) = 3;

            Matrix <int> mx3 = mx * mx2;

            if (mx3(0,0) != 2 || mx3(0,2) != 6 || mx3(1,1) != 16 || mx3(2,2) != 3)
                cerr << "Bad values after multiplication. \n";
        }

        //test operatora *=
        {
            Matrix <int> mx;
            mx(0,0) = 2;
            mx(1,1) = 4;
            
            Matrix <int> mx2;
            mx2(0,2) = 3;
            mx2(0,0) = 1;
            mx2(1,1) = 4;
            mx2(2,2) = 3;

            mx *= mx2;
            
            if (mx(0,0) != 2 || mx(0,2) != 6 || mx(1,1) != 16 || mx(2,2) != 3)
                cerr << "Bad values after *= operator. \n";
        }

        //test transformacji punktu przez macierz
        {
            FPoint pt(1,4);
            Matrix <float> mx;
            mx(1,2) = 3.0f;
            FPoint xd = mx.transform(pt);
            
            if (xd.x != 1.0f || xd.y != 7.0f)
                cerr << "Bad values after transform. \n";
        }

        // //test operatora wyprowadzania dla macierzy
        // {
        //     Matrix <int> mx;
        //     cout << mx;
        // }
    }
    cout << "Koniec testow. \n";
    return 0;
}
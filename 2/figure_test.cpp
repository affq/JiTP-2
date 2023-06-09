#include "figure.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <Simple_window.h>
#include <Point.h>
using namespace std;

int main()
{
    ifstream ifs("mapa_testv.txt");
    if (!ifs.good()) // dobrze sprawdzic przed czytaniem
        throw runtime_error("Cannot find input file.");
    // wersja zdatna do wyswietlania za moment
   vector<figure *> figures;
   figure* pfig;

   while ((pfig = get_figure(ifs)) != nullptr)
   {
       figures.push_back(pfig);
       cout << *pfig << endl;
   }
   ifs.close();

   Simple_window wnd(Point(100, 100), 600, 400, "Rajmund Kozuszek (121528)");
   for (auto pf : figures)
       wnd.attach(*(pf->get_shape()));
   wnd.wait_for_button();
    

    return 0;
}

//g++ -o example.out figure_test.cpp $(find graph_lib/ -name "*.cpp") $(find source_files/ -name "*.cpp") -I graph_lib/ -I header_files/ -lfltk -lfltk_images -lX11 -lXext -O4 && ./example.out < mapa_testv.txt 
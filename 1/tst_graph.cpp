#include <Graph.h>
#include <Simple_window.h>

int main()
{
    Simple_window wnd(Point(100, 100), 600, 400, "Adrian Fabisiewicz (328935)");
    Graph_lib::Polygon poly;
    poly.add(Point(100, 100));
    poly.add(Point(100, 200));
    poly.add(Point(200, 200));
    poly.set_color(Color::red);
    wnd.attach(poly);
    wnd.wait_for_button();
    poly.add(Point(200, 100));
    wnd.wait_for_button();
    return 0;
}

//g++ -o example.out tst_graph.cpp $(find graph_lib/ -name "*.cpp") -I graph_lib/ -lfltk -lfltk_images -lX11 -lXext -O4 && ./example.out
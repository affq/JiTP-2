#include "menuwindow.h"
#include <Graph.h>
#include <Simple_window.h>

int main()
{
    MenuWindow wnd(Graph_lib::Point(100, 100), 600, 400, "Adrian Fabisiewicz (328935)");
    Graph_lib::gui_main();
    return 0;
}
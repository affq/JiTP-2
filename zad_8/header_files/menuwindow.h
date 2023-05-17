#pragma once
#include <Window.h>

class MenuWindow : public Graph_lib::Window
{
public:
    MenuWindow(Graph_lib::Point loc, int width, int height, const string& title);
    ~MenuWindow();
};
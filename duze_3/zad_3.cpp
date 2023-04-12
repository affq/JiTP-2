// Adrian Fabisiewicz (328935)

#include "figure.h"
#include <Graph.h>
#include <Window.h>
#include <GUI.h>

using namespace Graph_lib;

void cb_close(Address, Address addr2)
{
    reinterpret_cast<Graph_lib::Window *> (addr2)->hide();
}

class Spinner;

class SpButton : public Button
{
    Spinner* pSpin = nullptr;
public:
    SpButton (Point loc, int w, int h, const string& label, Callback cb, Spinner* parent):
        Button(loc, w, h, label, cb), pSpin(parent)
    {};

    void attach (Graph_lib::Window& wnd)
    {
        Button::attach(wnd);
        pw->callback (reinterpret_cast<Fl_Callback*> (do_it), pSpin);
    }

};

class Spinner
{
    static const int radius_step = 5;
    
    // center of circle
    static const int center_x = 300;
    static const int center_y = 215;

    SpButton btn_up, btn_down;
    Out_box radius_box;
    int cur_val = 5;

    Circle *pCircle = nullptr;
    Graph_lib::Window *pWindow = nullptr;

    static void cb_up (Address, Address pSpin)
    {
        reinterpret_cast<Spinner *> (pSpin)->change_radius(radius_step);
    };

    static void cb_down (Address, Address pSpin)
    {
        reinterpret_cast<Spinner *> (pSpin)->change_radius(-radius_step);
    };

public:
    Spinner(Point loc, int start_val):
        btn_up(loc, 20, 20, "↑", Spinner::cb_up, this),
        btn_down(Point(loc.x + 60, loc.y), 20, 20, "↓",
            Spinner::cb_down, this),
        radius_box(Point(loc.x + 25, loc.y), 30, 20, ""),
        cur_val(start_val){};

    void attach (Graph_lib::Window& wnd)
    {
        pWindow = &wnd;
        
        wnd.attach(btn_up);
        wnd.attach(btn_down);
        wnd.attach(radius_box);
        radius_box.put(cur_val);

        pCircle = new Circle(Point(center_x, center_y), cur_val);
        wnd.attach(*pCircle);
        wnd.redraw();
    }

    void change_radius(int step)
    {
        if (cur_val + step <= 0 || cur_val + step >= 180)
            return;
        cur_val += step;
        radius_box.put(cur_val);

        pCircle->set_radius(cur_val);
        pCircle->move(-step, -step);
        pWindow->redraw();
    }
};

int main()
{
    Graph_lib::Window wnd (Point (100, 100), 600, 400, "Adrian Fabisiewicz (328935 % 8 == 7)");
    Spinner sp_radius(Point(wnd.x_max() / 2 - 50, 10), 100);

    Button btn_close(Point(10,10), 80, 20, "Close", cb_close);
    sp_radius.attach(wnd);
    wnd.attach(btn_close);
    gui_main();
    return 0;
}
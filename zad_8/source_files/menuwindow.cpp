#include "menuwindow.h"

using namespace std;

MenuHeader::MenuHeader(Graph_lib::Point loc, int width, int height, const string& label)
    : Graph_lib::Button(loc, width, height, label, cb_openclose)
{

}

MenuHeader::~MenuHeader()
{
    pWindow->detach(*this);
    for (size_t idx = 0; idx < buttons.size(); ++idx)
    {
        pWindow->detach(*buttons[idx]);
        delete buttons[idx];
    }
}

void MenuHeader::showMenu()
{
    for (auto ptr : buttons)
        ptr->show();
    is_open = true;
}

void MenuHeader::hideMenu()
{
    for (auto ptr : buttons)
        ptr->hide();
    is_open = false;
}

void MenuHeader::attach(MenuWindow* pWnd, const std::vector<colorSpec>& btns)
{
    pWindow = pWnd;
    pWindow->attach(*this);
    Graph_lib::Point xy = this->loc;
    for (size_t idx = 0; idx < btns.size(); ++idx)
    {
        xy.y += 20;
        buttons.push_back(new Graph_lib::Button(xy, 80, 20, btns[idx].label, nullptr));
        pWindow->attach(*buttons.back());
    }
    hideMenu();
    pw->callback(reinterpret_cast<Fl_Callback*>(cb_openclose), this);
    cout << "Menu button address:" << this << '\n';
}

void MenuHeader::cb_openclose (Graph_lib::Address, Graph_lib::Address pMenu)
{
    cout << "menu header pressed with pMenu == "<< pMenu << '\n';
    MenuHeader *pHeader = reinterpret_cast<MenuHeader *> (pMenu);
    pHeader->is_open ? pHeader->hideMenu() : pHeader->showMenu();
};

std::vector<colorSpec> MenuWindow::fill_colors{
    { "No fill", Graph_lib::Color::invisible },
    { "Red", Graph_lib::Color::red },
    { "Green", Graph_lib::Color::green },
    { "Blue", Graph_lib::Color::blue },
};


MenuWindow::MenuWindow(Graph_lib::Point loc, int width, int height, const string& title)
    : Graph_lib::Window(loc, width, height, title),
    btn_close(Graph_lib::Point(x_max() - 80, y_max() - 20), 80, 20, "Close", cb_close),
    rect(Graph_lib::Point(100, 100), width - 200, height - 200),
    menu_fill(Graph_lib::Point(50,50), 80, 20, "Fill")
{
    attach(btn_close);
    attach(rect);
    menu_fill.attach(this, fill_colors);
};

MenuWindow::~MenuWindow()
{

};

void MenuWindow::cb_close (Graph_lib::Address, Graph_lib::Address pWnd)
{
    reinterpret_cast<MenuWindow *> (pWnd)->hide();
};
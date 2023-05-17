#pragma once
#include <Window.h>
#include <GUI.h>

class MenuWindow;
class MenuHeader;
struct actionDescriptor
{
    enum Action { NoAction, Menu_open, Menu_close, Menu_select };
    MenuWindow *pParent = nullptr;
    MenuHeader *pMenu = nullptr;
    Action menu_action = NoAction;
    Graph_lib::Color selected_color = Graph_lib::Color::invisible;
};

class MenuItem : public Graph_lib::Button
{
public:
    MenuItem(Graph_lib::Point loc, int w, int h,
    const std::string& label);
    void attach(MenuWindow* pWnd, MenuHeader *pMenu,
    Graph_lib::Color color, Graph_lib::Callback cb_setColor);
private:
    actionDescriptor buttonAction;
};

struct colorSpec
{
    std::string label;
    Graph_lib::Color color;
};

class MenuHeader : public Graph_lib::Button
{
    MenuWindow *pWindow = nullptr;
    std::vector<Graph_lib::Button*> buttons;
    bool is_open = false;
    actionDescriptor mAction = { nullptr, nullptr, actionDescriptor::NoAction, Graph_lib::Color::invisible };

    static void cb_openclose (Graph_lib::Address, Graph_lib::Address pMenu);
public:
    MenuHeader(Graph_lib::Point loc, int width, int height, const string& label);
    ~MenuHeader();

    void showMenu();
    void hideMenu();

    void attach(MenuWindow* pWnd, const std::vector<colorSpec>& btns);
};

class MenuWindow : public Graph_lib::Window
{
    Graph_lib::Button btn_close;
    Graph_lib::Rectangle rect;
    MenuHeader menu_fill;

    static void cb_close (Graph_lib::Address, Graph_lib::Address pWnd);
    static std::vector<colorSpec> fill_colors;
public:
    MenuWindow(Graph_lib::Point loc, int width, int height, const string& title);
    ~MenuWindow();

    void menuAction(actionDescriptor* pAD);
};
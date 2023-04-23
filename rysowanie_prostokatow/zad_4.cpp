// Adrian Fabisiewicz (328935)

#include "Simple_window.h"

using namespace Graph_lib;

ostream& operator<<(ostream& os, const Point& pt)
{
    return os << '(' << pt.x << ',' << pt.y << ')';
}

ostream& operator<<(ostream &os, const Graph_lib::Rectangle& rect)
{
    Point top_left = rect.point(0);
    int bottom_right_x = top_left.x + rect.width();
    int bottom_right_y = top_left.y + rect.height();
    Point bottom_right = Point(bottom_right_x, bottom_right_y);
    return os << "Rect (" << top_left << bottom_right << ")" << endl; 
}

class DrawingArea : public Graph_lib::Rectangle
{
    Point tl, br;
public:
    DrawingArea (Point tl, Point br) : Graph_lib::Rectangle(tl, br), tl(tl), br(br) {};
    bool contains (Point point)
    {
        bool contains_x = point.x >= tl.x && point.x <= br.x;
        bool contains_y = point.y >= tl.y && point.y <= br.y;
        return contains_x && contains_y;
    };

    Point limit (Point point)
    {
        int left_limit = max(point.x, tl.x);
        point.x = min(left_limit, br.x);

        int upper_limit = max(point.y, tl.y);
        point.y = min(upper_limit, br.y);

        return point;
    }
};

class rectWindow : public Graph_lib::Window
{
    Button btn_close, btn_save;
    bool drawingNow = false;
    DrawingArea canvas;
    Point start;
    Graph_lib::Rectangle *pRect;
    vector<Shape *> shapes;
    
    static void cb_close (Address, Address pWnd)
    {
        reinterpret_cast<rectWindow *> (pWnd)->hide();
    };
    
    static void cb_save (Address, Address pWnd)
    {
        reinterpret_cast<rectWindow *> (pWnd)->save();
    };

    Point event_xy() const
    {
        return Point(Fl::event_x(), Fl::event_y());
    };

public:
    rectWindow (Point loc, int w, int h, const string& title):
    Graph_lib::Window(loc, w, h, title),
    btn_close(Point(x_max() - 80, y_max() - 20), 80, 20,
        "Close", rectWindow::cb_close),
    btn_save(Point(x_max() - 80, y_max() - 50), 80, 20,
        "Save", rectWindow::cb_save),
    drawingNow(false),
    canvas(Point(10,10), Point(x_max()-100, y_max()-10)),
    start(0, 0), pRect(nullptr)
        {
            attach(btn_close);
            attach(btn_save);
        };

    void start_drawing(Point start)
    {
        this->start = start;
        drawingNow = true;
    };

    int handle (int event)
    {
        switch (event)
        {
            case FL_PUSH: // nacisniecie lewego przycisku myszy
                if (canvas.contains(event_xy()))
                {
                    start_drawing(event_xy());
                }
                break;
            case FL_DRAG: // "wleczenie" myszy
                if (drawingNow)
                {
                    draw(event_xy());
                }
                break;
            case FL_RELEASE: // zwolnienie lewego przycisku myszy
                if (drawingNow)
                {
                    stop_drawing();
                }
                break;
        }
        return Fl_Window::handle(event);
    }

    void draw(Point stop)
    {
        if (start.x == stop.x || start.y == stop.y) return;
        
        stop = canvas.limit(stop);
        
        if (pRect)
        {
            detach(*pRect);
            delete pRect;
        }

        Point top_left = Point(min(start.x, stop.x), min(start.y, stop.y));
        Point bottom_right = Point(max(start.x, stop.x), max(start.y, stop.y));
        pRect = new Graph_lib::Rectangle(top_left, bottom_right);
        attach(*pRect);
        redraw();
    }

    void stop_drawing()
    {
        drawingNow = false;
        shapes.push_back(pRect);
        pRect = nullptr;
        delete pRect;
        redraw();
    }

    void save()
    {
        ofstream o("rect.txt");
        for (auto shape : shapes)
        {
             if (auto rect = dynamic_cast<Graph_lib::Rectangle *>(shape))
             {
                 o << *rect;
             }
             else 
                throw runtime_error("unknown shape");
        }
    }
};

int main()
{
    rectWindow wnd (Point(100, 100), 600, 400, "Adrian Fabisiewicz (328935)");
    gui_main();
    return 0;
}




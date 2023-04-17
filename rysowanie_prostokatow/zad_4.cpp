// Adrian Fabisiewicz (328935)

#include "Simple_window.h"

using namespace Graph_lib;

ostream& operator<<(ostream& os, const Point& pt)
{
    return os << '(' << pt.x << ',' << pt.y << ')';
}

class DrawingArea : public Shape
{
    Point tl, br;
public:
    DrawingArea (Point tl, Point br) : tl(tl), br(br) {};
    bool contains (Point point)
    {
        return point.x >= tl.x && point.x <= br.x && point.y >= tl.y && point.y <= br.y;
    }
};

class rectWindow : public Graph_lib::Window
{
    Button btn_close, btn_save;
    bool drawingNow = false;
    DrawingArea canvas;
    Point start, stop;
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

    void start_drawing(Point start)
    {
        this->start = start;
        drawingNow = true;
    }

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
                    drawingNow = false;
                    stop = event_xy();
                    if (pRect != nullptr)
                    {
                        detach(*pRect);
                        shapes.push_back(pRect);
                        pRect = nullptr;
                        redraw();
                    }
                }
                break;
        }
        return Fl_Window::handle(event);
    }

    
    void save()
    {
        ofstream ofs("rects.txt");
        for (auto p : shapes)
        {
            ofs << p->point(0).x << ' ' << p->point(0).y << ' '
                << p->point(1).x << ' ' << p->point(1).y << endl;
        }
    }

    Point event_xy() const
    {
        return Point(Fl::event_x(), Fl::event_y());
    }

public:
    rectWindow (Point loc, int w, int h, const string& title)
    : Window(loc, w, h, title),
    btn_close(Point(x_max() - 80, y_max() - 20), 80, 20,
        "Close", rectWindow::cb_close),
    btn_save(Point(x_max() - 80, y_max() - 50), 80, 20,
        "Save", rectWindow::cb_save),
    drawingNow(false),
    canvas(Point(10,10), Point(x_max()-100, y_max()-10)),
    start(0, 0), stop(0, 0), pRect(nullptr)
        {
            attach(btn_close);
            attach(btn_save);
        };

    void draw(Point stop)
    {
        this->stop = stop;

        if (start.x == stop.x || start.y == stop.y)
            return;
        
        if (pRect)
        {
            cout << "detach" << endl;
            detach(*pRect);
        }

        Point top_left = Point(min(start.x, stop.x), min(start.y, stop.y));
        Point bottom_right = Point(max(start.x, stop.x), max(start.y, stop.y));
        cout << "top_left: " << top_left << ", bottom_right: " << bottom_right << endl;

        pRect = new Graph_lib::Rectangle(top_left, bottom_right);
        attach(*pRect);
        redraw();
        return;
    }

};

void cb_quit (Address, Address pw) { reinterpret_cast<rectWindow *>(pw)->hide(); }

int main()
{
    rectWindow wnd (Point(100, 100), 600, 400, "Adrian Fabisiewicz (328935)");
    gui_main();
    return 0;
}




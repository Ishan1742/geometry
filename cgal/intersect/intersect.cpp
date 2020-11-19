#include <iostream>
#include <fstream>

#include <QtGui>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPen>

#include <CGAL/Qt/GraphicsViewNavigation.h>
#include <CGAL/Cartesian.h>
#include <CGAL/Point_2.h>

typedef CGAL::Cartesian<double> Kernel;
typedef Kernel::Point_2 Point_2;
typedef Kernel::Segment_2 Segment_2;

const double EPS = 1E-9;
struct point
{
    double x, y;

    bool operator<(const point &q) const
    {
        return x < q.x - EPS || (abs(x - q.x) < EPS && y < q.y - EPS);
    }
};

struct line
{
    double a, b, c;

    line() {}

    line(point p, point q)
    {
        a = p.y - q.y;
        b = q.x - p.x;
        c = -a * p.x - b * p.y;
        // convert to hesse normal form
        norm();
    }

    void norm()
    {
        double z = std::sqrt(a * a + b * b);
        if (std::abs(z) > EPS)
        {
            a /= z;
            b /= z;
            c /= z;
        }
    }

    double dist(point q) const
    {
        return a * q.x + b * q.y + c;
    }
};

double det(double a, double b, double c, double d)
{
    return a * d - b * c;
}

inline bool betw(double l, double r, double x)
{
    return std::min(l, r) <= x + EPS && x < std::max(l, r) + EPS;
}

inline bool intersect_1d(double a, double b, double c, double d)
{
    if (a > b)
    {
        std::swap(a, b);
    }
    if (c > d)
    {
        std::swap(c, d);
    }
    return std::max(a, c) <= std::min(b, d) + EPS;
}

bool intersect(point a, point b, point c, point d, point &left, point &right)
{
    if (!intersect_1d(a.x, b.x, c.x, d.x) || !intersect_1d(a.y, b.y, c.y, d.y))
    {
        return false;
    }

    line m(a, b);
    line n(c, d);
    double zn = det(m.a, m.b, n.a, n.b);
    if (std::abs(zn) < EPS)
    {
        if (std::abs(m.dist(c)) > EPS || std::abs(n.dist(a)) > EPS)
        {
            return false;
        }
        if (b < a)
        {
            std::swap(a, b);
        }
        if (d < c)
        {
            std::swap(c, d);
        }
        left = std::max(a, c);
        right = std::min(b, d);
        return true;
    }
    else
    {
        left.x = right.x = -det(m.c, m.b, n.c, n.b) / zn;
        left.y = right.y = -det(m.a, m.c, n.a, n.c) / zn;
        return betw(a.x, b.x, left.x) && betw(a.y, b.y, left.y) &&
               betw(c.x, d.x, left.x) && betw(c.y, d.y, left.y);
    }
}

int main(int argc, char **argv)
{
    std::fstream file("../input.txt", std::ios::in);
    if (file.fail())
    {
        std::cout << "File not found" << std::endl;
        return 0;
    }

    QApplication app(argc, argv);

    std::vector<std::pair<point, point>> segments;
    point p, q;
    while (file >> p.x >> p.y >> q.x >> q.y)
    {
        segments.push_back(std::make_pair(p, q));
    }

    for (int i = 0; i < segments.size(); i++)
    {
        for (int j = i + 1; j < segments.size(); j++)
        {
            point a = segments[i].first;
            point b = segments[i].second;
            point c = segments[j].first;
            point d = segments[j].second;
            point left;
            point right;

            bool result = intersect(a, b, c, d, left, right);

            QGraphicsScene scene;
            scene.setSceneRect(0, 0, 700, 700);
            p = segments[i].first;
            q = segments[i].second;
            scene.addLine(QLineF(p.x, p.y, q.x, q.y));
            p = segments[j].first;
            q = segments[j].second;
            scene.addLine(QLineF(p.x, p.y, q.x, q.y));

            std::string str = "";
            if (result)
            {
                if ((std::abs(left.x - right.x) <= EPS) && (std::abs(left.y - right.y) <= EPS))
                {
                    str = std::to_string(i + 1) + " - " + std::to_string(j + 1) +
                          "\nSegments intersect at a point\n" +
                          "Segments intersect at: (" + std::to_string(left.x) +
                          "," + std::to_string(left.y) + ")\n";
                }
                else
                {
                    str = std::to_string(i + 1) + " - " + std::to_string(j + 1) +
                          "\nSegments intersect at a segment\n" +
                          "Segments intersect at segment: \nsource: (" +
                          std::to_string(left.x) + "," + std::to_string(left.y) +
                          ")\ntarget: (" + std::to_string(right.x) + "," +
                          std::to_string(right.y) + ")\n";
                }
            }
            else
            {
                str = std::to_string(i + 1) + " - " +
                      std::to_string(j + 1) + "\nSegments do not intersect\n";
            }

            QGraphicsTextItem *text = scene.addText(str.c_str());
            text->setPos(350, 550);
            text->setFlag(QGraphicsItem::ItemIsMovable);
            QGraphicsView *view = new QGraphicsView(&scene);
            CGAL::Qt::GraphicsViewNavigation navigation;
            view->installEventFilter(&navigation);
            view->viewport()->installEventFilter(&navigation);
            view->setRenderHint(QPainter::Antialiasing);
            view->show();
            app.exec();
            delete view;
            delete text;
        }
    }

    return 0;
}

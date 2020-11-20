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
#include <CGAL/Triangle_2.h>
#include <CGAL/Polygon_2.h>

typedef CGAL::Cartesian<double> Kernel;
typedef Kernel::Point_2 Point_2;
typedef CGAL::Triangle_2<Kernel> Triangle_2;
typedef CGAL::Polygon_2<Kernel> Polygon_2;

struct vertex
{
    double x, y;
    bool isEar;
};

int findArea(Point_2 a, Point_2 b, Point_2 c)
{
    return (b.x() - a.x()) * (c.y() - a.y()) - (c.x() - a.x()) * (b.y() - a.y());
}

bool left(Point_2 a, Point_2 b, Point_2 c)
{
    return findArea(a, b, c) > 0;
}

bool leftOn(Point_2 a, Point_2 b, Point_2 c)
{
    return findArea(a, b, c) >= 0;
}

bool collinear(Point_2 a, Point_2 b, Point_2 c)
{
    return findArea(a, b, c) == 0;
}

bool isPropIntersection(Point_2 a, Point_2 b, Point_2 c, Point_2 d)
{
    if (collinear(a, b, c) || collinear(a, b, d) || collinear(c, d, a) || collinear(c, d, b))
    {
        return false;
    }

    return (left(a, b, c) ^ left(a, b, d)) && (left(c, d, a) ^ left(c, d, b));
}

bool isBetween(Point_2 a, Point_2 b, Point_2 c)
{
    Point_2 ba, ca;
    if (!collinear(a, b, c))
    {
        return false;
    }
    if (a.x() != b.x())
    {
        return ((a.x() <= c.x() && c.x() <= b.x()) ||
                (a.x() >= c.x() && c.x() >= b.x()));
    }
    else
    {
        return ((a.y() <= c.y() && c.y() <= b.y()) ||
                (a.y() >= c.y() && c.y() >= b.y()));
    }
}

bool doesIntersect(Point_2 a, Point_2 b, Point_2 c, Point_2 d)
{
    if (isPropIntersection(a, b, c, d))
        return true;
    else if (isBetween(a, b, c) || isBetween(a, b, d) || isBetween(c, d, a) || isBetween(c, d, b))
        return true;
    else
        return false;
}

bool diagonalie(int a, int b, std::vector<vertex> &vertices)
{
    int n = vertices.size();
    Point_2 ap = Point_2(vertices[a].x, vertices[a].y);
    Point_2 bp = Point_2(vertices[b].x, vertices[b].y);

    for (int c = 0; c < n; c++)
    {
        int d = c == n - 1 ? 0 : c + 1;
        Point_2 cp = Point_2(vertices[c].x, vertices[c].y);
        Point_2 dp = Point_2(vertices[d].x, vertices[d].y);

        if (cp.operator!=(ap) && dp.operator!=(ap) &&
            cp.operator!=(bp) && dp.operator!=(bp) &&
            doesIntersect(ap, bp, cp, dp))
        {
            return false;
        }
    }
    return true;
}

bool inCone(int a, int b, std::vector<vertex> &vertices)
{
    int n = vertices.size();
    Point_2 ap = Point_2(vertices[a].x, vertices[a].y);
    Point_2 bp = Point_2(vertices[b].x, vertices[b].y);
    int a1 = a == n - 1 ? 0 : a + 1;
    int a0 = a == 0 ? n - 1 : a - 1;
    Point_2 a1p = Point_2(vertices[a1].x, vertices[a1].y);
    Point_2 a0p = Point_2(vertices[a0].x, vertices[a0].y);

    if (leftOn(ap, a1p, a0p))
    {
        return (left(ap, bp, a0p) && left(bp, ap, a1p));
    }
    else
    {
        return !(leftOn(ap, bp, a1p) && leftOn(bp, ap, a0p));
    }
}

bool isDiagonal(int a, int b, std::vector<vertex> &vertices)
{
    return inCone(a, b, vertices) && inCone(b, a, vertices) && diagonalie(a, b, vertices);
}

void earInit(std::vector<vertex> &vertices)
{
    int n = vertices.size();
    for (int v1 = 0; v1 < n; v1++)
    {
        int v2 = v1 == n - 1 ? 0 : v1 + 1;
        int v0 = v1 == 0 ? n - 1 : v1 - 1;
        vertices[v1].isEar = isDiagonal(v0, v2, vertices);
    }
}

void triangulate(std::vector<vertex> &vertices, QGraphicsScene &scene)
{
    QPen pen;
    pen.setColor(Qt::black);
    char colors[][10] = {"red", "green", "blue", "cyan", "magenta", "yellow", "darkGray", "darkGreen", "darkRed", "darkCyan"};

    int color = 0;
    int n = vertices.size();
    earInit(vertices);
    while (n > 3)
    {
        for (int v2 = 0; v2 < n; v2++)
        {
            if (vertices[v2].isEar)
            {
                int v3 = v2 == n - 1 ? 0 : v2 + 1;
                int v4 = v3 == n - 1 ? 0 : v3 + 1;
                int v1 = v2 == 0 ? n - 1 : v2 - 1;
                int v0 = v1 == 0 ? n - 1 : v1 - 1;

                QPolygonF polygon;
                polygon << QPointF(vertices[v1].x, vertices[v1].y);
                polygon << QPointF(vertices[v2].x, vertices[v2].y);
                polygon << QPointF(vertices[v3].x, vertices[v3].y);
                scene.addPolygon(polygon, pen, QBrush(colors[color % 10]));
                color++;

                vertices[v1].isEar = isDiagonal(v0, v3, vertices);
                vertices[v3].isEar = isDiagonal(v1, v4, vertices);

                vertices.erase(vertices.begin() + v2);
                n--;
                break;
            }
        }
    }

    QPolygonF polygon;
    polygon << QPointF(vertices[0].x, vertices[0].y);
    polygon << QPointF(vertices[1].x, vertices[1].y);
    polygon << QPointF(vertices[2].x, vertices[2].y);
    scene.addPolygon(polygon, pen, QBrush(colors[color % 10]));
}

int main(int argc, char **argv)
{
    std::fstream file("../input.txt", std::ios::in);

    if (file.fail())
    {
        std::cout << "File not found" << std::endl;
        return 0;
    }

    Point_2 p;
    QApplication app(argc, argv);

    QGraphicsScene scene;
    scene.setSceneRect(0, 0, 700, 700);

    QPolygonF polygon;
    std::set<Point_2> duplicates;
    std::vector<Point_2> points;
    while (file >> p)
    {
        polygon << QPointF(p.x(), p.y());
        points.push_back(p);
        if (duplicates.insert(p).second == false)
        {
            std::cout << "Duplicate points!\n";
            exit(EXIT_FAILURE);
        }
    }
    // reverse the points
    std::reverse(points.begin(), points.end());

    if (points.size() < 3)
    {
        std::cout << "Input not a polygon!\n";
        exit(EXIT_FAILURE);
    }

    Polygon_2 pgn(points.begin(), points.end());
    if (!pgn.is_simple())
    {
        std::cout << "Polygon is not simple!\n";
        exit(EXIT_FAILURE);
    }

    std::vector<vertex> vertices;
    for (Point_2 p : points)
    {
        vertex v;
        v.x = p.x();
        v.y = p.y();
        vertices.push_back(v);
    }

    triangulate(vertices, scene);

    // scene.addPolygon(polygon);
    QGraphicsView *view = new QGraphicsView(&scene);
    CGAL::Qt::GraphicsViewNavigation navigation;
    view->installEventFilter(&navigation);
    view->viewport()->installEventFilter(&navigation);
    view->setRenderHint(QPainter::Antialiasing);

    view->show();
    return app.exec();
}

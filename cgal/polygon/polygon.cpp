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
#include <CGAL/Polygon_2.h>
#include <CGAL/draw_polygon_2.h>

typedef CGAL::Cartesian<double> Kernel;
typedef Kernel::Point_2 Point_2;
typedef CGAL::Polygon_2<Kernel> Polygon_2;

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

    double area = pgn.area();
    std::string str = "Area is: " + std::to_string(abs(area)) + "\n";

    scene.addPolygon(polygon);
    QGraphicsTextItem *text = scene.addText(str.c_str());
    text->setPos(500, 625);
    text->setFlag(QGraphicsItem::ItemIsMovable);
    QGraphicsView *view = new QGraphicsView(&scene);
    CGAL::Qt::GraphicsViewNavigation navigation;
    view->installEventFilter(&navigation);
    view->viewport()->installEventFilter(&navigation);
    view->setRenderHint(QPainter::Antialiasing);

    view->show();
    return app.exec();
}

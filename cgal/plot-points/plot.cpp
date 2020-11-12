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

int main(int argc, char **argv)
{
    Point_2 p;
    std::fstream file("../input.txt", std::ios::in);

    if (file.fail())
    {
        std::cout << "File not found" << std::endl;
        return 0;
    }

    QApplication app(argc, argv);
    QGraphicsScene scene;
    QGraphicsTextItem text;
    scene.setSceneRect(0, 0, 700, 700);

    QPen pen;
    pen.setColor(Qt::darkCyan);

    while (file >> p)
    {
        scene.addEllipse(p.x(), p.y(), 5, 5, pen, QBrush(Qt::darkCyan));
    }

    QGraphicsView *view = new QGraphicsView(&scene);
    CGAL::Qt::GraphicsViewNavigation navigation;
    view->installEventFilter(&navigation);
    view->viewport()->installEventFilter(&navigation);
    view->setRenderHint(QPainter::Antialiasing);

    view->show();
    return app.exec();
}

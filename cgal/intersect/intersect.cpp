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
#include <CGAL/intersections.h>

typedef CGAL::Cartesian<double> Kernel;
typedef Kernel::Point_2 Point_2;
typedef Kernel::Segment_2 Segment_2;
typedef Kernel::Intersect_2 Intersect_2;

int main(int argc, char **argv)
{
    Point_2 p, q;
    std::fstream file("../input.txt", std::ios::in);

    if (file.fail())
    {
        std::cout << "File not found" << std::endl;
        return 0;
    }

    QApplication app(argc, argv);

    std::vector<Segment_2> segments;
    while (file >> p >> q)
    {
        segments.push_back(Segment_2(p, q));
    }

    for (int i = 0; i < segments.size(); i++)
    {
        for (int j = i + 1; j < segments.size(); j++)
        {
            auto result = intersection(segments[i], segments[j]);

            QGraphicsScene scene;
            scene.setSceneRect(0, 0, 700, 700);
            p = segments[i].source();
            q = segments[i].target();
            scene.addLine(QLineF(p.x(), p.y(), q.x(), q.y()));
            p = segments[j].source();
            q = segments[j].target();
            scene.addLine(QLineF(p.x(), p.y(), q.x(), q.y()));

            std::string str = "";
            if (result)
            {
                if (const Segment_2 *s = boost::get<Segment_2>(&*result))
                {
                    Point_2 i_source = s->source();
                    Point_2 i_target = s->target();
                    str = std::to_string(i + 1) + " - " +
                          std::to_string(j + 1) + "\nLines intersect\n" +
                          "Lines intersect at segment: \nsource: (" +
                          std::to_string(i_source.x()) + "," + std::to_string(i_source.y()) +
                          ")\ntarget: (" + std::to_string(i_target.x()) + "," +
                          std::to_string(i_target.y()) + ")\n";
                }
                else
                {
                    const Point_2 *ip = boost::get<Point_2>(&*result);
                    str = std::to_string(i + 1) + " - " +
                          std::to_string(j + 1) + "\nLines intersect\n" +
                          "Lines intersect at: (" + std::to_string(ip->x()) +
                          "," + std::to_string(ip->y()) + ")\n";
                }
            }
            else
            {
                str = std::to_string(i + 1) + " - " +
                      std::to_string(j + 1) + "\nLines do not intersect\n";
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

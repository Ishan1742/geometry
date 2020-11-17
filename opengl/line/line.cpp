#include <iostream>
#include <fstream>
#include <vector>

#include <GL/glut.h>

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.647059, 0.164706, 0.164706);

    std::fstream file("../input.txt", std::ios::in);
    if (file.fail())
    {
        std::cout << "File not found" << std::endl;
        exit(EXIT_FAILURE);
    }
    float p, q;
    std::vector<std::pair<float, float>> points;
    while (file >> p >> q)
    {
        points.push_back(std::make_pair(p, q));
        points.push_back(std::make_pair(p, q));
    }
    if (points.size() > 0)
    {
        points.erase(points.begin());
    }
    if (points.size() > 0)
    {
        points.pop_back();
    }

    glBegin(GL_LINES);
    for (auto it : points)
    {
        glVertex2f(it.first, it.second);
    }
    glEnd();
    glFlush();
}

void myinit()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(5.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 0.99, -1.0, 0.99);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Points");
    glutDisplayFunc(display);

    myinit();
    glutMainLoop();
    return 0;
}

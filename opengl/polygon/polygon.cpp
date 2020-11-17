#include <iostream>
#include <fstream>
#include <vector>

#include <GL/glut.h>

void display()
{
    std::fstream file("../input.txt", std::ios::in);
    if (file.fail())
    {
        std::cout << "File not found" << std::endl;
        exit(EXIT_FAILURE);
    }

    float a, b, c;
    std::vector<float> x;
    std::vector<float> y;
    std::vector<float> z;
    while (file >> a >> b >> c)
    {
        x.push_back(a);
        y.push_back(b);
        z.push_back(c);
    }

    glBegin(GL_QUADS);
    glColor3f(1, 0, 0);
    for (int i = 0; i < 4; i++)
    {
        glVertex3f(x[i], y[i], z[i]);
    }
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0, 1, 0);
    for (int i = 4; i < 8; i++)
    {
        glVertex3f(x[i], y[i], z[i]);
    }
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0, 0, 1);
    for (int i = 8; i < 12; i++)
    {
        glVertex3f(x[i], y[i], z[i]);
    }
    glEnd();

    glFlush();
}

void myinit()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluOrtho2D(-1.0, 0.99, -1.0, 0.99);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Points");
    glutDisplayFunc(display);
    myinit();
    glutMainLoop();
    return 0;
}

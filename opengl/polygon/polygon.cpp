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
    glVertex3f(x[0], y[0], z[0]);
    glVertex3f(x[1], y[1], z[1]);
    glVertex3f(x[2], y[2], z[2]);
    glVertex3f(x[3], y[3], z[3]);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0, 1, 0);
    glVertex3f(x[4], y[4], z[4]);
    glVertex3f(x[5], y[5], z[5]);
    glVertex3f(x[6], y[6], z[6]);
    glVertex3f(x[7], y[7], z[7]);
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0, 0, 1);
    glVertex3f(x[8], y[8], z[8]);
    glVertex3f(x[9], y[9], z[9]);
    glVertex3f(x[10], y[10], z[10]);
    glVertex3f(x[11], y[11], z[11]);
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

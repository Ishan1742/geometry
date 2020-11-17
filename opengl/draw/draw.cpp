#include <iostream>
#include <fstream>
#include <limits>
#include <vector>

#include <GL/glut.h>

int mouseDrag = 1;
int newLine = 0;
int prevx = INT32_MIN;
int prevy = INT32_MIN;
std::vector<std::pair<int, int>> line;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

void myinit()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3f(0.0, 0.0, 0.0);
    glPointSize(5.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 499.0, 0.0, 499.0);
}

void mouseClicked(int button, int state, int mousex, int mousey)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        mouseDrag = 1;
    }
    else
    {
        mouseDrag = 0;
        line.clear();
    }
}

void mouseMoved(int mousex, int mousey)
{
    if (mouseDrag == 1)
    {
        glColor3f(0.0, 0.0, 0.0);
        GLint x = mousex;
        GLint y = 500 - mousey; // screenheight is fixed
        line.push_back(std::make_pair(x, y));
    }
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(3.0f);
    glBegin(GL_LINE_STRIP);
    for (auto &point : line)
    {
        glVertex2f(point.first, point.second);
    }
    glEnd();
    glFlush();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Points");
    glutDisplayFunc(display);
    glutMouseFunc(mouseClicked);
    glutMotionFunc(mouseMoved);

    myinit();
    glutMainLoop();
    return 0;
}

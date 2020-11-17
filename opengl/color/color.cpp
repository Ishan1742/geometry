#include <iostream>
#include <fstream>
#include <GL/glut.h>

void renderFunction()
{
    std::fstream file("../input.txt", std::ios::in);
    if (file.fail())
    {
        std::cout << "File not found" << std::endl;
        exit(EXIT_FAILURE);
    }

    float r, g, b, a;
    file >> r >> g >> b >> a;
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(700, 700);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Background Color");
    glutDisplayFunc(renderFunction);
    glutMainLoop();
    return 0;
}

/**
 * @file main.c
 * @author Bruno Dias (bruno@beveldrive.com.br)
 * @brief Learning how to create a raycaster in C and OpenGL
 * @version 0.1
 * @date 2023-04-16
 * 
 * @copyright Copyright (c) 2023
 * 
 */

// OpenGL Libs
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/freeglut.h>

// Libs from C
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Screen Definition
#define WIDTH 1024
#define HEIGHT 512
#define PI 3.1415926535

float px;   // Player x position
float py;   // Player y position
float pdx;  // Player delta x position
float pdy;  // Player delta y position
float pa;   // Player angle

// Map Definition
int mapX = 8, mapY = 8, mapS = 64;
int map[] =
{
    1,1,1,1,1,1,1,1,
    1,0,1,0,0,0,0,1,
    1,0,1,0,0,0,0,1,
    1,0,1,0,0,0,0,1,
    1,0,0,0,0,0,0,1,
    1,0,0,0,0,1,0,1,
    1,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,
};

void drawMap2D()
{
    int x, y, xo, yo;
    for (y = 0; y < mapY; y++) 
    {
        for (x = 0; x < mapX; x++)
        {
            if (map[y*mapX+x] == 1) {
                glColor3f(1, 1, 1);
             } else {
                glColor3f(0, 0, 0);
             }
             xo = x * mapS;
             yo = y * mapS;
             glBegin(GL_QUADS);
             glVertex2i(xo + 1, yo + 1);
             glVertex2i(xo + 1, yo + mapS - 1);
             glVertex2i(xo + mapS - 1, yo + mapS - 1);
             glVertex2i(xo + mapS - 1, yo + 1);
             glEnd();
        }
    }
}

void drawPlayer() 
{
    glColor3f(1, 1, 0);
    glPointSize(8);
    glBegin(GL_POINTS);
    glVertex2i(px, py);
    glEnd();

    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2i(px, py);
    glVertex2i(px + pdx * 5, py + pdy * 5);
    glEnd();
}

void display() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawMap2D();
    drawPlayer();
    glutSwapBuffers();
}

void buttons(unsigned char key, int x, int y)
{
    if (key=='a') {
        pa -= 0.1;
        if (pa < 0)
        {
             pa += 2 * PI;
        }
        pdx = cos(pa) * 5;
        pdy = sin(pa) * 5;
    }
    if (key=='d') {
        pa += 0.1;
        if (pa > 2 * PI)
        {
             pa -= 2 * PI;
        }
        pdx = cos(pa) * 5;
        pdy = sin(pa) * 5;
    }
    if (key=='w') {
        px += pdx;
        py += pdy;
    }
    if (key == 's') {
        px -= pdx;
        py -= pdy;
    }
    glutPostRedisplay();
}

void init() 
{
    glClearColor(0.3, 0.3, 0.3, 0);
    gluOrtho2D(0, WIDTH, HEIGHT, 0);
    px = 300;
    py = 300;
}

int main(int argc, char* argv[]) 
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("BevelDrive - Test");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(buttons);
    glutMainLoop();
}
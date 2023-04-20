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

#include <GL/freeglut.h>
#include <stdlib.h>
#include <math.h>

// Screen Definition
#define WIDTH 1024
#define HEIGHT 510
#define PI 3.1415926535
#define P2 PI / 2
#define P3 3 * PI / 2
#define DR 0.0174533 // one degree in radians

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

// ------------------ Draw the Player ------------------------------

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

void buttons(unsigned char key, int x, int y)
{
    if (key == 'a')
    {
        pa -= 0.1;
        if (pa < 0)
        {
             pa += 2 * PI;
        }
        pdx = cos(pa) * 5;
        pdy = sin(pa) * 5;
    }
    if (key == 'd')
    {
        pa += 0.1;
        if (pa > 2 * PI)
        {
             pa -= 2 * PI;
        }
        pdx = cos(pa) * 5;
        pdy = sin(pa) * 5;
    }
    if (key == 'w')
    {
        px += pdx * 5;
        py += pdy * 5;
    }
    if (key == 's')
    {
        px -= pdx * 5;
        py -= pdy * 5;
    }
    glutPostRedisplay();
}

// ----------------- Draw Rays and Walls --------------------------

float dist(float ax, float ay, float bx, float by, float ang)
{
    return (sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)));
}

void drawRays2D()
{
    int r;
    int mx;
    int my;
    int mp;
    int dof;

    float rx;
    float ry;
    float ra;
    float xo;
    float yo;
    float disT;

    ra = pa - DR * 30;
    if (ra < 0)
    {
        ra += 2 * PI;
    }

    if (ra > 2 * PI)
    {
        ra -= 2 * PI;
    }

    for (r = 0; r < 60; r++) // Field of View (60)
    {
        // ---Check Horizontal Lines---
        dof = 0;
        float disH = 1000000;
        float hx = px;
        float hy = py;
        float aTan = -1 / tan(ra); // reverse tangent
        if (ra > PI)               // looking up
        {
             ry = (((int)py >> 6) << 6) - 0.0001; // this divides the value by 64 (bit-shifting on 6 bits) and multiplies by 64 (bit-shifting on 6 bits again)
             rx = (py - ry) * aTan + px;
             yo = -64;
             xo = -yo * aTan;
        }

        if (ra < PI) // looking down
        {
             ry = (((int)py >> 6) << 6) + 64; // this divides the value by 64 (bit-shifting on 6 bits) and multiplies by 64 (bit-shifting on 6 bits again)
             rx = (py - ry) * aTan + px;
             yo = 64;
             xo = -yo * aTan;
        }

        if (ra == 0 || ra == PI) // looking straight left or right
        {
             rx = px;
             ry = py;
             dof = 8;
        }

        while (dof < 8)
        {
             mx = (int)(rx) >> 6;
             my = (int)(ry) >> 6;
             mp = my * mapX + mx;
             if (mp > 0 && mp < mapX * mapY && map[mp] == 1) // wall hit
             {
                hx = rx;
                hy = ry;
                disH = dist(px, py, hx, hy, ra);
                dof = 8;
             }
             else // next line
             {
                rx += xo;
                ry += yo;
                dof += 1;
             }
        }

        // ---Check Vertical Lines---

        dof = 0;
        float disV = 1000000;
        float vx = px;
        float vy = py;
        float nTan = -tan(ra);  // reverse tangent
        if (ra > P2 && ra < P3) // looking left
        {
             rx = (((int)px >> 6) << 6) - 0.0001; // this divides the value by 64 (bit-shifting on 6 bits) and multiplies by 64 (bit-shifting on 6 bits again)
             ry = (px - rx) * nTan + py;
             xo = -64;
             yo = -xo * nTan;
        }

        if (ra < P2 || ra > P3) // looking right
        {
             rx = (((int)px >> 6) << 6) + 64; // this divides the value by 64 (bit-shifting on 6 bits) and multiplies by 64 (bit-shifting on 6 bits again)
             ry = (px - rx) * nTan + py;
             xo = 64;
             yo = -xo * nTan;
        }

        if (ra == 0 || ra == PI) // looking straight up or down
        {
             rx = px;
             ry = py;
             dof = 8;
        }

        while (dof < 8)
        {
             mx = (int)(rx) >> 6;
             my = (int)(ry) >> 6;
             mp = my * mapX + mx;
             if (mp > 0 && mp < mapX * mapY && map[mp] == 1) // wall hit
             {
                vx = rx;
                vy = ry;
                disV = dist(px, py, vx, vy, ra);
                dof = 8;
             }
             else // next line
             {
                rx += xo;
                ry += yo;
                dof += 1;
             }
        }

        if (disV < disH)
        {
             rx = vx;
             ry = vy;
             disT = disV;
             glColor3f(0.9, 0, 0);
        }

        if (disH < disV)
        {
             rx = hx;
             ry = hy;
             disT = disH;
             glColor3f(0.7, 0, 0);
        }

        glLineWidth(3);
        glBegin(GL_LINES);
        glVertex2i(px, py);
        glVertex2i(rx, ry);
        glEnd();

        // ---Draw 3D Walls---
        float ca = pa - ra;
        if (ca < 0)
        {
             ca += 2 * PI;
        }
        disT *= cos(ca); // fix fisheye camera

        float lineH = (mapS * 320) / disT; // line height
        if (lineH > 320)
        {
             lineH = 320;
        }

        float lineO = 160 - lineH / 2; // line offset

        glLineWidth(8);
        glBegin(GL_LINES);
        glVertex2i(r * 8 + 530, lineO);
        glVertex2i(r * 8 + 530, lineH + lineO);
        glEnd();

        ra += DR;
        if (ra < 0)
        {
             ra += 2 * PI;
        }

        if (ra > 2 * PI)
        {
             ra -= 2 * PI;
        }
    }
}

void display() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawMap2D();
    drawPlayer();
    drawRays2D();
    glutSwapBuffers();
}

void resize(int w, int h)
{
    glutReshapeWindow(1024, 512);
}

void init() 
{
    glClearColor(0.3, 0.3, 0.3, 0);
    gluOrtho2D(0, WIDTH, HEIGHT, 0);
    px = 150;
    py = 400;
    pa = 90;
    pdx = cos(pa);
    pdy = -sin(pa);
}

int main(int argc, char* argv[]) 
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("BevelDrive - Test");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutKeyboardFunc(buttons);
    glutMainLoop();
}
#include <stdio.h>
#include <math.h>
#include "wave.h"

GLuint quad[4*QUADNUM];
struct Vertex vertex[VERTEXNUM];

double dt;
double wave_strength = 20.0;
double animation_speed = 5.0;
// size of starting impression
double impression = 0.1;
double p[GRIDW][GRIDH];
double vx[GRIDW][GRIDH], vy[GRIDW][GRIDH];
double ax[GRIDW][GRIDH], ay[GRIDW][GRIDH];

// initialize grid geometry
void init_grid(void)
{
    int x, y;
    double dx, dy, d;

    for (y = 0; y < GRIDH; y++) {
        for (x = 0; x < GRIDW; x++) {
            dx = (double) (x - GRIDW / 2);
            dy = (double) (y - GRIDH / 2);
            d = sqrt(dx * dx + dy * dy);
            // if center
            if (d < impression * (double) (GRIDW / 2)) {
                d = d * 10.0;
                p[x][y] = -cos(d * (M_PI / (double)(GRIDW * 4))) * wave_strength;
            } else {
                p[x][y] = 0.0;
            }
            vx[x][y] = 0.0;
            vy[x][y] = 0.0;
        }
    }
}

void init_vertices(void)
{
    int x, y, p;

    // place the verticies in a grid
    for (y = 0; y < GRIDH; y++) {
        for (x = 0; x < GRIDW; x++) {
            p = y * GRIDW + x;

            vertex[p].x = (GLfloat) (x - GRIDW / 2) / (GLfloat) (GRIDW / 2);
            vertex[p].y = (GLfloat) (y - GRIDH / 2) / (GLfloat) (GRIDH / 2);
            vertex[p].z = 0;

            // set colors
            if ((x % 4 < 2) ^ (y % 4 < 2))
                vertex[p].r = 0.0;
            else
                vertex[p].r = 1.0;

            vertex[p].g = (GLfloat) y / (GLfloat) GRIDH;
            vertex[p].b = 1.f - ((GLfloat) x / (GLfloat) GRIDW + (GLfloat) y / (GLfloat) GRIDH / 2.f);
        }
    }

    for (y = 0; y < QUADH; y++) {
        for (x = 0; x < QUADW; x++) {
            p = 4 * (y * QUADW + x);

            quad[p + 0] = y * GRIDW + x; // some point
            quad[p + 1] = y * GRIDW + x + 1; // neighbor at the right side
            quad[p + 2] = (y + 1) * GRIDW + x + 1; // upper right neighbor
            quad[p + 3] = (y + 1) * GRIDW + x; // upper neighbor
        }
    }
}

// modify the height of each vertex according to the pressure
void adjust_grid(void)
{
    int pos;
    int x, y;
    for (y = 0; y < GRIDH; y++) {
        for (x = 0; x < GRIDW; x++) {
            pos = y * GRIDW + x;
            // decrease momentum over time
            p[x][y] = p[x][y] * 0.991;
            vertex[pos].z = (float) (p[x][y] * 0.02);
        }
    }
}

void calc_grid(void)
{
    int x, y, x2, y2;
    double time_step = dt * animation_speed;

    // compute accelerations
    for (x = 0; x < GRIDW; x++) {
        x2 = (x + 1) % GRIDW;
        for (y = 0; y < GRIDH; y++) {
            ax[x][y] = p[x][y] - p[x2][y];
        }
    }
    for (y = 0; y < GRIDH; y++) {
        y2 = (y + 1) % GRIDH;
        for (x = 0; x < GRIDW; x++) {
            ay[x][y] = p[x][y] - p[x][y2];
        }
    }

    // compute speed
    for (x = 0; x < GRIDW; x++) {
        for (y = 0; y < GRIDH; y++) {
            vx[x][y] = vx[x][y] + ax[x][y] * time_step;
            vy[x][y] = vy[x][y] + ay[x][y] * time_step;
        }
    }

    // compute pressure
    for (x = 1; x < GRIDW; x++) {
        x2 = x -1;
        for (y = 1; y < GRIDH; y++) {
            y2 = y -1;
            // calculate distance between current point and previous
            // then multiply by the time_step to slow it down
            p[x][y] = p[x][y] + (vx[x2][y] - vx[x][y] + vy[x][y2] - vy[x][y]) * time_step;
        }
    }
}


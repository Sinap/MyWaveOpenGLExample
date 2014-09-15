#ifndef craft_wave_h
#define craft_wave_h

#include <GLFW/glfw3.h>
#include "config.h"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

// plane size
#define GRIDW 50
#define GRIDH 50
#define VERTEXNUM (GRIDW*GRIDH)

#define QUADW (GRIDW - 1)
#define QUADH (GRIDH - 1)
#define QUADNUM (QUADW*QUADH)

struct Vertex {
    GLfloat x, y, z;
    GLfloat r, g, b;
};

extern double dt;
extern GLuint quad[4*QUADNUM];
extern struct Vertex vertex[VERTEXNUM];

extern double wave_strength;
extern double animation_speed;
extern double impression;

void init_vertices(void);
void init_grid(void);
void adjust_grid(void);
void calc_grid(void);

#endif

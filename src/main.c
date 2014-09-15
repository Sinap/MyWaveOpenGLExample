#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <math.h>
#include <GLFW/glfw3.h>

#include "wave.h"
#include "config.h"
#include "call_backs.h"

typedef struct {
    GLFWwindow *window;
    int width;
    int height;
    float scale;
} Window;

static Window window;
static Window *w = &window;

double pause_t = 0.0;
GLfloat alpha = 210.f;
GLfloat beta = -70.f;
GLfloat zoom = 2.5f;

void draw_scene(void)
{
    // clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // enable line anti-aliasing
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // we don't want to modify the projection matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // move back
    glTranslatef(0.0, 0.0, -zoom);

    // rotate the view
    glRotatef(beta, 1.0, 0.0, 0.0);
    glRotatef(alpha, 0.0, 0.0, 1.0);

    glDrawElements(GL_QUADS, 4 * QUADNUM, GL_UNSIGNED_INT, quad);

    glfwSwapBuffers(w->window);
}

void init_opengl(void)
{
    // use Gouraud (smooth) shading
    glShadeModel(GL_SMOOTH);

    // switch on the z-buffer
    glEnable(GL_DEPTH_TEST);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(struct Vertex), vertex);
    glColorPointer(3, GL_FLOAT, sizeof(struct Vertex), &vertex[0].r); // pointer to the first color

    glPointSize(2.0);

    // background color is black
    glClearColor(0, 0, 0, 0);
}

void create_window(void)
{
    int width = WINDOW_WIDTH;
    int height = WINDOW_HEIGHT;
    static const char *title = WINDOW_TITLE;
    w->window = glfwCreateWindow(width, height, title, NULL, NULL);
}

int main(int argc, char const *argv[])
{
    double t, dt_total, t_old;

    // initialize the library
    if (!glfwInit())
        exit(EXIT_FAILURE);

    // create a windowed mode window and its OpenGL context
    create_window();
    if (!w->window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // set all of our callback functions
    glfwSetCursorPosCallback(w->window, cursor_position_callback);
    glfwSetScrollCallback(w->window, scroll_callback);
    glfwSetMouseButtonCallback(w->window, mouse_button_callback);
    glfwSetKeyCallback(w->window, key_callback);
    glfwSetFramebufferSizeCallback(w->window, framebuffer_size_callback);
    glfwSetErrorCallback(error_callback);


    // make the window's context current
    glfwMakeContextCurrent(w->window);
    glfwSwapInterval(VSYNC);

    glfwGetFramebufferSize(w->window, &w->width, &w->height);
    framebuffer_size_callback(w->window, w->width, w->height);

    // initialize OpenGL
    init_opengl();

    // initialize timer
    t_old = glfwGetTime() - 0.01;

    // initialize simulation
    init_vertices();
    init_grid();
    adjust_grid();

    // main loop
    while (!glfwWindowShouldClose(w->window)) {

        // calculate elapsed time since last iteration
        t = glfwGetTime() - pause_t;
        dt_total = t - t_old;
        t_old = t;

        if (paused) {
            // keep track of time paused so we can resume the animation
            pause_t += glfwGetTime();
        } else {
            // safety - iterate if dt_total is too large
            while (dt_total > 0.f) {
                // select iteration time step
                dt = dt_total > MAX_DELTA_T ? MAX_DELTA_T : dt_total;
                dt_total -= dt;
                // calculate wave propagation
                calc_grid();
            }

            // compute height of each vertex
            adjust_grid();
        }

        // draw wave grid to OpenGL display
        draw_scene();

        // poll for and process events
        glfwPollEvents();
    }

    glfwDestroyWindow(w->window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

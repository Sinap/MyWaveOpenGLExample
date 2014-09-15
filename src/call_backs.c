#include <stdio.h>
#include <OpenGL/glu.h>
#include "call_backs.h"
#include "wave.h"

GLboolean locked = GL_FALSE;
GLboolean wireframe = GL_FALSE;
GLboolean paused = GL_FALSE;


void cursor_position_callback(GLFWwindow* window, double x, double y)
{
    // make sure left mouse button is being held down
    if (locked) {
        // only add what was changed and divide by 10 to slow movement
        alpha += (GLfloat) (x - cursorX) / 10.f;
        beta += (GLfloat) (y - cursorY) / 10.f;
    }
    
    // save cursors position for use as delta
    cursorX = (int) x;
    cursorY = (int) y;
}

void scroll_callback(GLFWwindow* window, double x, double y)
{
    zoom += (float) y / 4.f;
    if (zoom < 0)
        zoom = 0;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button != GLFW_MOUSE_BUTTON_LEFT)
        return;
    
    // hide cursor when pressing left mouse button down
    if (action == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        locked = GL_TRUE;
    } else {
        locked = GL_FALSE;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mod)
{
    if (action != GLFW_PRESS)
        return;
    
    switch (key) {
        // exit on escape
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        // exit on command+w
        case GLFW_KEY_W:
            if (mod == GLFW_MOD_SUPER)
                glfwSetWindowShouldClose(window, GL_TRUE);
            else
                wireframe = wireframe ? GL_FALSE : GL_TRUE;
            break;
        case GLFW_KEY_P:
            paused = paused ? GL_FALSE : GL_TRUE;
            break;
        // pan left
        case GLFW_KEY_I:
            if (mod == GLFW_MOD_SHIFT) {
                if (impression <= 0)
                    impression = 0.0;
                else
                    impression -= 0.1;
                init_grid();
            } else {
                impression += 0.1;
                init_grid();
            }
            break;
        case GLFW_KEY_LEFT:
            alpha += 5;
            break;
        // pan right
        case GLFW_KEY_RIGHT:
            alpha -= 5;
            break;
        // tilt up
        case GLFW_KEY_UP:
            beta += 5;
            break;
        // tilt down
        case GLFW_KEY_DOWN:
            beta -= 5;
            break;
        case GLFW_KEY_SPACE:
            init_grid();
            break;
        case GLFW_KEY_EQUAL:
            wave_strength += 10.0;
            init_grid();
            break;
        case GLFW_KEY_MINUS:
            wave_strength -= 10.0;
            init_grid();
            break;
        case GLFW_KEY_RIGHT_BRACKET:
            animation_speed += 1.0;
            break;
        case GLFW_KEY_LEFT_BRACKET:
            animation_speed -= 1.0;
            break;
        default:
            break;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    float ratio = 1.f;
    
    if (height > 0) {
        ratio = (float) width / (float) height;
    }
    
    // setup viewport
    glViewport(0, 0, width, height);
    
    // change to the projection matrix and set our viewing volume
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, ratio, 1.0, 1024.0);
    
}

void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

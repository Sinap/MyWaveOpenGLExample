#ifndef craft_call_backs_h
#define craft_call_backs_h

#include <GLFW/glfw3.h>

extern GLfloat alpha;
extern GLfloat beta;
extern GLfloat zoom;
extern GLboolean wireframe;
extern GLboolean paused;

int cursorX;
int cursorY;

void cursor_position_callback(GLFWwindow* window, double x, double y);
void scroll_callback(GLFWwindow* window, double x, double y);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mod);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void error_callback(int error, const char* description);

#endif

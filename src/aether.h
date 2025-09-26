#ifndef AETHER_H_
#define AETHER_H_

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct aether {
    GLFWwindow* window;
};

struct aether * aetherInit();
void aetherLoop(struct aether * aether);
void aetherFramebufferSizeCallback(GLFWwindow* window, int width, int height);

#endif
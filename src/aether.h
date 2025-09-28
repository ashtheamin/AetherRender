#ifndef AETHER_H_
#define AETHER_H_

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

struct aetherModelAttributes {
    int coord3d;
    int colour;
};

struct aetherModelUniforms {
    int view;
    int model;
    int projection;
};


struct aetherModel {
    struct aetherModelAttributes attributes;
    struct aetherModelUniforms uniforms;
    unsigned int vertexSize;
    unsigned int IBO;
    unsigned int VBO;
    unsigned int colours;
    unsigned int shader;
    struct aetherModel* next;
};

struct aether {
    GLFWwindow* window;
    struct aetherModel* models;
};

struct aether * aetherInit();
struct aetherModel * aetherModelInit();
void aetherModelFree(struct aetherModel* model);
void aetherInput(struct aether * aether);
void aetherLoop(struct aether * aether);
void aetherFree(struct aether * aether);
void aetherFramebufferSizeCallback(GLFWwindow* window, int width, int height);
unsigned int aetherShaderDefaultInit();
void aetherModelDefaultCubeInit(struct aetherModel* model);
#endif
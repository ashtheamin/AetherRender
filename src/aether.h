#ifndef AETHER_H_
#define AETHER_H_

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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

struct aetherMouse {
    float lastX;
    float lastY;
    float sensitivity;
    bool usedBefore;
};

struct aetherCamera {
    vec3 position;
    vec3 front;
    vec3 up;

    float speed;
    float yaw;
    float pitch;

    struct aetherMouse mouse;
};

struct aetherTiming {
    float deltaTime;
    float lastTime;
};

struct aether {
    GLFWwindow* window;
    struct aetherModel* models;
    struct aetherCamera camera;
    struct aetherTiming timing;
};

struct aether * aetherInit();
struct aetherModel * aetherModelInit();
void aetherModelFree(struct aetherModel* model);
void aetherInput(struct aether * aether);
void aetherLoop(struct aether * aether);
void aetherFree(struct aether * aether);
void aetherFramebufferSizeCallback(GLFWwindow* window, int width, int height);
void aetherMouseCallback(GLFWwindow* window, double x, double y);
unsigned int aetherShaderDefaultInit();
void aetherModelDefaultCubeInit(struct aetherModel* model);
void aetherUpdateTiming(struct aether* aether);
#endif
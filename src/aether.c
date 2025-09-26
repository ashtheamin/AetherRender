#include "aether.h"

void aetherFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

struct aether * aetherInit() {
    struct aether* aether = malloc(sizeof(struct aether));
    if (aether == NULL) return NULL;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    aether->window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Aether Renderer", NULL, NULL);
    if (aether->window == NULL) {
        free(aether);
        return NULL;
    }

    if (glewInit() != GLEW_OK) {
        free(aether);
        return NULL;
    }

    glfwMakeContextCurrent(aether->window);
    glfwSetFramebufferSizeCallback(aether->window, aetherFramebufferSizeCallback);
    return aether;
}

void aetherLoop(struct aether * aether) {
    while (!glfwWindowShouldClose(aether->window)) {
        
    }
}
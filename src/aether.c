#include "aether.h"

void aetherFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

struct aetherModel * aetherModelInit() {
    struct aetherModel* model = malloc(sizeof(struct aetherModel));
    if (model == NULL) return NULL;

    model->next = NULL;
    model->vertexData = NULL;
    model->VAO = 0;
    model->VBO = 0;
    model->shader = 0;

    return model;
}

void aetherModelFree(struct aetherModel* model) {
    if (model == NULL) return;

    struct aetherModel* temp = model;
    struct aetherModel* next = model;

    while (next != NULL) {
        next = temp->next;
        free(temp);
        temp = next;
    }
}

struct aether * aetherInit() {
    struct aether* aether = malloc(sizeof(struct aether));
    if (aether == NULL) return NULL;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    
    aether->window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Aether Renderer", NULL, NULL);
    if (aether->window == NULL) {
        printf("Failed to create GLFW Window. Exiting.\n");
        free(aether);
        return NULL;
    }

    glfwMakeContextCurrent(aether->window);
    glfwSetFramebufferSizeCallback(aether->window, aetherFramebufferSizeCallback);
    
    GLenum glewStatus = glewInit();
    if (glewStatus != GLEW_OK) {
        printf("%s\n", glewGetErrorString(glewStatus));
        printf("Failed to initialise GLEW. Exiting.\n");
        free(aether);
        return NULL;
    }

    aether->models = aetherModelInit();

    return aether;
}

void aetherInput(struct aether * aether) {
    if (glfwGetKey(aether->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(aether->window, GLFW_TRUE);
    }
}

void aetherLoop(struct aether * aether) {
    while (!glfwWindowShouldClose(aether->window)) {
        aetherInput(aether);
        glClearColor(0.1, 0.3, 0.4, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(aether->window);
        glfwPollEvents();
    }
}

void aetherFree(struct aether * aether) {
    if (aether == NULL) return;
    glfwDestroyWindow(aether->window);
    aetherModelFree(aether->models);
    free(aether);
    return;
}
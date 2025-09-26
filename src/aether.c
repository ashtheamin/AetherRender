#include "aether.h"

struct aether * aetherInit() {
    struct aether* aether = malloc(sizeof(struct aether));
    if (aether == NULL) return NULL;

    glewInit();

    aether->window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Aether Renderer", NULL, NULL);
    return aether;
}
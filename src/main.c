#include "aether.h"

int main(void) {
    struct aether* aether = aetherInit();
    if (aether == NULL) {
        printf("aetherInit() Failed. Exiting.\n");
        return -1;
    }
    aetherModelDefaultCubeInit(aether->models);
    aether->models->next = aetherModelInit();
    aetherModelDefaultCubeInit(aether->models->next);
    glm_vec3_copy((vec3){3.0, 3.0, 3.0}, aether->models->next->position);
    glm_vec3_copy((vec3){0.5, 0.5, 0.5}, aether->models->next->scale);
    aetherLoop(aether);
    aetherFree(aether);
    return 0;
}
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
    vec3 newPos = {3.0, 3.0, 3.0};
    glm_vec3_copy(newPos, aether->models->next->position);
    aetherLoop(aether);
    aetherFree(aether);
    return 0;
}
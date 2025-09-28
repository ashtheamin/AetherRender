#include "aether.h"

int main(void) {
    struct aether* aether = aetherInit();
    if (aether == NULL) {
        printf("aetherInit() Failed. Exiting.\n");
        return -1;
    }
    aetherModelDefaultCubeInit(aether->models);

    aetherLoop(aether);
    aetherFree(aether);
    return 0;
}
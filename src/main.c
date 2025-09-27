#include "aether.h"

float aetherDefaultTriangle[] = {
    0.0,  0.8,
    -0.8, -0.8,
    0.8, -0.8,
};

int main(void) {
    struct aether* aether = aetherInit();
    if (aether == NULL) {
        printf("aetherInit() Failed. Exiting.\n");
        return -1;
    }
    
    aether->models->vertexData = malloc(sizeof(aetherDefaultTriangle));
    aether->models->vertexSize = sizeof(aetherDefaultTriangle);

    memcpy(aether->models->vertexData, aetherDefaultTriangle, sizeof(aetherDefaultTriangle));
    glEnableVertexAttribArray(aether->models->attributes.coord3d);

    glGenBuffers(1, &aether->models->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, aether->models->VBO);
    glBufferData(GL_ARRAY_BUFFER, aether->models->vertexSize, aether->models->vertexData, GL_STATIC_DRAW);

    aetherLoop(aether);
    aetherFree(aether);
    return 0;
}
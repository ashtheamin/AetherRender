#include "aether.h"

void aetherModelDefaultCubeInit(struct aetherModel* model) {
    if(model == NULL) return;
    
    float aetherDefaultCubeVertices[] = {
        // front
        -0.8, -0.8,  0.8,
        0.8, -0.8,  0.8,
        0.8,  0.8,  0.8,
        -0.8,  0.8,  0.8,
        // back
        -0.8, -0.8, -0.8,
        0.8, -0.8, -0.8,
        0.8,  0.8, -0.8,
        -0.8,  0.8, -0.8
    };

    float aetherDefaultCubeColours[] = {
        // front colours
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0,
        1.0, 1.0, 1.0,
        // back colours
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0,
        1.0, 1.0, 1.0
    };

    unsigned short aetherDefaultCubeIBOElements[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

    model->vertexSize = sizeof(aetherDefaultCubeVertices);
    glEnableVertexAttribArray(model->attributes.coord3d);
    glGenBuffers(1, &model->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, model->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(aetherDefaultCubeVertices), aetherDefaultCubeVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(model->attributes.colour);
    glGenBuffers(1, &model->colours);
    glBindBuffer(GL_ARRAY_BUFFER, model->colours);
    glBufferData(GL_ARRAY_BUFFER, sizeof(aetherDefaultCubeColours), aetherDefaultCubeColours, GL_STATIC_DRAW);


    glGenBuffers(1, &model->IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(aetherDefaultCubeIBOElements), aetherDefaultCubeIBOElements, GL_STATIC_DRAW);
}

void aetherFramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

unsigned int aetherShaderDefaultInit() {
    const char *aetherDefaultVertexShaderSource = 
        "#version 120\n"
        "attribute vec3 coord3d;"
        "attribute vec3 colour;"
        "varying vec3 f_colour;"
        "uniform mat4 model;"
        "uniform mat4 view;"
        "uniform mat4 projection;"
        "void main(void) {"
        "  gl_Position = projection * view * model * vec4(coord3d, 1.0);"
        "  f_colour = colour;"
    "}";

    const char *aetherDefaultFragmentShaderSource = 
        "#version 120\n"
        "varying vec3 f_colour;"
        "void main(void) {        "
            "gl_FragColor = vec4(f_colour.r, f_colour.g, f_colour.b, 1.0);"
        "}";
    unsigned int shader = 0;

    unsigned int aetherVertexShader = 0;
    aetherVertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(aetherVertexShader, 1, &aetherDefaultVertexShaderSource, NULL);
    glCompileShader(aetherVertexShader);

    int success = 0;
    char infoLog[512];
    glGetShaderiv(aetherVertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(aetherVertexShader, 512, NULL, infoLog);
        printf("aetherShaderInit(): Compiling vertex shader failed.\n. %s", infoLog);
        return 0;
    }

    unsigned int aetherFragmentShader = 0;
    aetherFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(aetherFragmentShader, 1, &aetherDefaultFragmentShaderSource, NULL);
    glCompileShader(aetherFragmentShader);

    success = 0;
    glGetShaderiv(aetherFragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(aetherFragmentShader, 512, NULL, infoLog);
        printf("aetherShaderInit(): Compiling Fragment shader failed.\n. %s", infoLog);
        return 0;
    }

    shader = glCreateProgram();
    glAttachShader(shader, aetherVertexShader);
    glAttachShader(shader, aetherFragmentShader);
    glLinkProgram(shader);

    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(aetherFragmentShader, 512, NULL, infoLog);
        printf("aetherShaderInit(): Compiling shader program failed.\n. %s", infoLog);
        return 0;
    }

    glDeleteShader(aetherVertexShader);
    glDeleteShader(aetherFragmentShader);

    return shader;
}

struct aetherModel * aetherModelInit() {
    struct aetherModel* model = malloc(sizeof(struct aetherModel));
    if (model == NULL) return NULL;

    model->next = NULL;
    model->IBO = 0;
    model->VBO = 0;
    vec3 position = {0.0, 0.0, 0.0};
    glm_vec3_copy(position, model->position);

    model->shader = aetherShaderDefaultInit();
    if (model->shader == 0) {
        printf("aetherModelInit(): Failed to initialise model. Exiting.\n");
        free(model);
        return NULL;
    }

	model->attributes.coord3d = glGetAttribLocation(model->shader, "coord3d");
	if (model->attributes.coord3d == -1) {
		printf("aetherModelInit(): Failed to bind coord3d attribute. Exiting.\n");
        free(model);
        return NULL;
	}

    model->attributes.colour = glGetAttribLocation(model->shader, "colour");
	if (model->attributes.colour == -1) {
		printf("aetherModelInit(): Failed to bind colour attribute. Exiting.\n");
        free(model);
        return NULL;
	}

    model->uniforms.view = glGetUniformLocation(model->shader, "view");
    model->uniforms.model = glGetUniformLocation(model->shader, "model");
    model->uniforms.projection = glGetUniformLocation(model->shader, "projection");

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

    aether->window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Aether Renderer", NULL, NULL);
    if (aether->window == NULL) {
        printf("Failed to create GLFW Window. Exiting.\n");
        free(aether);
        return NULL;
    }

    glfwSetWindowUserPointer(aether->window, aether);
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
    if (aether->models == NULL) {
        printf("aetherInit() Failed. Returning.\n");
        free(aether);
        return NULL;
    }

    // Camera init code.
    memcpy(aether->camera.position, (vec3){0.0, 0.0, 0.0}, sizeof(vec3));
    memcpy(aether->camera.front, (vec3){0.0, 0.0, -1.0}, sizeof(vec3));
    memcpy(aether->camera.up, (vec3){0.0, 1.0, 0.0}, sizeof(vec3));
    aether->camera.yaw = -90.0;
    aether->camera.pitch = 0.0;
    aether->camera.speed = 5;

    aether->camera.mouse.lastX = SCREEN_WIDTH/2;
    aether->camera.mouse.lastY = SCREEN_HEIGHT/2;
    aether->camera.mouse.usedBefore = false;
    aether->camera.mouse.sensitivity = 0.1;

    aether->timing.deltaTime = 0.0;
    aether->timing.lastTime = 0.0;

    glEnable(GL_DEPTH_TEST);

    glfwSetCursorPosCallback(aether->window, aetherMouseCallback);
    glfwSetInputMode(aether->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    return aether;
}

void aetherInput(struct aether * aether) {
    if (glfwGetKey(aether->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(aether->window, GLFW_TRUE);
    }

    float speed = aether->camera.speed * aether->timing.deltaTime;
    vec3 updatedPosition = {0.0, 0.0, 0.0};

    if (glfwGetKey(aether->window, GLFW_KEY_W) == GLFW_PRESS) {
        glm_vec3_scale(aether->camera.front, speed, updatedPosition);
        glm_vec3_add(aether->camera.position, updatedPosition, aether->camera.position);
    }

    if (glfwGetKey(aether->window, GLFW_KEY_S) == GLFW_PRESS) {
        glm_vec3_scale(aether->camera.front, speed, updatedPosition);
        glm_vec3_negate(updatedPosition);
        glm_vec3_add(aether->camera.position, updatedPosition, aether->camera.position);
    }

    if (glfwGetKey(aether->window, GLFW_KEY_A) == GLFW_PRESS) {
        glm_vec3_cross(aether->camera.front, aether->camera.up, updatedPosition);
        glm_vec3_normalize(updatedPosition);
        glm_vec3_scale(updatedPosition, speed, updatedPosition);
        glm_vec3_negate(updatedPosition);
        glm_vec3_add(updatedPosition, aether->camera.position, aether->camera.position);
    }

    if (glfwGetKey(aether->window, GLFW_KEY_D) == GLFW_PRESS) {
        glm_vec3_cross(aether->camera.front, aether->camera.up, updatedPosition);
        glm_vec3_normalize(updatedPosition);
        glm_vec3_scale(updatedPosition, speed, updatedPosition);
        glm_vec3_add(updatedPosition, aether->camera.position, aether->camera.position);
    }
}

void aetherLoop(struct aether * aether) {
    while (!glfwWindowShouldClose(aether->window)) {
        aetherUpdateTiming(aether);
        aetherInput(aether);
        glClearColor(0, 0.0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        struct aetherModel* model = aether->models;

        while (model != NULL) {
            mat4 modelUniform = GLM_MAT4_IDENTITY_INIT;
            mat4 view = GLM_MAT4_IDENTITY_INIT;
            mat4 projection = GLM_MAT4_IDENTITY_INIT;

            glm_perspective(glm_rad(45.0f), (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 0.1f, 100.0f, projection);

            vec3 lookingat = {0.0, 0.0, 0.0};
            glm_vec3_add(aether->camera.position, aether->camera.front, lookingat);
            glm_lookat(aether->camera.position, lookingat, aether->camera.up, view);

            glm_translate(modelUniform, model->position);

            glUniformMatrix4fv(model->uniforms.model, 1, GL_FALSE, modelUniform[0]);
            glUniformMatrix4fv(model->uniforms.view, 1, GL_FALSE, view[0]);
            glUniformMatrix4fv(model->uniforms.projection, 1, GL_FALSE, projection[0]);

            glUseProgram(aether->models->shader);
            glBindBuffer(GL_ARRAY_BUFFER, aether->models->VBO);
            glVertexAttribPointer(aether->models->attributes.coord3d, 3, GL_FLOAT, GL_FALSE, 0, 0);
            
            glBindBuffer(GL_ARRAY_BUFFER, aether->models->colours);
            glVertexAttribPointer(aether->models->attributes.colour, 3, GL_FLOAT, GL_FALSE, 0, 0);
            
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, aether->models->IBO);
            int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
            glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

            model = model->next;
        }

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

void aetherMouseCallback(GLFWwindow* window, double x, double y) {
    struct aether* aether = NULL;
    aether = glfwGetWindowUserPointer(window);
    // Set the last x and y mouse positions
    if (aether->camera.mouse.usedBefore == false) {
        aether->camera.mouse.usedBefore = true;
        aether->camera.mouse.lastX = x;
        aether->camera.mouse.lastY = y;
    }

    // Get the difference between the last mouse position and current
    float x_offset = x - aether->camera.mouse.lastX;
    float y_offset = y - aether->camera.mouse.lastY;

    // Store the current mouse positions to be the last
    aether->camera.mouse.lastX = x;
    aether->camera.mouse.lastY = y;

    // Multiply the difference in mouse position with sensitivty
    x_offset = x_offset * aether->camera.mouse.sensitivity;
    y_offset = y_offset * aether->camera.mouse.sensitivity;

    // Add the movement difference to the camera's viewing angles.
    aether->camera.yaw = aether->camera.yaw + x_offset;
    aether->camera.pitch = aether->camera.pitch - y_offset;

    // Lock the camera's pitch so it does not go too far up or down
    if (aether->camera.pitch > 89.0) {
        aether->camera.pitch = 89.0;
    }

    if (aether->camera.pitch < -89.0) {
        aether->camera.pitch = -89.0;
    }

    // Calculate the angles that the vector the camera is looking at needs to change with.
    vec3 direction = {0.0, 0.0, 0.0};
    direction[0] = cos(glm_rad(aether->camera.yaw)) * cos(glm_rad(aether->camera.pitch));
    direction[1] = sin(glm_rad(aether->camera.pitch));
    direction[2] = sin(glm_rad(aether->camera.yaw)) * cos(glm_rad(aether->camera.pitch));

    // Add the movement difference to the vector the camera is looking at.
    glm_vec3_normalize_to(direction, aether->camera.front);
}

void aetherUpdateTiming(struct aether* aether) {
    float currentTime = glfwGetTime();
    aether->timing.deltaTime = currentTime - aether->timing.lastTime;
    aether->timing.lastTime = currentTime;
}
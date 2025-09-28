#include "aether.h"

const char *aetherDefaultVertexShaderSource = 
    "#version 120\n"
    "attribute vec3 coord3d;"
    "attribute vec3 colour;"
    "varying vec3 f_colour;"
    "void main(void) {"
    "  gl_Position = vec4(coord3d, 1.0);"
    "  f_colour = colour;"
"}";


const char *aetherDefaultFragmentShaderSource = 
    "#version 120\n"
    "varying vec3 f_colour;"
    "void main(void) {        "
        "gl_FragColor = vec4(f_colour.r, f_colour.g, f_colour.b, 1.0);"
    "}";

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
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

unsigned int aetherShaderDefaultInit() {
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

    model->IBO = 0;
    model->VBO = 0;

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

    glEnable(GL_DEPTH_TEST);
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
        glClearColor(0, 0.0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        glUseProgram(aether->models->shader);
        glBindBuffer(GL_ARRAY_BUFFER, aether->models->VBO);
        glVertexAttribPointer(aether->models->attributes.coord3d, 3, GL_FLOAT, GL_FALSE, 0, 0);
        
        glBindBuffer(GL_ARRAY_BUFFER, aether->models->colours);
        glVertexAttribPointer(aether->models->attributes.colour, 3, GL_FLOAT, GL_FALSE, 0, 0);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, aether->models->IBO);
        int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
        glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

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
#ifndef SHADER_H_
#define SHADER_H_
const char *aetherDefaultVertexShaderSource = 
    "#version 100\n"
    "attribute vec2 coord2d;                  "
    "void main(void) {                        "
    "  gl_Position = vec4(coord2d, 0.0, 1.0); "
    "}";

const char *aetherDefaultFragmentShaderSource = 
    "#version 100\n"
    "void main(void) {        "
    "  gl_FragColor[0] = 0.0; "
    "  gl_FragColor[1] = 0.0; "
    "  gl_FragColor[2] = 1.0; "
    "}";
#endif
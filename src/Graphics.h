//
// Created by mateuszp on 1/19/24.
//

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include "GraphicsShader.h"
#include "ComputeShader.h"

class Graphics {
public:
    Graphics();

    void init();

    void run();

    void cleanup();

private:
    enum ShaderChoice {
        Mandelbrot,
        RayTracing
    };

    const std::vector<float> quad = {
        1.0, 1.0, 1.0, 1.0,
        1.0, -1.0, 1.0, 0.0,
        -1.0, 1.0, 0.0, 1.0,

        1.0, -1.0, 1.0, 0.0,
        -1.0, -1.0, 0.0, 0.0,
        -1.0, 1.0, 0.0, 1.0,
    };

    GLFWwindow *window;

    uint32_t vao, vbo;

    GraphicsShader* quad_shader;

    ComputeShader* mandelbrot_shader;
    ComputeShader* raytracing_shader;

    ShaderChoice shader_choice = Mandelbrot;
    bool key_down = false;

    uint32_t screen_texture;

    uint32_t tex_w, tex_h;
};


#endif //GRAPHICS_H

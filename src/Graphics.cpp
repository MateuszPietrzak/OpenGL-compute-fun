//
// Created by mateuszp on 1/19/24.
//

#include "Graphics.h"

#include "GraphicsShader.h"

Graphics::Graphics() {
}

void Graphics::init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);


    window = glfwCreateWindow(win_width, win_height, "OpenGL", nullptr, nullptr);
    if (window == nullptr) {
        throw std::runtime_error("No window");
    }
    glfwSetWindowUserPointer(window, this);

    glfwSetWindowSizeCallback(window, [](GLFWwindow * window, int width, int height) {
        Graphics* graphics = (Graphics*) glfwGetWindowUserPointer(window);
        graphics->win_width = width;
        graphics->win_height = height;
        std::cout << "New width: " << graphics->win_width << "\n";
        std::cout << "New height: " << graphics->win_height << "\n";
        glViewport(0, 0, graphics->win_width, graphics->win_height);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, graphics->screen_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, graphics->win_width, graphics->win_height, 0, GL_RGBA,
                     GL_FLOAT, nullptr);
        glBindImageTexture(0, graphics->screen_texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    });

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        throw std::runtime_error("Sad no glad :(");
    }

    glViewport(0, 0, win_width, win_height);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    glGenTextures(1, &screen_texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, screen_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, win_width, win_height, 0, GL_RGBA,
                 GL_FLOAT, nullptr);
    glBindImageTexture(0, screen_texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, quad.size() * sizeof(quad.front()), quad.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          reinterpret_cast<void *>(2 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    quad_shader = new GraphicsShader("assets/shaders/render_quad.vert",
                                     "assets/shaders/render_quad.frag");

    quad_shader->use();
    quad_shader->setInt("tex_sampler", 0);

    mandelbrot_shader = new ComputeShader("assets/shaders/mandelbrot.glsl");
    raytracing_shader = new ComputeShader("assets/shaders/raytracing.glsl");
}

void Graphics::run() {
    while (!glfwWindowShouldClose(window)) {

        if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            if(!key_down) {
                switch (shader_choice) {
                    case Mandelbrot:
                        shader_choice = RayTracing;
                        break;
                    case RayTracing:
                        shader_choice = Mandelbrot;
                        break;
                }
            }
            key_down = true;
        } else {
            key_down = false;
        }

        switch (shader_choice) {
            case Mandelbrot:
                mandelbrot_shader->use();
                break;
            case RayTracing:
                raytracing_shader->use();
                break;
        }
        glDispatchCompute(win_width, win_height, 1);

        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        glClear(GL_COLOR_BUFFER_BIT);
        quad_shader->use();
        glBindVertexArray(vao);
        glActiveTexture(GL_TEXTURE0);
        glDrawArrays(GL_TRIANGLES, 0, quad.size());

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Graphics::cleanup() {
    glfwTerminate();
}

#include "ui.h"
#include <iostream>
#include <fstream>
#include <sstream>

namespace Asuka::UI {
    void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    void processInput(GLFWwindow* window) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }

    void preview_render_result_with_ui() {




    }

    GLFWwindow* create_gui_window() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


        GLFWwindow* window = glfwCreateWindow(800, 600, "Asuka Renderer", NULL, NULL);
        if (window == nullptr) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return nullptr;
        }
        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return nullptr;
        }

        return window;
    }

    void draw_gui(GLFWwindow* window, std::shared_ptr<Image> image) {
        if (image == nullptr) return;
        // std::shared_ptr<Image> image = std::make_shared<Image>("F:/Project/AsukaRenderer/outputs/result.png");
        glViewport(0, 0, image->width, image->height);
        // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetWindowSize(window, image->width, image->height);

        while (!glfwWindowShouldClose(window)) {
            processInput(window);

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            image->draw();


            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glfwTerminate();
    }
}
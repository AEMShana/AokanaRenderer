#include "ui.h"
#include <iostream>
#include <fstream>
#include <sstream>

namespace Aokana::UI {

    void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    void ProcessInput(GLFWwindow* window) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }

    void PreviewRenderResultWithUI() {
    }

    GLFWwindow* CreateGUIWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


        GLFWwindow* window = glfwCreateWindow(800, 600, "Aokana Renderer", NULL, NULL);
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

    void DrawGUI(GLFWwindow* window, std::shared_ptr<Image> image) {
        if (image == nullptr) return;
        // std::shared_ptr<Image> image = std::make_shared<Image>("F:/Project/AokanaRenderer/outputs/result.png");
        glViewport(0, 0, image->width, image->height);
        // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetWindowSize(window, image->width, image->height);

        while (!glfwWindowShouldClose(window)) {
            ProcessInput(window);

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            image->Draw();


            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glfwTerminate();
    }
}
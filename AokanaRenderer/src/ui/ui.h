#pragma once

#include "image.h"

namespace Aokana::UI {

    void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
    void ProcessInput(GLFWwindow* window);
    void PreviewRenderResultWithUI();
    GLFWwindow* CreateGUIWindow();
    void DrawGUI(GLFWwindow* window, std::shared_ptr<Image> image);
}
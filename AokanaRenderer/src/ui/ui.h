#pragma once

#include "image.h"

namespace Aokana::UI {

    void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    void processInput(GLFWwindow* window);

    void preview_render_result_with_ui();

    GLFWwindow* create_gui_window();

    void draw_gui(GLFWwindow* window, std::shared_ptr<Image> image);
}
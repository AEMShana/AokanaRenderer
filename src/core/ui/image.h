#pragma once

#include <vector>
#include <iostream>
#include <memory>
#include <string>

#include "shader.h"
#include "../../../external/glad/glad.h"
#include "../../../external/glfw/include/GLFW/glfw3.h"

namespace Asuka::UI {

    class Image {
    public:
        Image(int width, int height);
        Image(int width, int height, const unsigned char* data);
        Image(const std::string& image_path);
        ~Image();
        void create_mesh();
        void create_texture(const unsigned char* data, unsigned int width, unsigned int height, unsigned int channels);
        void draw();
        void modify_subimage(int x_min, int y_min, int x_max, int y_max, const std::vector<unsigned char>& subimage_data);
    public:
        unsigned int texture;
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;

        int width;
        int height;
        int channels;

        std::shared_ptr<Shader> shader;

    private:
        static constexpr float vertices[] = {
            //     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
            1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
            1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
            -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
            -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
        };

        static constexpr unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };

    };

}
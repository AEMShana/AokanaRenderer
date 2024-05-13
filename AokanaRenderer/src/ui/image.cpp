#include "image.h"
#include <vector>
#include <stb_image.h>

namespace Aokana::UI {

    Image::Image(int width, int height) {
        this->width = width;
        this->height = height;
        this->channels = 3;

        std::vector<unsigned char> image_data(width * height * channels, 0);

        create_texture(image_data.data(), width, height, channels);
        create_mesh();
        shader = std::make_shared<Shader>(
            "./src/ui/shaders/ui_image_shader/ui_image_shader.vertex",
            "./src/ui/shaders/ui_image_shader/ui_image_shader.fragment");

        shader->use();
        shader->setInt("texture1", 0);
    }

    Image::Image(int width, int height, const unsigned char* data) {
        this->width = width;
        this->height = height;
        this->channels = 3;


        create_texture(data, width, height, channels);
        create_mesh();
        shader = std::make_shared<Shader>(
            "./src/ui/shaders/ui_image_shader/ui_image_shader.vertex",
            "./src/ui/shaders/ui_image_shader/ui_image_shader.fragment");

        shader->use();
        shader->setInt("texture1", 0);
    }


    Image::Image(const std::string& image_path) {
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(image_path.c_str(), &width, &height, &channels, 0);

        create_texture(data, width, height, channels);
        stbi_image_free(data);

        create_mesh();

        // shader = std::make_shared<Shader>(
        //     "shaders/ui_image_shader/ui_image_shader.vertex",
        //     "shaders/ui_image_shader/ui_image_shader.fragment");

        shader = std::make_shared<Shader>(
            "./src/ui/shaders/ui_image_shader/ui_image_shader.vertex",
            "./src/ui/shaders/ui_image_shader/ui_image_shader.fragment");

        shader->use(); // don't forget to activate/use the shader before setting uniforms!
        shader->setInt("texture1", 0);
    }

    Image::~Image() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void Image::create_mesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    void Image::create_texture(const unsigned char* data, unsigned int width, unsigned int height, unsigned int channels) {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (channels == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    void Image::draw() {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        shader->use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    void Image::modify_subimage(int x_min, int y_min, int x_max, int y_max, const std::vector<unsigned char>& subimage_data) {
        glBindTexture(GL_TEXTURE_2D, texture);

        x_min = std::max(0, std::min(x_min, width));
        x_max = std::max(0, std::min(x_max, width));
        y_min = std::max(0, std::min(y_min, height));
        y_max = std::max(0, std::min(y_max, height));

        int subimage_width = x_max - x_min;
        int subimage_height = y_max - y_min;

        // std::vector<unsigned char> red_pixels(subimage_width * subimage_height * 4, 0);

        // for (int i = 0; i < subimage_width * subimage_height; i++) {
        //     red_pixels[i * 4] = 255;
        // }
        glTexSubImage2D(GL_TEXTURE_2D, 0, x_min, y_min, subimage_width + 1, subimage_height + 1, GL_RGB, GL_UNSIGNED_BYTE, subimage_data.data());
    }
}
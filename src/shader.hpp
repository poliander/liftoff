#pragma once

#include <GL/glew.h>

#include <fstream>
#include <string>

#include <glm/glm.hpp>

using std::ifstream;
using std::string;

enum uniforms_e {
    UNI_MODEL,
    UNI_VIEW,
    UNI_PROJECTION,
    UNI_MVP,
    UNI_COLOR
};

class Shader {
    public:
        Shader(const string vsFilename, const string fsFilename);
        ~Shader();

        void   bind();
        void   unbind();
        void   update(uint8_t id, glm::mat4 m);
        void   update(uint8_t id, glm::vec2 v);
        void   update(uint8_t id, glm::vec4 v);

    private:
        GLuint program;
        GLuint shaders[2];
        GLuint uniforms[5];

        string load(const string& filename);
        GLuint create(const string& text, unsigned int type);
};

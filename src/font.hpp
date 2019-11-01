#pragma once

#include <map>
#include <memory>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "texture.hpp"
#include "shader.hpp"
#include "view.hpp"

using namespace std;

struct Glyph
{
    GLuint     texture;
    glm::ivec2 size;
    glm::ivec2 bearing;
    GLint      advance;
};

class Font
{
    public:
        Font(const string& filename, shared_ptr<Shader> s);
        ~Font();

        void draw(const string& txt, float x, float y, float s, float r, float g, float b, float a);

    private:
        shared_ptr<Shader> shader;
        map<GLchar, Glyph> glyphs;

        GLuint vertexArray;
        GLuint vertexArrayBuffer;
};

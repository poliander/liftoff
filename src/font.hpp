#pragma once

#include <map>
#include <memory>
#include <string>

#include "texture.hpp"
#include "shader.hpp"
#include "view.hpp"

using namespace std;

struct Glyph
{
    unsigned short code;

    float x;
    float y;
    float w;
};

class State;

class Font
{
    public:
        Font(const string& filename, shared_ptr<Shader> s);
        ~Font();

        void draw(const string& txt, float x, float y, float z, float s, float r, float g, float b, float a);

    private:
        shared_ptr<Shader> shader;

        GLuint vertexArray;
        GLuint vertexArrayBuffer;
};

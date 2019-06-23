#pragma once

#include <string>

using namespace std;

class Shader
{
    public:
        Shader(string vertexShaderFilename, string fragmentShaderFlename);
        ~Shader();
};

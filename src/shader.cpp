#include "shader.hpp"

Shader::Shader(string vsFilename, string fsFilename)
{
    program = glCreateProgram();

    shaders[0] = create(load(vsFilename), GL_VERTEX_SHADER);
    shaders[1] = create(load(fsFilename), GL_FRAGMENT_SHADER);

    glAttachShader(program, shaders[0]);
    glAttachShader(program, shaders[1]);

    glBindAttribLocation(program, 0, "position");
    glBindAttribLocation(program, 1, "texCoord");
    glBindAttribLocation(program, 2, "normal");

    glLinkProgram(program);
    glValidateProgram(program);
}

Shader::~Shader()
{
    glDetachShader(program, shaders[1]);
    glDeleteShader(shaders[1]);

    glDetachShader(program, shaders[2]);
    glDeleteShader(shaders[0]);

    glDeleteProgram(program);
}

GLuint Shader::create(const string& text, unsigned int type)
{
    GLuint shader = glCreateShader(type);

    if (shader) {
        const GLchar* t[1];
        GLint l[1];

        t[0] = text.c_str();
        l[0] = text.length();

        glShaderSource(shader, 1, t, l);
        glCompileShader(shader);
    }

    return shader;
}

string Shader::load(const string& filename)
{
    string line, output;
    ifstream file;

    file.open((filename).c_str());

    if (file.is_open()) {
        while (file.good()) {
            getline(file, line);
            output.append(line + "\n");
        }
    }

    return output;
}

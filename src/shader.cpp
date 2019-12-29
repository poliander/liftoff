#include "shader.hpp"

Shader::Shader(const string vsFilename, const string fsFilename)
{
    GLint success;
    GLchar error[1024];

    program = glCreateProgram();

    shaders[0] = create(load(vsFilename), GL_VERTEX_SHADER);
    shaders[1] = create(load(fsFilename), GL_FRAGMENT_SHADER);

    glAttachShader(program, shaders[0]);
    glAttachShader(program, shaders[1]);

    glBindAttribLocation(program, 0, "position");
    glBindAttribLocation(program, 1, "uv");
    glBindAttribLocation(program, 2, "normal");

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
        glGetProgramInfoLog(program, sizeof(error), NULL, error);
        FILE *fp = fopen("shader.log", "a");fprintf(fp,"%s\n", error);fclose(fp);
        exit(-1);
    }

    glValidateProgram(program);
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
    if (success == GL_FALSE) {
        glGetProgramInfoLog(program, sizeof(error), NULL, error);
        FILE *fp = fopen("shader.log", "a");fprintf(fp,"%s\n", error);fclose(fp);
        exit(-1);
    }

    uniforms[UNI_MODEL] = glGetUniformLocation(program, "model");
    uniforms[UNI_VIEW] = glGetUniformLocation(program, "view");
    uniforms[UNI_PROJECTION] = glGetUniformLocation(program, "projection");
    uniforms[UNI_MVP] = glGetUniformLocation(program, "mvp");
    uniforms[UNI_COLOR] = glGetUniformLocation(program, "color");
}

Shader::~Shader()
{
    glDetachShader(program, shaders[1]);
    glDeleteShader(shaders[1]);

    glDetachShader(program, shaders[0]);
    glDeleteShader(shaders[0]);

    glDeleteProgram(program);
}

void Shader::bind()
{
    glUseProgram(program);
}

void Shader::unbind()
{
    glUseProgram(0);
}

void Shader::update(uint8_t id, glm::vec2 v)
{
    glUniform2fv(uniforms[id], 1, &v[0]);
}

void Shader::update(uint8_t id, glm::vec4 v)
{
    glUniform4fv(uniforms[id], 1, &v[0]);
}

void Shader::update(uint8_t id, glm::mat4 m)
{
    glUniformMatrix4fv(uniforms[id], 1, GL_FALSE, &m[0][0]);
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

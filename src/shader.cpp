#include "shader.hpp"
#include "transform.hpp"

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

    GLint success = 0;
    GLchar error[1024] = { 0 };
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
    if (success == GL_FALSE) {
        glGetProgramInfoLog(program, sizeof(error), NULL, error);
        FILE *fp = fopen("shader.log", "a");fprintf(fp,"%s\n", error);fclose(fp);
        exit(-1);
    }

    uniforms[0] = glGetUniformLocation(program, "MVP");
    uniforms[1] = glGetUniformLocation(program, "Normal");
    uniforms[2] = glGetUniformLocation(program, "lightDirection");
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

void Shader::update(const Transform& transform, const Camera& camera)
{
    glm::mat4 mvp = transform.getMVP(camera);
    glm::mat4 normal = transform.getModel();

    glUniformMatrix4fv(uniforms[0], 1, GL_FALSE, &mvp[0][0]);
    glUniformMatrix4fv(uniforms[1], 1, GL_FALSE, &normal[0][0]);
    glUniform3f(uniforms[2], 0.0f, 0.0f, 1.0f);
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

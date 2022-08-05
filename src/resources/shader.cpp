/*
 *  LIFT-OFF: BEYOND GLAXIUM
 *  Copyright (C) 2019 René Pollesch <poliander@mailbox.org>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "shader.hpp"

Shader::Shader(const string vsFilename, const string fsFilename) {
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

        FILE *fp = fopen("shader.log", "w");
        fprintf(fp, "glLinkProgram failed\n\n%s\n%s\n\n%s\n\n", (vsFilename).c_str(), (fsFilename).c_str(), error);
        fclose(fp);

        exit(-1);
    }

    glValidateProgram(program);
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);

    if (success == GL_FALSE) {
        glGetProgramInfoLog(program, sizeof(error), NULL, error);

        FILE *fp = fopen("shader.log", "w");
        fprintf(fp, "glValidateProgram failed\n\n%s\n%s\n\n%s\n\n", (vsFilename).c_str(), (fsFilename).c_str(), error);
        fclose(fp);

        exit(-1);
    }

    uniforms[UNI_MODEL] = glGetUniformLocation(program, "model");
    uniforms[UNI_VIEW] = glGetUniformLocation(program, "view");
    uniforms[UNI_PROJECTION] = glGetUniformLocation(program, "projection");
    uniforms[UNI_MVP] = glGetUniformLocation(program, "mvp");
    uniforms[UNI_COLOR] = glGetUniformLocation(program, "color");
}

Shader::~Shader() {
    glDetachShader(program, shaders[1]);
    glDeleteShader(shaders[1]);

    glDetachShader(program, shaders[0]);
    glDeleteShader(shaders[0]);

    glDeleteProgram(program);
}

void Shader::bind() {
    glUseProgram(program);
}

void Shader::unbind() {
    glUseProgram(0);
}

void Shader::update(uint8_t id, glm::vec2 v) {
    glUniform2fv(uniforms[id], 1, &v[0]);
}

void Shader::update(uint8_t id, glm::vec4 v) {
    glUniform4fv(uniforms[id], 1, &v[0]);
}

void Shader::update(uint8_t id, glm::mat4 m) {
    glUniformMatrix4fv(uniforms[id], 1, GL_FALSE, &m[0][0]);
}

GLuint Shader::create(const string& text, unsigned int type) {
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

string Shader::load(const string& filename) {
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

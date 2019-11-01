#include "font.hpp"

Font::Font(const string& filename, shared_ptr<Shader> s) : shader(s)
{
    FT_Library ft;
    FT_Face face;

    FT_Init_FreeType(&ft);
    FT_New_Face(ft, filename.c_str(), 0, &face);
    FT_Set_Pixel_Sizes(face, 0, 64);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (GLubyte c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            continue;
        }

        GLuint texture;

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Glyph glyph = {
            texture, 
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };

        glyphs.insert(std::pair<GLchar, Glyph>(c, glyph));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glGenVertexArrays(1, &vertexArray);
    glGenBuffers(1, &vertexArrayBuffer);
    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Font::~Font()
{
}

void Font::draw(const string& txt, float x, float y, float s, float r, float g, float b, float a)
{
    shader->bind();
    shader->update(UNI_COLOR, glm::vec4(r, g, b, a));

    glBindVertexArray(vertexArray);

    for (char const &c: txt) {
/*
        auto i = glyphs.find(c);

        if (i != glyphs.end()) {
            shader->update(UNI_MVP, View::transform2D(x, y, 0, 0, s, s));

            GLfloat vertices[6][4] = {
                { 0.0, 0.0, tx,     ty + 0.25f },
                { 0.0, 1.0, tx,     ty },
                { 1.0, 1.0, tx + w, ty },

                { 0.0, 0.0, tx,     ty + 0.25f },
                { 1.0, 1.0, tx + w, ty },
                { 1.0, 0.0, tx + w, ty + 0.25f }
            };

            glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffer);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glDrawArrays(GL_TRIANGLES, 0, 6);

            x -= s * w * 30;
        }
*/
    }

    shader->unbind();
}

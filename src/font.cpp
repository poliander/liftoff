/*
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

#include "font.hpp"

Font::Font(const string& filename, shared_ptr<Shader> s, uint8_t q) : Quad() {
    FT_Library ft;
    FT_Face face;

    view = View::createOrthographic(-400.0f, 400.0f, -300.0f, 300.0f);
    shader = s;
    scale = 256.0f / pow(2, q);

    FT_Init_FreeType(&ft);
    FT_New_Face(ft, filename.c_str(), 0, &face);
    FT_Set_Pixel_Sizes(face, 0, pow(2, q));

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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glyph_t glyph = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x >> 6,
            face->glyph->metrics.vertAdvance >> 6
        };

        glyphs.insert(std::pair<GLchar, glyph_t>(c, glyph));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

Font::~Font() {
    for (auto i = glyphs.begin(); i != glyphs.end(); i++) {
        glDeleteTextures(1, &i->second.texture);
    }
}

void Font::draw(const string& txt, float x, float y, float s, float r, float g, float b, float a) {
    x *= .5f;
    y *= .5f;
    s *= .5f * scale;

    float ox = x;

    shader->bind();
    shader->update(UNI_COLOR, glm::vec4(r, g, b, a));
    shader->update(UNI_MVP, view->transform(x, y, 1.0f, 1.0f));

    glBindVertexArray(vertexArray);

    for (string::const_iterator c = txt.begin(); c != txt.end(); c++) {
        glyph_t g = glyphs[*c];

        GLfloat xp = x + g.bearing.x * s;
        GLfloat yp = y - g.bearing.y * s;

        glBindTexture(GL_TEXTURE_2D, g.texture);

        Quad::setPosition(glm::vec4(xp, yp + (g.size.y * s), xp + (g.size.x * s) * 1.25f, yp));
        Quad::draw();

        if (*c == 10) {
            x = ox;
            y += g.height * s;
        } else {
            x += g.advance * s * 1.25f;
        }
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    shader->unbind();
}

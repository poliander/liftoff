#include "font.hpp"

Font::Font(const string& filename, shared_ptr<Shader> s) : shader(s)
{
    FT_Library ft;
    FT_Face face;

    FT_Init_FreeType(&ft);
    FT_New_Face(ft, filename.c_str(), 0, &face);
    FT_Set_Pixel_Sizes(face, 0, 128);

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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);

        Glyph glyph = {
            texture, 
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x >> 6,
            face->glyph->metrics.vertAdvance >> 6
        };

        glyphs.insert(std::pair<GLchar, Glyph>(c, glyph));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

Font::~Font()
{
}

void Font::draw(const string& txt, float x, float y, float s, float r, float g, float b, float a)
{
    float ox = x;

    shader->bind();

    shader->update(UNI_COLOR, glm::vec4(r, g, b, a));
    shader->update(UNI_MVP, glm::ortho(0.0f, 800.0f, 0.0f, 600.0f) * glm::translate(glm::vec3(x, y, 0)));

    glBindVertexArray(vertexArray);

    string::const_iterator c;

    for (c = txt.begin(); c != txt.end(); c++) {
        Glyph g = glyphs[*c];

        GLfloat xp = x + g.bearing.x * s;
        GLfloat yp = y - (g.size.y - g.bearing.y) * s;

        setPosition(glm::vec4(xp, yp, xp + g.size.x * s, yp + g.size.y * s));
        update();

        glBindTexture(GL_TEXTURE_2D, g.texture);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        if (*c == 10) {
            x = ox;
            y -= g.height * s;
        } else {
            x += g.advance * s;
        }
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    shader->unbind();
}

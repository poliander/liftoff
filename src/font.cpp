#include "font.hpp"

Font::Font(shared_ptr<Texture> t, shared_ptr<GlyphMap> m) :
    texture(t),
    map(m)
{
}

Font::~Font()
{
}

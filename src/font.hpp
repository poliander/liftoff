#pragma once

#include <memory>
#include <string>
#include <vector>

#include "glyphmap.hpp"
#include "texture.hpp"

using namespace std;

class Font
{
    public:
        Font(shared_ptr<Texture> t, shared_ptr<GlyphMap> m);
        ~Font();

    private:
        shared_ptr<Texture>  texture;
        shared_ptr<GlyphMap> map;
};

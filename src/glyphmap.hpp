#pragma once

#include <string>
#include <vector>

using namespace std;

struct Glyph
{
    unsigned short code;

    float x;
    float y;
    float w;
};

class GlyphMap
{
    public:
        GlyphMap(const string filename);
        ~GlyphMap();

    private:
        vector<Glyph> glyphs;
};

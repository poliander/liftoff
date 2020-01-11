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

#pragma once

#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

#include <glm/glm.hpp>

using std::ifstream;
using std::string;
using std::vector;

struct index_t {
    unsigned int vertex;
    unsigned int uv;
    unsigned int normal;

    bool operator < (const index_t& r) const { return vertex < r.vertex; }
};

class Object {
    public:
        explicit Object(string filename);
        ~Object();

        vector<glm::vec3>    positions;
        vector<glm::vec2>    texCoords;
        vector<glm::vec3>    normals;
        vector<unsigned int> indices;

        static bool          compareIndex(const index_t* a, const index_t* b) {
            return a->vertex < b->vertex;
        }

    private:
        vector<glm::vec3>    objVertices;
        vector<glm::vec2>    objTexCoords;
        vector<glm::vec3>    objNormals;
        vector<index_t>      objIndices;

        bool                 load(string filename);
        void                 index();
        unsigned int         previousVertexIndex(const vector<index_t*> &lookup, index_t* currentIndex);

        vector<string>       split(string txt, char delim);
        index_t              toIndex(const string& token);
        glm::vec2            toVec2(const string& line);
        glm::vec3            toVec3(const string& line);
};

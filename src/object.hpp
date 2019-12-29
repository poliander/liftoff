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
        Object(string filename);
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
        unsigned int         previousVertexIndex(vector<index_t*> &lookup, index_t* currentIndex);

        vector<string>       split(string txt, char delim);
        index_t              toIndex(const string& token);
        glm::vec2            toVec2(const string& line);
        glm::vec3            toVec3(const string& line);
};

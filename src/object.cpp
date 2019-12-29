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

#include "object.hpp"

Object::Object(string filename) {
    if (load(filename)) {
        index();
    }
}

Object::~Object() {
}

bool Object::load(string filename) {
    ifstream file;
    string line;

    file.open(filename.c_str());

    if (!file.is_open()) {
        return false;
    }

    while (file.good()) {
        getline(file, line);

        if (line.length() < 2) {
            continue;
        }

        switch (line.at(0)) {
            case 'v':
                switch (line.at(1)) {
                    case 't':
                        objTexCoords.push_back(toVec2(line));
                        break;

                    case 'n':
                        objNormals.push_back(toVec3(line));
                        break;

                    case ' ':
                        objVertices.push_back(toVec3(line));
                        break;
                }
                break;

            case 'f':
                vector<string> tokens = split(line, ' ');

                objIndices.push_back(toIndex(tokens[1]));
                objIndices.push_back(toIndex(tokens[2]));
                objIndices.push_back(toIndex(tokens[3]));
                break;
        }
    }

    return true;
}

void Object::index() {
    std::vector<index_t*> lookup;

    for (unsigned int i = 0; i < objIndices.size(); i++) {
        lookup.push_back(&objIndices[i]);
    }

    sort(lookup.begin(), lookup.end(), Object::compareIndex);

    positions.clear();
    texCoords.clear();
    normals.clear();
    indices.clear();

    for (unsigned int i = 0; i < objIndices.size(); i++) {
        index_t* currentIndex = &objIndices[i];

        unsigned int vertexIndex = previousVertexIndex(lookup, currentIndex);

        if (vertexIndex == -1) {
            vertexIndex = positions.size();

            positions.push_back(objVertices[currentIndex->vertex]);
            texCoords.push_back(objTexCoords[currentIndex->uv]);
            normals.push_back(objNormals[currentIndex->normal]);
        }

        indices.push_back(vertexIndex);
    }
}

unsigned int Object::previousVertexIndex(const vector<index_t*> &lookup, index_t* currentIndex) {
    unsigned int start = 0;
    unsigned int end = lookup.size();
    unsigned int current = (end - start) / 2 + start;
    unsigned int previous = start;

    while (current != previous) {
        index_t* testIndex = lookup[current];

        if (testIndex->vertex == currentIndex->vertex) {
            unsigned int countStart = current;

            for (unsigned int i = 0; i < current; i++) {
                index_t* possibleIndex = lookup[current - i];

                if (possibleIndex == currentIndex) {
                    continue;
                }

                if (possibleIndex->vertex != currentIndex->vertex) {
                    break;
                }

                countStart--;
            }

            for (unsigned int i = countStart; i < lookup.size() - countStart; i++) {
                index_t* possibleIndex = lookup[current + i];

                if (possibleIndex == currentIndex) {
                    continue;
                }

                if (possibleIndex->vertex != currentIndex->vertex) {
                    break;
                } else if (
                    (possibleIndex->uv == currentIndex->uv) &&
                    (possibleIndex->normal == currentIndex->normal)
                ) {
                    glm::vec3 currentPosition = objVertices[currentIndex->vertex];
                    glm::vec2 currentTexCoord = objTexCoords[currentIndex->uv];
                    glm::vec3 currentNormal = objNormals[currentIndex->normal];

                    for (unsigned int j = 0; j < positions.size(); j++) {
                        if (
                            currentPosition == positions[j] &&
                            currentTexCoord == texCoords[j] &&
                            currentNormal == normals[j]
                        ) {
                            return j;
                        }
                    }
                }
            }

            return -1;
        } else {
            if (testIndex->vertex < currentIndex->vertex) {
                start = current;
            } else {
                end = current;
            }
        }

        previous = current;
        current = (end - start) / 2 + start;
    }

    return -1;
}

glm::vec2 Object::toVec2(const string& line) {
    float x, y;

    sscanf(line.c_str(), "%*s %f %f", &x, &y);

    return glm::vec2(x, y);
}

glm::vec3 Object::toVec3(const string& line) {
    float x, y, z;

    sscanf(line.c_str(), "%*s %f %f %f", &x, &y, &z);

    return glm::vec3(x, y, z);
}

index_t Object::toIndex(const string& line) {
    unsigned int v, u, n;

    sscanf(line.c_str(), "%d/%d/%d", &v, &u, &n);

    return index_t { --v, --u, --n };
}

vector<string> Object::split(string line, char delim) {
    vector<string> result;

    while (line.size()) {
        int i = line.find(delim);

        if (i != string::npos) {
            result.push_back(line.substr(0, i));
            line = line.substr(i + 1);

            if (line.size() == 0) {
                result.push_back(line);
            }
        } else {
            result.push_back(line);
            line = "";
        }
    }

    return result;
}

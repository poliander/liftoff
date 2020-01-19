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

#include "level.hpp"

bool Level::load(string filename) {
    FILE *fp = fopen(filename.c_str(), "r");

    if (fp == NULL) {
        return false;
    }

    char msg[255], buf[1024], cmd[16], par[255];
    int i, money, life, m, p = 0;
    uint16_t e_obj;
    float p_x, p_y, p_z;
    float r_x, r_y, r_z;
    float s_x, s_y, s_z;
    float w_x, w_y, w_z;

    state->entities.clear();
    state->spawns.clear();
    state->spawn(state->player);

    while (!feof(fp)) {
        if (fgets(buf, 1024, fp) != NULL) {
            snprintf(cmd, sizeof(cmd), "");
            snprintf(cmd, sizeof(par), "");

            i = 0;
            m = 0;

            while ((i < strlen(buf)) && (m < 4)) {
                // forget the rest of the line
                if (buf[i] == ';') break;

                // remove unnecessary characters
                if (buf[i] < 33) {
                    if (m == 0) { i++; continue; }
                    if (m == 1) { m++; continue; }
                    if (m == 2) { i++; continue; }
                    if (m == 3) { m++; continue; }
                } else {
                    if (m == 0) { p = 0; m = 1; }
                    if (m == 2) { p = 0; m = 3; }
                }

                if (m == 1) {
                    cmd[p] = buf[i];
                    p++;
                    cmd[p] = 0;
                }

                if (m == 3) {
                    if (buf[i] == '{') {
                        i++;
                        while ((buf[i] != '}') && (i < strlen(buf))) {
                            par[p] = buf[i];
                            p++;
                            par[p] = 0;
                            i++;
                        }
                    } else {
                        par[p] = buf[i];
                        p++;
                        par[p] = 0;
                    }
                }

                i++;
            }

            if (cmd[0] && par[0]) {
                // music
                if (!strcmp(cmd, "soundtrack")) {
                    snprintf(state->lvl_music, sizeof(state->lvl_music), "%s", par);
                }

                // length
                if (!strcmp(cmd, "length")) sscanf(par, "%d", &state->lvl_length);

                // colliding object, obstacle
                if (!strcmp(cmd, "collider")) {
                    r_x = static_cast<float>(rand() % 3600) * .1f;
                    r_y = static_cast<float>(rand() % 3600) * .1f;
                    r_z = static_cast<float>(rand() % 3600) * .1f;

                    sscanf(par, "%f,%u,%f,%f,%f,%f,%f,%f,%f,%f,%u,%u",
                        (float *) &p_z,
                        (int *)   &e_obj,
                        (float *) &p_x,
                        (float *) &p_y,
                        (float *) &s_x,
                        (float *) &s_y,
                        (float *) &s_z,
                        (float *) &w_x,
                        (float *) &w_y,
                        (float *) &w_z,
                        (int *)   &life,
                        (int *)   &money
                    );

                    switch (e_obj) {
                        case OBJ_ASTEROID_1:
                            {
                                auto asteroid = make_shared<Asteroid>(state);

                                asteroid->setPos(p_x, p_y, p_z);
                                asteroid->setRot(r_x, r_y, r_z);
                                asteroid->setScale(s_x, s_y, s_z);
                                asteroid->setSpin(w_x, w_y, w_z);
                                asteroid->setLife(life);

                                state->spawn(asteroid);
                            }
                            break;

                        case OBJ_CARGO_1:
                            {
                                auto cargo = make_shared<Cargo>(state);

                                cargo->setPos(p_x, p_y, p_z);
                                cargo->setRot(r_x, r_y, r_z);
                                cargo->setScale(s_x, s_y, s_z);
                                cargo->setSpin(w_x, w_y, w_z);
                                cargo->setLife(life);

                                state->spawn(cargo);
                            }
                            break;
                    }
                }

                // scenery object
                if (!strcmp(cmd, "scenery")) {
                    r_x = static_cast<float>(rand() % 360);
                    r_y = static_cast<float>(rand() % 360);
                    r_z = static_cast<float>(rand() % 360);

                    sscanf(par, "%f,%u,%f,%f,%f,%f,%f,%f,%f,%f,%u",
                        (float *) &p_z,
                        (int *)   &e_obj,
                        (float *) &p_x,
                        (float *) &p_y,
                        (float *) &s_x,
                        (float *) &s_y,
                        (float *) &s_z,
                        (float *) &w_x,
                        (float *) &w_y,
                        (float *) &w_z,
                        (int *)   &life
                    );

                    switch (e_obj) {
                        case OBJ_ASTEROID_1:
                            {
                                auto asteroid = make_shared<Asteroid>(state);

                                asteroid->setType(E_TYPE_SCENERY);
                                asteroid->setPos(p_x, p_y, p_z);
                                asteroid->setRot(r_x, r_y, r_z);
                                asteroid->setScale(s_x, s_y, s_z);
                                asteroid->setSpin(w_x, w_y, w_z);
                                asteroid->setLife(life);

                                state->spawn(asteroid);
                            }
                            break;
                    }
                }
            }
        }
    }

    return true;
}

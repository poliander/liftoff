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

#include "configuration.hpp"

bool Configuration::load(const char* dir, config_t* c) {
    bool result = false;
    char cwd[255];

    getcwd(cwd, 255);

    if (chdir(dir) == 0) {
        FILE *fp = fopen(CFG_FILENAME, "rb");

        if (fp) {
            if (fread(c, sizeof(config_t), 1, fp) == 1) {
                result = true;
            }

            fclose(fp);
        }
    }

    chdir(cwd);

    return result;
}

bool Configuration::save(const char* dir, config_t* c) {
    bool result = false;
    char cwd[255];

    getcwd(cwd, 255);

    if (chdir(dir) != 0) {
#ifdef _WIN32
        mkdir(dir);
#else
        mkdir(dir, 0755);
#endif
    }

    if (chdir(dir) == 0) {
        FILE *fp = fopen(CFG_FILENAME, "wb");

        if (fp) {
            if (fwrite(c, sizeof(config_t), 1, fp) == 1) {
                result = true;
            }

            fclose(fp);
        }
    }

    chdir(cwd);

    return result;
}

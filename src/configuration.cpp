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

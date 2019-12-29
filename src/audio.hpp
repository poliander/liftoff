#pragma once

#include <SDL2/SDL_mixer.h>

class Audio {
    public:
        Audio();
        ~Audio();

        int         volume_sfx;
        int         volume_music;
        int         mixer_frequency;

        void        init(char* data_dir, int vol_sfx, int vol_music, int mix_freq);

        // sound effects
        Mix_Chunk*  sample[32];
        Mix_Chunk*  loadSample(const char* filename);

        void        playSample(int id, int vol, int pos);
        void        playSampleLoop(int id, int fadetime);
        void        stopSampleLoop(int fadetime);
        void        updatePosition(float player_x);

        // music
        Mix_Music*  music[2];
        Mix_Music*  loadMusic(const char* filename);

        void        playMusic(int id, int fadetime);
        void        stopMusic(int fadetime);

    private:
        int         state_background_sound;
        char        resource_dir[255];
};

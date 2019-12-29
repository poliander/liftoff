#include "audio.hpp"

Audio::Audio() {
    volume_sfx = 0;
    volume_music = 0;

    state_background_sound = -1;
}

Audio::~Audio() {
}

void Audio::init(char* data_dir, int vol_sfx, int vol_music, int mix_freq) {
    volume_sfx = vol_sfx;
    volume_music = vol_music;
    mixer_frequency = mix_freq;
    snprintf(resource_dir, sizeof(resource_dir), "%s/sounds", data_dir);
    Mix_AllocateChannels(32);
}

Mix_Chunk* Audio::loadSample(const char *filename) {
    Mix_Chunk *tmp = NULL;
    char f[255];

    if (volume_sfx > 0) {
        snprintf(f, sizeof(f), "%s/%s", resource_dir, filename);
        tmp = Mix_LoadWAV(f);
    }

    return(tmp);
}

void Audio::playSample(int id, int v, int pos) {
    int res;
    float vol;

    if (volume_sfx > 0) {
        res = Mix_PlayChannel(-1, sample[id], 0);
        vol = static_cast<float>(v) * (static_cast<float>(MIX_MAX_VOLUME) / 255.0f) * static_cast<float>(volume_sfx) * .333f;

        Mix_Volume(res, static_cast<int>(vol));
        Mix_SetPosition(res, pos, 100);
    }
}

void Audio::playSampleLoop(int id, int ms) {
    if (volume_sfx > 0) {
        state_background_sound = Mix_FadeInChannel(-1, sample[id], -1, ms);
    }
}

void Audio::stopSampleLoop(int fadetime) {
    if (state_background_sound != -1) {
        Mix_FadeOutChannel(state_background_sound, fadetime);
        state_background_sound = -1;
    }
}

void Audio::updatePosition(float player_x) {
    if (state_background_sound != -1) {
        int angle = static_cast<int>(.75f * player_x);

        if (angle < 0) {
            angle += 360;
        }

        Mix_SetPosition(state_background_sound, angle, 0);
    }
}

Mix_Music* Audio::loadMusic(const char* filename) {
    Mix_Music* tmp = NULL;
    char f[255];

    if (volume_music > 0) {
        snprintf(f, sizeof(f), "%s/%s", resource_dir, filename);
        tmp = Mix_LoadMUS(f);
    }

    return tmp;
}

void Audio::playMusic(int id, int fadetime) {
    float vol;

    if (volume_music > 0) {
        vol = static_cast<float>(volume_music) * static_cast<float>(MIX_MAX_VOLUME) * .125f;
        Mix_VolumeMusic(static_cast<int>(vol));

        if (fadetime > 0) {
             Mix_FadeInMusic(music[id], -1, fadetime);
        } else {
            Mix_PlayMusic(music[id], -1);
        }
    }
}

void Audio::stopMusic(int fadetime) {
    if (volume_music > 0) {
        Mix_FadeOutMusic(fadetime);
    }
}

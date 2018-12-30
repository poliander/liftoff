#include "audio.hpp"

Audio::Audio() {
    volume_sfx = 0;
    volume_music = 0;

    state_background_sound = -1;
}

Audio::~Audio() {
}

/*
 * init
 */
void Audio::init(char *data_dir, int vol_sfx, int vol_music, int mix_freq) {
    volume_sfx = vol_sfx;
    volume_music = vol_music;
    mixer_frequency = mix_freq;
    sprintf(resource_dir, "%s/sfx", data_dir);
    Mix_AllocateChannels(32);
}

/*
 * load sample
 */
Mix_Chunk *Audio::loadSample(const char *filename) {
    Mix_Chunk *tmp = NULL;
    char f[255];

    if (volume_sfx > 0) {
        sprintf(f, "%s/%s", resource_dir, filename);
        tmp = Mix_LoadWAV(f);
    }

    return(tmp);
}

/*
 *     play sample
 */
void Audio::playSample(int id, int vol, int pos) {
    int res, v;

    if (volume_sfx > 0) {
        res = Mix_PlayChannel(-1, sample[id], 0);
        v = int(vol * (float(MIX_MAX_VOLUME)/255.0f) * float(volume_sfx) * .333f);

        Mix_Volume(res, v);
        Mix_SetPosition(res, pos, 100);
    }
}

/*
 * play looping background sound
 */
void Audio::playSampleLoop(int id, int ms) {
    if (volume_sfx) {
        state_background_sound = Mix_FadeInChannel(-1, sample[id], -1, ms);
    }
}

/*
 * stop looping background sound
 */
void Audio::stopSampleLoop(int fadetime) {
    if (state_background_sound != -1) {
        Mix_FadeOutChannel(state_background_sound, fadetime);
        state_background_sound = -1;
    }
}

/*
 * update background sound position
 */
void Audio::updatePosition(float player_x) {
    int angle;

    // player's ship engine
    if (state_background_sound != -1) {
            angle = int(.75f * player_x);
            if (angle < 0) angle += 360;
            Mix_SetPosition(state_background_sound, angle, 0);
    }
}

/*
 * load background music
 */
Mix_Music *Audio::loadMusic(const char *filename) {
    Mix_Music *tmp = NULL;
    char f[255];

    if (volume_music) {
        sprintf(f, "%s/%s", resource_dir, filename);
        tmp = Mix_LoadMUS(f);
    }

    return(tmp);
}

/*
 * play background music
 */
void Audio::playMusic(int id, int fadetime) {
    int vol;

    if (volume_music) {
        vol = int(float(volume_music) * float(MIX_MAX_VOLUME) * .125f);
        Mix_VolumeMusic(vol);

        if (fadetime > 0) {
             Mix_FadeInMusic(music[id], -1, fadetime);
        } else {
            Mix_PlayMusic(music[id], -1);
        }
    }
}

/*
 * stop music playback
 */
void Audio::stopMusic(int fadetime) {
    if (volume_music) {
        Mix_FadeOutMusic(fadetime);
    }
}

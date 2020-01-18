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

#include <SDL2/SDL_mixer.h>

class Audio {
    public:
        Audio();
        ~Audio() {}

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

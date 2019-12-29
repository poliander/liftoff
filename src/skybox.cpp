#include "skybox.hpp"

Skybox::Skybox(State* s) : state(s) {
    view = View::createPerspective(65.0f, 1.0f, 1.0f, 10000.0f);
    framebuffer = make_unique<Framebuffer>(state->vid_fb_size, state->vid_fb_size, 0);

    for (int i = 0; i < SKYBOX_NUM_STARS; i++) {
        float x = 0;
        float y = 0;

        while ((fabs(x) < 50.0f) && (fabs(y) < 50.0f)) {
            x = .1f * (rand() % 2000 - 1000);
            y = .1f * (rand() % 2000 - 1000);
        }

        stars[i][0] = x;
        stars[i][1] = y;
        stars[i][2] = -1000.0f + (rand() % 1000);

        if (i > (SKYBOX_NUM_STARS - SKYBOX_NUM_STARS_WARP)) {
            stars[i][3] = 90.0f + (atan(stars[i][1] / stars[i][0]) * 180.0f/ M_PI);
        } else  {
            stars[i][3] = 0.35f + (((float)(rand() % 100)) * .005f);
        }
    }
}

Skybox::~Skybox() {
}

void Skybox::update() {
    for (int i = 0; i < SKYBOX_NUM_STARS; i++) {
        if (i > (SKYBOX_NUM_STARS - SKYBOX_NUM_STARS_WARP)) {
            stars[i][2] += state->global_timer * state->stars_speed * 1.25f;
        } else {
            stars[i][2] += state->global_timer * state->stars_speed * 0.25f;
        }

        if (stars[i][2] > 0) {
            stars[i][2] -= 1000.0f;
        }
    }

    state->stars_rotation_pos -= state->global_timer * state->stars_rotation_speed;
}

void Skybox::draw() {
    float a, c, s = max(static_cast<float>(state->vid_height) / static_cast<float>(state->vid_width), state->vid_aspect);

    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);

    framebuffer->bind();
    framebuffer->clear();

    state->shaders[S_TEXTURE]->bind();

    // background

    state->shaders[S_TEXTURE]->update(UNI_COLOR, glm::vec4(.65f, .7f, .8f, 1.0f));
    state->shaders[S_TEXTURE]->update(UNI_MVP, glm::ortho(-0.5f, 0.5f, -0.5f, 0.5f));

    state->textures[T_BACKGROUND_1]->bind();
    state->textures[T_BACKGROUND_1]->draw();

    // far stars

    state->textures[T_STAR]->bind();

    for (int i = 0; i < (SKYBOX_NUM_STARS - SKYBOX_NUM_STARS_WARP); ++i) {
        state->shaders[S_TEXTURE]->update(UNI_COLOR, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        state->shaders[S_TEXTURE]->update(UNI_MVP, view->getProjection() * view->getModel(
            stars[i][0], stars[i][1], stars[i][2],
            0, 0, 0,
            stars[i][3], stars[i][3], 0
        ));

        state->textures[T_STAR]->draw();
    }

    // warp stars

    if (state->stars_warp) {
        for (int i = (SKYBOX_NUM_STARS - SKYBOX_NUM_STARS_WARP); i < SKYBOX_NUM_STARS; ++i) {
            a = (1000.0f + stars[i][2]) / 1250.0f;
            float sl = pow(a * 1.45f, 2) * (1.0f / isqrt(pow(stars[i][0], 2) + pow(stars[i][1], 2)));

            state->shaders[S_TEXTURE]->update(UNI_COLOR, glm::vec4(1.0f, 1.0f, 1.0f, a * (state->stars_speed - .3f)));
            state->shaders[S_TEXTURE]->update(UNI_MVP, view->getProjection() * view->getModel(
                stars[i][0], stars[i][1], stars[i][2],
                90.0f, stars[i][3], 0,
                .9f, sl * (state->stars_speed - .3f), 0
            ));

            state->textures[T_STAR]->draw();
        }
    }

    framebuffer->unbind();

    state->shaders[S_TEXTURE]->update(UNI_COLOR, glm::vec4(
        state->global_alpha,
        state->global_alpha,
        state->global_alpha,
        1.0f
    ));

    state->shaders[S_TEXTURE]->update(UNI_MVP, state->view->transform(
        0, 0, -45.0f,
        0, 0, state->stars_rotation_pos,
        75.0f * s, 75.0f * s, 0
    ));

    framebuffer->draw();
    state->shaders[S_TEXTURE]->unbind();

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
}

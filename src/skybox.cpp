#include "skybox.hpp"

Skybox::Skybox(State& s) : state(s)
{
    view = View::createPerspective(65.0f, 1.0f, .01f, 10000.0f);
    framebuffer = make_unique<Framebuffer>(state.vid_framebuffer_size, GL_RGB);

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

Skybox::~Skybox()
{
}

void Skybox::update()
{
    for (int i = 0; i < SKYBOX_NUM_STARS; i++) {
        if (i > (SKYBOX_NUM_STARS - SKYBOX_NUM_STARS_WARP)) {
            stars[i][2] += state.timer_adjustment * state.stars_speed * 1.25f;
        } else {
            stars[i][2] += state.timer_adjustment * state.stars_speed * 0.25f;
        }

        if (stars[i][2] > 0) {
            stars[i][2] -= 1000.0f;
        }
    }

    if (state.stars_rotation) {
        state.stars_rotation_pos -= state.timer_adjustment * state.stars_rotation_speed;
    }
}

void Skybox::draw()
{
    unsigned short i;
    float a, c, sl;

    glDepthMask(GL_FALSE);

    framebuffer->bind();

    glClearColor(0, 0, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    state.shaders[S_TEXTURE]->bind();

    // background

    state.shaders[S_TEXTURE]->update(UNI_COLOR, glm::vec4(.65f, .7f, .8f, 1.0f));
    state.shaders[S_TEXTURE]->update(UNI_MVP, glm::ortho(-0.5f, 0.5f, -0.5f, 0.5f));

    state.textures[T_BACKGROUND_1]->bind();
    state.textures[T_BACKGROUND_1]->draw();

    // far stars

    state.textures[T_STAR]->bind();

    for (i = 0; i < (SKYBOX_NUM_STARS - SKYBOX_NUM_STARS_WARP); ++i) {
        state.shaders[S_TEXTURE]->update(UNI_COLOR, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        state.shaders[S_TEXTURE]->update(UNI_MVP, view->getProjection() * view->getModel(
            stars[i][0], stars[i][1], stars[i][2],
            0, 0, 0,
            stars[i][3], stars[i][3], 0
        ));

        state.textures[T_STAR]->draw();
    }

    // warp stars

    if (state.stars_warp) {
        for (i = (SKYBOX_NUM_STARS - SKYBOX_NUM_STARS_WARP); i < SKYBOX_NUM_STARS; ++i) {
            a = (1000.0f + stars[i][2]) / 1250.0f;
            sl = pow(a * 1.45f, 2) * (1.0f / isqrt(pow(stars[i][0], 2) + pow(stars[i][1], 2)));

            state.shaders[S_TEXTURE]->update(UNI_COLOR, glm::vec4(1.0f, 1.0f, 1.0f, a * (state.stars_speed - .3f)));
            state.shaders[S_TEXTURE]->update(UNI_MVP, view->getProjection() * view->getModel(
                stars[i][0], stars[i][1], stars[i][2],
                90.0f, stars[i][3], 0,
                .9f, sl * (state.stars_speed - .3f), 0
            ));

            state.textures[T_STAR]->draw();
        }
    }

    framebuffer->unbind();

    state.shaders[S_TEXTURE]->update(UNI_COLOR, glm::vec4(
        state.global_alpha * .01f,
        state.global_alpha * .01f,
        state.global_alpha * .01f,
        1.0f
    ));

    state.shaders[S_TEXTURE]->update(UNI_MVP, state.view->transform(
        0, 0, -50.0f,
        0, 0, state.stars_rotation_pos,
        135.0f, 135.0f, 0
    ));

    framebuffer->draw();

    state.shaders[S_TEXTURE]->unbind();

    glDepthMask(GL_TRUE);
}

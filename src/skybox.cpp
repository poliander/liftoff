#include <skybox.hpp>

Skybox::Skybox() : texture(new Texture())
{
    int i;
    float x, y;

    for (i = 0; i < SKYBOX_NUM_STARS; i++) {
        x = 0;
        y = 0;

        while ((fabs(x) < 50.0f) && (fabs(y) < 50.0f)) {
            x = .1f * (rand() % 2000 - 1000);
            y = .1f * (rand() % 2000 - 1000);
        }

        if (i > (SKYBOX_NUM_STARS - SKYBOX_NUM_STARS_WARP)) {
            stars[i][0] = x;
            stars[i][1] = y;
            stars[i][2] = -1000.0f + (rand() % 1000);
            stars[i][3] = 90.0f + (atan(stars[i][1] / stars[i][0]) * 180.0f/ M_PI);
        } else  {
            stars[i][0] = x;
            stars[i][1] = y;
            stars[i][2] = -1000.0f + (rand() % 1000);
            stars[i][3] = (float)((rand() % 100) * .005f) + .35f;
        }
    }
}

Skybox::~Skybox()
{
}

void Skybox::move(State &s)
{
    for (int i = 0; i < SKYBOX_NUM_STARS; i++) {
        if (i > (SKYBOX_NUM_STARS - SKYBOX_NUM_STARS_WARP)) {
            stars[i][2] += s.timer_adjustment * s.stars_speed * 1.25f;
        } else {
            stars[i][2] += s.timer_adjustment * s.stars_speed * 0.25f;
        }

        if (stars[i][2] > 0) {
            stars[i][2] -= 1000.0f;
        }
    }

    if (s.stars_rotation) {
        s.stars_rotation_pos -= s.timer_adjustment * s.stars_rotation_speed;
    }
}

void Skybox::draw(State &s)
{
    unsigned short i;
    float a, c, sl;

    glDepthMask(GL_FALSE);

    texture->bindFrameBuffer();

    glClearColor(0, 0, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    s.shaders[S_TEXTURE_1]->bind();

    // background

    s.textures[T_BACKGROUND_1]->bind();

    s.shaders[S_TEXTURE_1]->update(UNI_COLOR, glm::vec4(.65f, .7f, .8f, 1.0f));
    s.shaders[S_TEXTURE_1]->update(UNI_MVP, s.view.transform2D(
        0, 0, -1.0f,
        0, 0, 0,
        1.35f, 1.35f, 0
    ));

    s.textures[T_BACKGROUND_1]->draw();

    // far stars

    s.textures[T_STAR]->bind();

    for (i = 0; i < (SKYBOX_NUM_STARS - SKYBOX_NUM_STARS_WARP); ++i) {
        s.shaders[S_TEXTURE_1]->update(UNI_COLOR, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        s.shaders[S_TEXTURE_1]->update(UNI_MVP, s.view.transform2D(
            stars[i][0], stars[i][1], stars[i][2],
            0, 0, 0,
            stars[i][3], stars[i][3], 0
        ));

        s.textures[T_STAR]->draw();
    }

    // warp stars

    if (s.stars_warp) {
        for (i = (SKYBOX_NUM_STARS - SKYBOX_NUM_STARS_WARP); i < SKYBOX_NUM_STARS; ++i) {
            a = (1000.0f + stars[i][2]) / 1250.0f;
            sl = pow(a * 1.45f, 2) * (1.0f / isqrt(pow(stars[i][0], 2) + pow(stars[i][1], 2)));

            s.shaders[S_TEXTURE_1]->update(UNI_COLOR, glm::vec4(1.0f, 1.0f, 1.0f, a * (s.stars_speed - .3f)));
            s.shaders[S_TEXTURE_1]->update(UNI_MVP, s.view.transform2D(
                stars[i][0], stars[i][1], stars[i][2],
                90.0f, stars[i][3], 0,
                .9f, sl * (s.stars_speed - .3f), 0
            ));

            s.textures[T_STAR]->draw();
        }
    }

    s.shaders[S_TEXTURE_1]->unbind();

    texture->unbindFrameBuffer();

    texture->bind();

    s.shaders[S_TEXTURE_1]->bind();
    s.shaders[S_TEXTURE_1]->update(UNI_COLOR, glm::vec4(s.global_alpha * .01f, s.global_alpha * .01f, s.global_alpha * .01f, 1.0f));
    s.shaders[S_TEXTURE_1]->update(UNI_MVP, s.view.transform(
        0, 0, -50.0f,
        0, 0, s.stars_rotation_pos,
        135.0f, 135.0f, 0
    ));

    texture->draw();

    s.shaders[S_TEXTURE_1]->unbind();

    glDepthMask(GL_TRUE);
}

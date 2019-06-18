#include <skybox.hpp>

Skybox::Skybox()
{
    int i;
    float x, y;

    // far stars
    for (i = 0; i < (num_stars - num_stars_warp); i++) {
        x = 0;
        y = 0;

        while ((fabs(x) < 200.0f) && (fabs(y) < 200.0f)) {
            x = (float)(rand() % 800 - 400);
            y = (float)(rand() % 800 - 400);
        }

        stars[i][0] = x;
        stars[i][1] = y;
        stars[i][2] = (float)(rand() % 5000);
        stars[i][3] = (float)((rand() % 100) * .01f);
    }

    // warp stars
    for (i = (num_stars - num_stars_warp); i < num_stars; i++) {
        x = 0;
        y = 0;

        while ((fabs(x) < 25.0f) && (fabs(y) < 25.0f)) {
            x = (float)(rand() % 400 - 200);
            y = (float)(rand() % 400 - 200);
        }

        stars[i][0] = x;
        stars[i][1] = y;
        stars[i][2] = (float)(rand() % 5000);
        stars[i][3] = (float)((rand() % 100) * .01f);
    }
}

Skybox::~Skybox()
{
}

void Skybox::move(State &s)
{
    for (int i = 0; i < num_stars; i++) {
        stars[i][2] += s.timer_adjustment * s.stars_speed;

        if (stars[i][2] > 300.0f) {
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
    float a, c, sl, sa;

    glShadeModel(GL_FLAT);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDisable(GL_CULL_FACE);

    // background
    glBindTexture(GL_TEXTURE_2D, *s.textures[T_BACKGROUND_1]);
    glRotatef(s.stars_rotation_pos, 0, 0, 1);
    glPushMatrix();
    glTranslatef(0, 0, 0);
    glColor4f(.65f, .7f, .8f, s.global_alpha * .01f);
    glBegin (GL_QUADS);
      glTexCoord2i (0, 0);
      glVertex2i (-260, 260);

      glTexCoord2i (1, 0);
      glVertex2i (260, 260);

      glTexCoord2i (1, 1);
      glVertex2i (260, -260);

      glTexCoord2i (0, 1);
      glVertex2i (-260, -260);
    glEnd();
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, *s.textures[T_STAR]);

    // far stars
    for (i = 0; i < (num_stars - num_stars_warp); ++i) {
        c = stars[i][3];
        a = ((500.0f+stars[i][2])/250.0f) * (.0075f * s.global_alpha);

        glPushMatrix();
        glTranslatef(stars[i][0], stars[i][1], stars[i][2]);

        glColor4f(c, c, c, a*.75f);
        c = .75f + c*1.5f;
        glBegin(GL_QUADS);
          glTexCoord2i(0, 0);
          glVertex3f(-c, -c, 0);

          glTexCoord2i(1, 0);
          glVertex3f(c, -c, 0);

          glTexCoord2i(1, 1);
          glVertex3f(c, c, 0);

          glTexCoord2i(0, 1);
          glVertex3f(-c, c, 0);
        glEnd();

        glPopMatrix();
    }

    // warp stars
    if (s.stars_warp) {
        for (i = (num_stars - num_stars_warp); i < num_stars; ++i) {
            c = stars[i][3];
            a = ((500.0f+stars[i][2])/250.0f) * (.0075f * s.global_alpha);
            sl = (a*3.5f) * (a*3.5f) * (s.stars_speed - .3f);
            sa = .6f * (s.stars_speed - .3f);

            glPushMatrix();
            glTranslatef(stars[i][0], stars[i][1], stars[i][2]);

            glBegin (GL_QUADS);
              glColor4f(a, a, a, sa);

              glTexCoord2i(1, 0);
              glVertex3f(-1, -1, -sl);

              glTexCoord2i(1, 1);
              glVertex3f (1, -1, -sl);

              glColor4f(c*a, c*a, c*a, .35f*sa);

              glTexCoord2i(0, 1);
              glVertex3f (1, 1, sl);

              glTexCoord2i(0, 0);
              glVertex3f (-1, 1, sl);
            glEnd();
            glPopMatrix();
        }
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);
}

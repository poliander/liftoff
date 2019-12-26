#include "cargo.hpp"

Cargo::Cargo() : Entity()
{
    e_obj = OBJ_CARGO_1;
    e_type = E_TYPE_COLLIDER;
    e_state = E_STATE_IDLE;

    counter = .01f;
    focusable = true;
    money = 50;
}

Cargo::~Cargo()
{
}

bool Cargo::damage(State &s, int p)
{
    bool damaged = Entity::damage(s, p);

    if (damaged && e_state == E_STATE_GONE && yield == false) {
        s.audio.playSample(SFX_EXPLOSION_1, 192, 180);

        s.spawn(make_shared<Explosion>(OBJ_EXPLOSION_3, p_x, p_y, p_z, .6f, .75f, 1.0f));
        s.spawn(make_shared<Powerup>(p_x, p_y, p_z));

        s.notify(MSG_MONEY, money);
        e_state = E_STATE_FADING;
        yield = true;
    }

    return damaged;
}

void Cargo::update(State &s)
{
    Entity::update(s);

    if (e_state == E_STATE_FADING) {
        counter += s.global_timer * .2f;
    }

    if (p_z > 0 || counter > 5.0f) {
        e_state = E_STATE_GONE;
    }
}

void Cargo::draw(State &s)
{
    float a = s.global_alpha, d = calcDistanceScale(s);

    glm::vec4 color = glm::vec4(a, a, a, a);
    glm::mat4 projection = s.view->getProjection();
    glm::mat4 camera = s.view->getCamera();
    glm::mat4 m, model = s.view->getModel(
        (getPosX() - s.cam_x) * E_RELATIVE_MOVEMENT,
        (getPosY() - s.cam_y) * E_RELATIVE_MOVEMENT,
        getPosZ(),

        getRotX(),
        getRotY(),
        getRotZ(),

        getScaleX() * d,
        getScaleY() * d,
        getScaleZ() * d
    );

    s.shaders[S_TEXTURE]->bind();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);

    if (e_state == E_STATE_FADING) {
        float scale = -1.0f / (.2f * counter + .2f) + 4.5f;

        m = s.view->getModel(
            (getPosX() - s.cam_x) * E_RELATIVE_MOVEMENT,
            (getPosY() - s.cam_y) * E_RELATIVE_MOVEMENT,
            (getPosZ()),

            0, 0, 0,

            d * scale * 150.0f,
            d * scale * 150.0f,
            0
        );

        s.textures[T_HALO]->bind();
        s.shaders[S_TEXTURE]->update(UNI_COLOR, glm::vec4(.8f * a, .9f * a, 1.0f * a, a * (1.0f - (counter * .2f))));
        s.shaders[S_TEXTURE]->update(UNI_MVP, projection * camera * m);
        s.textures[T_HALO]->draw();
    } else {
        s.textures[T_GLOW]->bind();

        m = model;
        m = glm::rotate(m, glm::radians(90.0f), glm::vec3(1.0f, 0, 0));
        m = glm::scale(m, glm::vec3(15.0f, 15.0f, 0));

        s.shaders[S_TEXTURE]->update(UNI_COLOR, glm::vec4(.8f * a, .9f * a, 1.0f * a, 0.45f * a));
        s.shaders[S_TEXTURE]->update(UNI_MVP, projection * camera * m);
        s.textures[T_GLOW]->draw();

        m = model;
        m = glm::rotate(m, glm::radians(90.0f), glm::vec3(0, 1.0f, 0));
        m = glm::scale(m, glm::vec3(15.0f, 15.0f, 0));

        s.shaders[S_TEXTURE]->update(UNI_MVP, projection * camera * m);
        s.textures[T_GLOW]->draw();

        m = model;
        m = glm::rotate(m, glm::radians(90.0f), glm::vec3(0, 0, 1.0f));
        m = glm::scale(m, glm::vec3(15.0f, 15.0f, 0));

        s.shaders[S_TEXTURE]->update(UNI_MVP, projection * camera * m);
        s.textures[T_GLOW]->draw();
    }

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    s.shaders[S_TEXTURE]->unbind();

    if (e_state == E_STATE_ACTIVE) {
        s.models[e_obj]->draw(model, camera, projection, color);
    }
}

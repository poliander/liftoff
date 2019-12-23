#include "cargo.hpp"

Cargo::Cargo() : Entity()
{
    e_obj = OBJ_CARGO_1;
    e_type = E_TYPE_COLLIDER;
    e_state = E_STATE_IDLE;

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

        s.notify(MSG_MONEY, money);
        s.player->addMoney(money);

        s.spawn(make_shared<Explosion>(OBJ_EXPLOSION_3, p_x, p_y, p_z, .6f, .75f, 1.0f));
        s.spawn(make_shared<Powerup>(p_x, p_y, p_z));

        yield = true;
    }

    return damaged;
}

void Cargo::update(State &s)
{
    Entity::update(s);

    counter += s.global_timer * .1f;

    if (p_z > 0) {
        e_state = E_STATE_GONE;
    }
}

void Cargo::draw(State &s)
{
    float a = float(s.global_alpha) * .01f, d = calcDistanceScale(s);

    glm::vec4 color = glm::vec4(a, a, a, 1.0f);
    glm::mat4 m;

    glm::mat4 projection = s.view->getProjection();
    glm::mat4 camera = s.view->getCamera();
    glm::mat4 model = s.view->getModel(
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

    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glDepthMask(GL_FALSE);

    s.textures[T_GLOW]->bind();

    s.shaders[S_TEXTURE]->bind();
    s.shaders[S_TEXTURE]->update(UNI_COLOR, glm::vec4(.8f * a, .9f * a, 1.0f * a, 0.45f * a));

    m = model;
    m = glm::rotate(m, glm::radians(90.0f), glm::vec3(1.0f, 0, 0));
    m = glm::scale(m, glm::vec3(15.0f, 15.0f, 0));

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

    s.shaders[S_TEXTURE]->unbind();

    glDepthMask(GL_TRUE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    s.models[e_obj]->draw(model, camera, projection, color);
}

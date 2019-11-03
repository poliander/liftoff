#include "missile.hpp"

Missile::Missile() : Entity()
{
    e_obj = OBJ_MISSILE_1;
    e_type = E_TYPE_COLLIDER;
    e_state = E_STATE_ACTIVE;

    setScale(2.25f, 2.25f, 2.25f);
    setRot(90.0f, 0, 0);

    c_r = 0.5f;
    c_g = 1.0f;
    c_b = 0.8f;
    c_a = 1.0f;

    v_x = 0;
    v_y = 0;
    v_z = -(E_BASE_SPEED + 100.0f);

    power = 20;
}

Missile::~Missile()
{
}

void Missile::collide(State &s, shared_ptr<Entity> e)
{
    if (e->isPlayer()) {
        return;
    }

    if (e->damage(s, power)) {
        s.audio.playSample(SFX_GUN_IMPACT, 192, 180);
        s.entities.push_back(make_shared<Explosion>(OBJ_EXPLOSION_1, getPosX(), getPosY(), getPosZ()));

        e_state = E_STATE_GONE;
    }
}

void Missile::update(State &s)
{
    Entity::update(s);

    c_a = (s.global_alpha * .005f) + ((getPosZ() + 200.0f) * .00002f);

    if (c_a < 0 || getPosZ() < -10000.0f) {
        e_state = E_STATE_GONE;
    }
}

void Missile::draw(State &s)
{
    float a = float(s.global_alpha) * .01f;

    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    s.shaders[S_TEXTURE]->bind();
    s.shaders[S_TEXTURE]->update(UNI_COLOR, glm::vec4(c_r * a, c_g * a, c_b * a, c_a * a));

    for (int i = 0; i < 5; i++) {
        s.shaders[S_TEXTURE]->update(UNI_MVP, s.view.transform(
            (getPosX() - s.cam_x) * E_RELATIVE_MOVEMENT,
            (getPosY() - s.cam_y) * E_RELATIVE_MOVEMENT,
            getPosZ(),

            getRotX(),
            getRotY() + float(i) * 30.0f,
            getRotZ(),

            20.0f,
            250.0f,
            0
        ));

        s.textures[T_MISSILE_1]->draw();
    }

    s.shaders[S_TEXTURE]->unbind();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
}

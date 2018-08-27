#ifndef PLAYER_H
#define PLAYER_H

#include "mat.h"
#include "vec.h"
#include "mesh.h"
#include "wavefront.h"
#include "camera.h"
#include "collider.h"
#include "program.h"
#include "texture.h"

class Player: public Mesh {

public:
    Player();

    void turnLeft();
    void turnRight();
    void accelerate(float velo);
    void decelerate(float velo);
    void init(Point origine, Vector direction, Vector lateral, float rayon, int width, int height, int fov);

    void quit();
    void boostPlayer(int velocityMax, float time, float duration);
    Collider& getCollisionBox();

    void render(vec4 sky);

    float getVelocity();
    Transform getPosition();
    void movePlayer(Transform playerPos);
    Camera & getCamera();
    int getAngularVelocity();
    Point& getCurrentPos();
    bool decreasePlayerLife();
    int getLife();
    Transform getOffset();
    void boost();
    int getAcc();
    void setAcc(int val);

private:
    float m_velocity;
    float m_current_velocity;
    int m_angular_velocity;
    Transform m_position;
    Camera m_player_camera;
    Transform m_offset;
    Point m_current_pos;
    Vector m_direction;
    Vector m_normal;
    float m_acc_duration;
    float m_current_acc_duration;
    int m_velocity_max;
    int m_angle_rotation;
    Collider m_collision_box;
    GLuint m_program;
    GLuint m_texture;
    int m_playerLife;
};

#endif
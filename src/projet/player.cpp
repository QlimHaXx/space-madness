#include "player.h"
#include "uniforms.h"

#include <math.h> 

Player::Player() : Mesh(read_mesh("data/vaisseau.obj"))
{
    m_velocity = 0.18;
    m_angular_velocity = 2;
    m_current_velocity = m_velocity;
    m_current_acc_duration = 0;
    m_angle_rotation = 0;
    m_acc_duration = -1;
}

void Player::init(Point origine, Vector direction, Vector lateral, float rayon, int width, int height, int fov){
    Vector translate = Vector(Point(), origine);
    Vector axe = cross(Vector(0,0,-1), direction);
    float angle = degrees(acos(dot(Vector(0,0,-1), direction)));
    m_offset = Translation(Vector(0, rayon, 0));

    Transform playerPos = Translation(translate) * Rotation(axe, angle) * m_offset;
    m_texture= read_texture(0, "data/texture.png");
    m_program = read_program("data/shaders/shader_player.glsl");
    program_print_errors(m_program);
    m_position = playerPos;
    m_player_camera.init(width, height, fov, m_position);

    m_collision_box.init(*this, m_position, false);
    m_playerLife = 6;
}

void Player::turnLeft()
{
    m_angle_rotation += m_angular_velocity;
}

void Player::turnRight()
{
    m_angle_rotation -= m_angular_velocity;
}
    
void Player::accelerate(float vel)
{
    m_velocity = m_velocity + vel;
}

void Player::decelerate(float vel){
    if(m_velocity - vel > 1){
        m_velocity = m_velocity - vel;
    }
}

float Player::getVelocity()
{
    return m_velocity;
}

int Player::getAngularVelocity()
{
    return m_angular_velocity;
}

Transform Player::getPosition()
{
    return m_position;
}

void Player::movePlayer(Transform playerPos){
    m_position = playerPos;
    m_position = m_position * Inverse(m_offset) * RotationZ(m_angle_rotation) * m_offset;
    m_collision_box.update(m_position);
    m_player_camera.setPosition(m_position);
    m_player_camera.update();
}

Camera & Player::getCamera(){
    return m_player_camera;
}

void Player::quit()
{
    glDeleteTextures(1, &m_texture);
    release_program(m_program);
    release();
}

Point& Player::getCurrentPos()
{
	return m_current_pos;
}

void Player::boostPlayer(int velocityMax, float time, float duration){
    m_velocity_max = velocityMax;
    m_current_acc_duration = 0;
    m_acc_duration = duration;
}

Collider& Player::getCollisionBox()
{
    return m_collision_box;
}

void Player::render(vec4 skyColor)
{
    glUseProgram(m_program);
    program_uniform(m_program, "transformationMatrix", m_position);
    program_uniform(m_program, "viewMatrix", m_player_camera.getPosition());
    program_uniform(m_program, "projectionMatrix", m_player_camera.getProjection());
    program_uniform(m_program, "skyColor", skyColor);
    //program_uniform(m_program, "color", vec4(1,0.5,0));
    program_use_texture(m_program, "texture0", 0, m_texture);
    draw(m_program, true, true, true, false);
}

bool Player::decreasePlayerLife()
{
    if(m_playerLife > 1)
    {  
        m_playerLife -=1;
        return true;
    }
    return false;
}

int Player::getLife()
{
    return m_playerLife;
}

void Player::boost(){
    int fov = 1;
    if(m_current_acc_duration >= 0 && m_current_acc_duration < 50){
        m_player_camera.addCameraFOV(fov);
        m_current_acc_duration++;
        accelerate(0.0005);
        //std::cout << m_current_acc_duration << std::endl;
    }
    else if(m_current_acc_duration >= 50 && m_current_acc_duration < 100){
        decelerate(0.0005);
        m_player_camera.reduceCameraFOV(fov);
        m_current_acc_duration++;
    }
    else if(m_current_acc_duration >= 100){
        m_current_acc_duration = 0;
        m_acc_duration = -1;
    }
}

int Player::getAcc(){
    return m_acc_duration;
}

void Player::setAcc(int val){
    m_acc_duration = val;
}

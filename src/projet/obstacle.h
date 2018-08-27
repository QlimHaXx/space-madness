#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "mesh.h"
#include "wavefront.h"
#include "program.h"
#include "collider.h"

#include "player.h"

class Obstacle : Mesh {
protected:
	Transform m_position;
	Transform m_offset;
	Mesh m_model;
	GLuint m_program;
	Collider m_collision_box;
	int m_type;
public:
	Obstacle();
	void init(int type, Point pos, Point p1, Vector v, int offset);
	Transform getPosition();
	void setPosition(Point p, Point p1, Vector v);
	Mesh & getModel();
	GLuint &getShader();
	void readShader();
	Collider& getCollisionBox();
	void render(vec4 skyColor, Player & player);
	void moveObstacle(Point p, Point p1, Vector v);
	void quit();
	int getType();
};

#endif

#include "uniforms.h"
#include "obstacle.h"

Obstacle::Obstacle(){
}

void Obstacle::init(int type, Point p, Point p1, Vector v, int offset){
	m_program = read_program("data/shaders/shader_obstacle.glsl");
	program_print_errors(m_program);

	if(type == 0){ // pic
		m_type = 0;
		m_model = Mesh(read_mesh("data/obstacle.obj"));
		m_offset = Translation(Vector(0, offset, 0));
		Vector direction = normalize(Vector(p - p1));
    	Vector Oy = cross(direction, v);

		m_position = Transform(v, Oy, direction, Vector());
    	m_position = Rotation(direction, rand()%360) * m_position;
    	m_position = m_position * m_offset;
    	m_position = Translation(p-Point()) * m_position;

		m_collision_box.init(*this, m_position, true);
	}
	if(type == 1){ // boost
		m_type = 1;
		m_model = Mesh(read_mesh("data/boost.obj"));
		m_offset = Translation(Vector(0, offset+0.5, 0));
		Mesh(read_mesh("data/boost.obj"));
		Vector direction = normalize(Vector(p - p1));
    	Vector Oy = cross(direction, v);

		m_position = Transform(v, Oy, direction, Vector());
    	m_position = Rotation(direction, rand()%360) * m_position;
    	m_position = m_position * m_offset;
    	m_position = Translation(p-Point()) * m_position;

		m_collision_box.init(*this, m_position, true);
	}	
}

Transform Obstacle::getPosition(){
	return m_position;
}

void Obstacle::setPosition(Point p, Point p1, Vector v){
	Vector direction = normalize(Vector(p - p1));
	Vector Oy = cross(direction, v);

	m_position = Transform(v, Oy, direction, Vector());
	m_position = Rotation(direction, rand()%360) * m_position;
	m_position = m_position * m_offset;
	m_position = Translation(p-Point()) * m_position;
}

Mesh & Obstacle::getModel(){
	return m_model;
}
GLuint & Obstacle::getShader(){
	return m_program;
}

void Obstacle::render(vec4 skyColor, Player & player){
	Transform view= player.getCamera().getPosition();
    Transform projection= player.getCamera().getProjection();

    glUseProgram(m_program);
    program_uniform(m_program, "transformationMatrix", m_position);
    program_uniform(m_program, "viewMatrix", view);
    program_uniform(m_program, "projectionMatrix", projection);
    program_uniform(m_program, "skyColor", skyColor);
    m_model.draw(m_program, true, false, true, false);
}

void Obstacle::moveObstacle(Point p, Point p1, Vector v){
	Vector direction = normalize(Vector(p - p1));
	Vector Oy = cross(direction, v);

	m_position = Transform(v, Oy, direction, Vector());
	m_position = Rotation(direction, rand()%360) * m_position;
	m_position = m_position * m_offset;
	m_position = Translation(p-Point()) * m_position;

	m_collision_box.update(m_position);
}

void Obstacle::quit(){
    release_program(m_program);
    release();
}

Collider& Obstacle::getCollisionBox()
{
	return m_collision_box;
}

int Obstacle::getType(){
    return m_type;
}



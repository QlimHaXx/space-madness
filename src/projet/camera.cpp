#include "camera.h"
#include "orbiter.h"

Camera::Camera(){
	m_offset = Translation(0, -2, -6);
}

void Camera::init(float width, float height, int fov, Transform playerPos){
	//float d= distance(m_center, Point(m_position.x, m_position.y, m_size));     // meme resultat plus rapide a calculer
    
    // regle near et far en fonction du centre et du rayon englobant l'objet 
    m_fov = fov;
    m_fov_init = fov;
    m_width = width;
    m_height = height;
    m_projection = Perspective(fov, width / height, 3.6f, 1000.0f);
	m_position = m_offset * Inverse(playerPos);
}

Transform Camera::getPosition(){
	return m_position;
}

Transform Camera::getProjection(){
	return m_projection;
}

void Camera::setPosition(Transform t){
	m_position = m_offset * Inverse(t);
}

Transform Camera::getOffset(){
	return m_offset;
}

void Camera::addCameraFOV(float fov){
	m_fov += fov;
}

void Camera::reduceCameraFOV(float fov){
	if(m_fov >= m_fov_init){
		m_fov -= fov;
	}
}

void Camera::update() {
	//std::cout << m_fov << std::endl;
	m_projection = Perspective(m_fov, m_width / (float)m_height, 0.1f, 1000);
}

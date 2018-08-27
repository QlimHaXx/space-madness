#ifndef _CAMERA_H
#define _CAMERA_H

#include "mat.h"

class Camera {
protected:
	Transform m_position;
	Transform m_projection;
	Transform m_offset;
	Transform m_player_offset;
	Point m_center;
	float m_fov;
	float m_fov_init;
	int m_width;
	int m_height;
public:
	Camera();
	void init(float width, float height, int fov, Transform playerPos);
	Transform getPosition();
	Transform getProjection();
	void setPosition(Transform t);
	Transform getOffset();
	void update();
	void addCameraFOV(float fov);
	void reduceCameraFOV(float fov);
};

#endif
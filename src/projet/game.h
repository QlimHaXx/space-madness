#ifndef GAME_H
#define GAME_H

#include "pipe.h"
#include "obstacle.h"

class Game {
protected:
	Player m_player;
	std::vector<Pipe> m_pipes;
	std::vector<Obstacle> m_obstacles;
	vec4 m_skyColor;
	int m_currentPipe;
	int m_width;
	int m_height;
	bool m_newpipe;
	bool m_obstacle_moved;
	int m_last_colision;
public:
	Game();
	void init(int width, int height);
	int update();
	void render();
	int quit();

	Pipe & getPipe();
	Player & getPlayer();

	void setSkyColor(vec4 sky);
	vec4 getSkyColor();
	void managePipe();
	int manageColision();
};

#endif

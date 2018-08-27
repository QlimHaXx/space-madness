#include "game.h"

Game::Game(){}

void Game::init(int width, int height){
	m_obstacle_moved = m_newpipe = false;
	m_width = width;
	m_height = height;
	m_last_colision = -1;
	m_currentPipe = 0;
	m_pipes.push_back(Pipe());
	//		sommets, rayon, origine, direction, normal, player
	m_pipes[m_currentPipe].init(18, 2.5, Point(0,0,0), Vector(1,0,0), Vector(0,1,0), width, height);

	m_player.init(Point(0,0,0), Vector(1,0,0), Vector(0,1,0), m_pipes[m_currentPipe].getRayon(), width, height, 45);

	float size = m_pipes[m_currentPipe].getCenterPoints().size();
	int pos;

	for(int i = 0; i < 10; i++){
		Obstacle obs;
		pos = rand() % ((int)size - 1) / 2;
		obs.init(0, m_pipes[m_currentPipe].getCenterPoints()[pos], m_pipes[m_currentPipe].getCenterPoints()[pos + 1], m_pipes[m_currentPipe].getRayons()[pos], m_pipes[m_currentPipe].getRayon());
		m_obstacles.push_back(obs);
	}

	for(int i = 0; i < 5; i++){
		Obstacle obs;
		pos = rand() % ((int)size - 1) / 2;
		obs.init(1, m_pipes[m_currentPipe].getCenterPoints()[pos], m_pipes[m_currentPipe].getCenterPoints()[pos + 1], m_pipes[m_currentPipe].getRayons()[pos], m_pipes[m_currentPipe].getRayon());
		m_obstacles.push_back(obs);
	}
}

int Game::update(){
	managePipe();


	m_pipes[m_currentPipe].update(m_player);
	

	float size = m_pipes[m_currentPipe].getCenterPoints().size();
	float m_parcourus = m_pipes[m_currentPipe].getParcourus();

	if(m_parcourus > 50 && !m_obstacle_moved){

		for(unsigned int i = 0; i < m_obstacles.size(); i++){
			int pos = rand() % 50;
			pos += m_parcourus;
			pos = pos * size / 100;

			if(pos + 1 < size){
				m_obstacles[i].moveObstacle(m_pipes[m_currentPipe].getCenterPoints()[pos], m_pipes[m_currentPipe].getCenterPoints()[pos + 1], m_pipes[m_currentPipe].getRayons()[pos]);
			}
		}

		m_obstacle_moved = true;
	}

	if(m_player.getAcc() != -1){
		m_player.boost();
	}

	int res = manageColision();
	if(res == 1)
	{
		return 0;
	}

	return 1;
}

void Game::render(){
	for(unsigned int i = 0; i < m_pipes.size(); i++){
    	m_pipes[i].render(m_skyColor, m_player);
	}
	for(unsigned int i = 0; i < m_obstacles.size(); i++){
    	m_obstacles[i].render(m_skyColor, m_player);
	}
    m_player.render(m_skyColor);
}

int Game::quit(){
	for(unsigned int i = 0; i < m_pipes.size(); i++){
		m_pipes[i].quit();
	}

	for(unsigned int i = 0; i < m_obstacles.size(); i++){
    	m_obstacles[i].quit();
	}

	return 0;
}

Pipe & Game::getPipe(){
	return m_pipes[m_currentPipe];
}

Player & Game::getPlayer(){
	return m_player;
}

void Game::setSkyColor(vec4 sky){
	m_skyColor = sky;
}

vec4 Game::getSkyColor(){
	return m_skyColor;
}

void Game::managePipe(){
	unsigned int size = m_pipes[m_currentPipe].getLength();
	float parcourus = m_pipes[m_currentPipe].getParcourus();

	if(parcourus > 50 && !m_newpipe){
		Pipe & currentPipe = m_pipes[m_currentPipe];

		Pipe pipe;
		pipe.init(currentPipe.getSommets(), currentPipe.getRayon(), currentPipe.getLastPoint(), currentPipe.getLastDirection(), currentPipe.getLastNormal(), m_width, m_height);

		m_pipes.push_back(pipe);

		m_newpipe = true;

		if(m_currentPipe > 0){
			m_pipes[0].release();
    		m_pipes.erase(m_pipes.begin());
    		m_currentPipe = 0;
		}
	}
	if(parcourus >= 99){
		m_currentPipe++;
		m_newpipe = false;
	}

	if(parcourus < 2 && m_obstacle_moved){
		float size = m_pipes[m_currentPipe].getCenterPoints().size();

		for(unsigned int i = 0; i < m_obstacles.size(); i++){
			int pos = rand() % 50;
			pos += parcourus;
			pos = pos * size / 100;

			if(pos + 1 < size){
				m_obstacles[i].moveObstacle(m_pipes[m_currentPipe].getCenterPoints()[pos], m_pipes[m_currentPipe].getCenterPoints()[pos + 1], m_pipes[m_currentPipe].getRayons()[pos]);
			}
		}

		m_obstacle_moved = false;
		m_last_colision = -1;
	}
}

int Game::manageColision()
{
	Collider playerBox = m_player.getCollisionBox();

        for(int i = 0; i < m_obstacles.size() ; i++)
        {
			if(i != m_last_colision)
			{
				Collider obstacleBox = m_obstacles[i].getCollisionBox();
				Collider playerBox = m_player.getCollisionBox();

				if ((playerBox.isInCollision(obstacleBox) && m_obstacles[i].getType() == 0) ||
					(obstacleBox.isInCollision(playerBox) && m_obstacles[i].getType() == 0))
				{
					if(m_player.decreasePlayerLife())
					{
						std::cout << "Il vous reste " << m_player.getLife() << " vie, attention" << std::endl;
						m_last_colision = i;
						return 0;
					}
					else
					{
						std::cout << "Vous êtes mort" << std::endl;
						return 1;
					}
				}
				if((playerBox.isInCollision(obstacleBox) && m_obstacles[i].getType() == 1) ||
					(obstacleBox.isInCollision(playerBox) && m_obstacles[i].getType() == 1)){
					std::cout << "Boost" << std::endl;
					m_player.setAcc(1);
					m_player.boost();
					return 0;
				}
			}
        }
	return 0;
}

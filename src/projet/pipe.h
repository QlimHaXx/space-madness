#ifndef _PIPE_H
#define _PIPE_H

#include <vector>
#include <vec.h>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include "mesh.h"
#include "mat.h"
#include "program.h"

#include "obstacle.h"
#include "player.h"

class Pipe : public Mesh {
protected:
  float m_sommets;
  Vector m_normal;
  float m_rayon;
  float m_length;
  float m_parcourus;
  bool m_obstacle_moved;
  std::vector<Point> m_control_points;
  std::vector<Point> m_smooth_points;
  std::vector<Point> m_pipe_center_points;
  std::vector<Point> m_pipe_points;
  std::vector<Vector> m_pipe_rayons;
  std::vector<Obstacle> m_obstacles;
  GLuint m_program;
  
public:
  Pipe();
  void generateControlPoints(Point origin, Vector direction);
  void generatePipe();
  void generateCircle(const Point centre, const Vector & direction, Vector axe);
  void init(float sommets, float rayon, Point p1, Vector direction, Vector lateral, int width, int height);
  void update(Player & player);
  void quit();
  std::vector<Point> getCenterPoints();
  std::vector<Vector> getRayons();
  float getRayon();
  float getSommets();
  Point& getLastPoint();
  Vector getLastDirection();
  Vector& getLastNormal();
  Point& getLastLastPoint();
  Point& getPoint(int index);
  int getSize();
  GLuint &getShader();
  Vector & getNormal();
  void render(vec4 skyColor, Player & player);
  void movePlayer(Player & player);
  float getParcourus();
  float getLength();
};

#endif
#ifndef COLLIDER_H
#define COLLIDER_H

#include <vector>
#include "vec.h"
#include "mat.h"
#include <mesh.h>

class Collider
{
    public:
        Collider();
        void init(Mesh& object, Transform& T, bool obstacle);

        bool isInCollision(Collider obstacle);
        void update(Transform T);

        std::vector<Point> changeMark (Transform T);

        Point& getMin();
        Point& getMax();
        Transform getTransform();

    private:
        int m_type;
        Point m_pMin;
        Point m_pMax;
        std::vector<Point> m_boxEdge;
        Transform m_T;

        std::vector<Point> setBoxEdge(Point min, Point max);
};

#endif
#include "collider.h"

Collider::Collider()
{
}

void Collider::init(Mesh& object, Transform& T, bool obstacle)
{
    Point pMin, pMax;

    object.bounds(pMin, pMax);

    if(obstacle)
    {
        m_pMin = Point(pMin.x - 0.5, pMin.y - 0.5, pMin.z - 0.5);
        //m_pMax = Point(pMax.x, pMax.y, pMax.z);
        m_pMax = Point(pMin.x + 0.5, pMin.y + 0.5, pMin.z + 0.5);
    }
    else
    {
        m_pMin = Point(pMin.x, pMin.y, pMin.z);
        m_pMax = Point(pMax.x, pMax.y, pMax.z);
    }

    m_T = T;
}

bool Collider::isInCollision(Collider obstacle)
{
    Point obstacleMin = obstacle.getMin();
    Point obstacleMax = obstacle.getMax();

    std::vector<Point> boxEdge = obstacle.setBoxEdge(obstacleMin,obstacleMax);

    for(unsigned int i = 0; i < boxEdge.size(); i++)
    {
        Point pm = obstacle.getTransform()(boxEdge[i]);
        Point pmd1 = Inverse(m_T)(pm);
        boxEdge[i] = pmd1;
    }

    bool bx, by, bz;
    bx = by = bz = true;

    for(unsigned int i=0; i < boxEdge.size(); i++)
    {
        Point p = boxEdge[i];
        (p.x < m_pMin.x || p.x > m_pMax.x) ? bx &= true : bx &= false;
        (p.y < m_pMin.y || p.y > m_pMax.y) ? by &= true : by &= false;
        (p.z < m_pMin.z || p.z > m_pMax.z) ? bz &= true : bz &= false;
    }

    if(bx || by || bz)
    {
        return false;
    }

    if(bx && by)
    {
        return false;
    }

    if(by && bz)
    {
        return false;
    }

    if(bx && bz)
    {
        return false;
    }

    return true;
}

void Collider::update(Transform T)
{
    m_T = T;
}

std::vector<Point> Collider::setBoxEdge(Point min, Point max)
{
    std::vector<Point> edge;

    edge.push_back(Point(min.x, min.y, min.z));
    edge.push_back(Point(min.x, min.y, max.z));
    edge.push_back(Point(max.x, min.y, max.z));
    edge.push_back(Point(max.x, min.y, min.z));
    edge.push_back(Point(min.x, max.y, min.z));
    edge.push_back(Point(min.x, max.y, max.z));
    edge.push_back(Point(max.x, max.y, max.z));
    edge.push_back(Point(max.x, max.y, min.z));

    return edge;
}

Point& Collider::getMin()
{
    return m_pMin;
}

Point& Collider::getMax()
{
    return m_pMax;
}

Transform Collider::getTransform()
{
    return m_T;
}

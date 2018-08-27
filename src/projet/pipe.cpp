#include "pipe.h"
#include "uniforms.h"

Pipe::Pipe() : Mesh(GL_TRIANGLE_STRIP) {
}

void Pipe::init(float sommets, float rayon, Point origin, Vector direction, Vector lateral, int width, int height){
    int count = 0;
    m_length = m_parcourus = 0;
    m_rayon = rayon;
    m_sommets = sommets;
    m_parcourus = 1;
    m_program = read_program("data/shaders/shader_pipe.glsl");
    program_print_errors(m_program);
    m_normal = lateral;
    
    generateControlPoints(origin, direction);
    generatePipe();

    std::vector<Point>::iterator center = m_pipe_center_points.begin();

    for(std::vector<Point>::iterator point = m_pipe_points.begin(); point != m_pipe_points.end()-2*m_sommets; ++point){
        normal(normalize(Vector(*(point), *(center))));
        vertex(*point);
        normal(normalize(Vector(*(point + 2 * m_sommets), *(center+1))));
        vertex(*(point + 2 * m_sommets));

        if(count % ((int)2 * (int)m_sommets) == 0){
            m_length += length(Vector(*(center), *(center+1)));
            center++;
        }
        
        count++;
    }
}

void Pipe::generateControlPoints(Point origin, Vector direction)
{    
    srand (static_cast <unsigned> (time(0)));
    float x,y,z;
    int distancePoint;
    bool firstPoint = true;


    distancePoint = 100;

    m_control_points.clear();
    m_control_points.push_back(origin);

    x = origin.x + distancePoint * direction.x;
    y = origin.y + distancePoint * direction.y;
    z = origin.z + distancePoint * direction.z;

    for(int i=1; i < 10; i++)
    {
        //construire les points par rapport aux points précédents si existant
        if(!firstPoint)
        {
            float theta = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/3.16));
            float phi = -3.16/3 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/((3.16/3)-(-3.16/3))));

            x = x + distancePoint * cos(theta) * cos(phi);
            y = y + distancePoint * sin(phi);
            z = z + distancePoint * sin(theta) * cos(phi);
            m_control_points.push_back(Point(x,y,z));
        }
        else {
            m_control_points.push_back(Point(x,y,z));
        }

        firstPoint = false;
    }

    m_smooth_points = m_control_points;
}

void Pipe::generatePipe(){
    std::vector<Point> tab2;
    std::vector<Point> tab3;

    // lissage de la courbe
    for(int i = 0; i < 7; i++){
        
        tab2.push_back(m_smooth_points[0]);

        for(std::vector<Point>::iterator point = m_smooth_points.begin(); point != m_smooth_points.end()-1; ++point){
            Point pt25 = *point + 0.25*(*(point+1) - *point);
            Point pt75 = *point + 0.75*(*(point+1) - *point);

            tab2.push_back(pt25);
            tab2.push_back(pt75);
        }

        tab2.push_back(m_smooth_points[m_smooth_points.size()-1]);

        m_smooth_points.swap(tab2);
        tab2.clear();
    }

    // deplacement du repère
    Point p1;
    Point p2;
    Vector a0;
    Vector v;

    // creation des cercles
    for(std::vector<Point>::iterator point = m_smooth_points.begin(); point != m_smooth_points.end()-1; ++point){
        p1 = *point;
        p2 = *(point+1);

        Vector a1 = normalize(p2 - p1);
        Vector v1 = cross(a0, a1);
        float c = dot(a0, a1);
        Transform vx;
        Transform id = Identity();

        vx.m[0][0] = vx.m[1][1] = vx.m[2][2] = 0;

        vx.m[0][1] = -v1.z;
        vx.m[0][2] = v1.y;

        vx.m[1][0] = v1.z;
        vx.m[1][2] = -v1.x;

        vx.m[2][0] = -v1.y;
        vx.m[2][1] = v1.x;

        Transform t; // = id + vx + ((vx * vx) * (1 / (1 + c)));

        Transform t1 = ((vx * vx) * (1 / (1 + c)));

        for (int i = 0; i < 4; ++i){
            for (int j = 0; j < 4; ++j){
                if(i == 3 && j == 3){
                    t.m[i][j] = 1;
                }
                else {
                    t.m[i][j] = id.m[i][j] + vx.m[i][j] + t1.m[i][j];
                }
            }
        }
        m_pipe_rayons.push_back(m_normal);
        m_normal = t(m_normal);
        a0 = a1;

        m_pipe_center_points.push_back(p1);
        // dessine le cercle
        generateCircle(p1, a0, m_normal);
    }
}

void Pipe::generateCircle(const Point centre, const Vector & direction, Vector axe){
    Transform t;
    float alpha = 2.0 * M_PI / m_sommets;
    alpha = degrees(alpha);

    for(int i = 0; i < m_sommets; i++){
        t = Rotation(direction, alpha);
        m_pipe_points.push_back(Point(centre.x + axe.x * m_rayon, centre.y + axe.y * m_rayon, centre.z + axe.z * m_rayon));
        axe = t(axe);
        m_pipe_points.push_back(Point(centre.x + axe.x * m_rayon, centre.y + axe.y * m_rayon, centre.z + axe.z * m_rayon));
    }
}

void Pipe::update(Player & player){
    movePlayer(player);
}

std::vector<Point> Pipe::getCenterPoints(){
    return m_pipe_center_points;
}

std::vector<Vector> Pipe::getRayons(){
    return m_pipe_rayons;
}

float Pipe::getRayon(){
    return m_rayon;
}

float Pipe::getSommets(){
    return m_sommets;
}

Point& Pipe::getLastPoint()
{
    return m_control_points.back();
}

Vector Pipe::getLastDirection(){
    Vector v = Vector(m_control_points[m_control_points.size()-1] - m_control_points[m_control_points.size()-2]);
    return normalize(v);
}

Vector& Pipe::getLastNormal(){
    return m_pipe_rayons.back();
}

Point& Pipe::getLastLastPoint(){
    return m_control_points[m_control_points.size() - 2];
}

Point& Pipe::getPoint(int index)
{
    return m_control_points[index];
}

GLuint & Pipe::getShader(){
    return m_program;
}

Vector & Pipe::getNormal(){
    return m_normal;
}

void Pipe::quit(){
    release_program(m_program);
    release();
}

void Pipe::render(vec4 skyColor, Player & player){
    Transform view= player.getCamera().getPosition();
    Transform projection= player.getCamera().getProjection();

    glUseProgram(m_program);
    program_uniform(m_program, "transformationMatrix", Transform());
    program_uniform(m_program, "viewMatrix", view);
    program_uniform(m_program, "projectionMatrix", projection);
    program_uniform(m_program, "skyColor", skyColor);
    draw(m_program, true, false, true, false);
}

void Pipe::movePlayer(Player & player){
    float percent = (m_parcourus * m_pipe_center_points.size()) / 100;

    if(floor(percent) + 1 < m_pipe_center_points.size()-1){
        Point un = m_pipe_center_points[floor(percent)];
        Point deux = m_pipe_center_points[floor(percent) + 1];
        Vector direction = Vector(deux - un);

        Vector translate = Vector(Point(0,0,0), m_pipe_center_points[floor(percent)+1]);
        Vector axe = cross(direction, m_pipe_rayons[floor(percent)+1]);

        float angle = degrees(acos(dot(m_pipe_rayons[floor(percent)+1], axe)));

        Transform offset = Translation(Vector(0, m_rayon, 0));

        Transform playerPos = Transform(m_pipe_rayons[floor(percent)+1], normalize(axe), normalize(direction), Vector());

        playerPos = Rotation(axe, 180) * playerPos;

        playerPos = Translation(translate) * playerPos;

        playerPos = playerPos * offset;

        player.movePlayer(playerPos);
        m_parcourus += player.getVelocity();

    }
}


float Pipe::getParcourus(){
    return m_parcourus;
}

float Pipe::getLength(){
    return m_length;
}

#include "mat.h"
#include "wavefront.h"
#include "uniforms.h"
#include "draw.h"

#include "game.h"

#include "app.h"

class Main : public App
{
protected:
    Game m_jeu;
    int width = 1024;
    int height = 640;
public:
    Main( ) : App(1024, 640), m_jeu() {}
    
    int init( )
    {
        m_jeu.setSkyColor(vec4(0, 0.05f, 0.1f, 1.f));

        m_jeu.init(width, height);

        glClearColor(m_jeu.getSkyColor().x, m_jeu.getSkyColor().y, m_jeu.getSkyColor().z, m_jeu.getSkyColor().w);        // couleur par defaut de la fenetre
        
        glClearDepth(1.f);                          // profondeur par defaut
        glDepthFunc(GL_LESS);                       // ztest, conserver l'intersection la plus proche de la camera
        glEnable(GL_DEPTH_TEST);                    // activer le ztest

        return 0;   // ras, pas d'erreur
    }
    
    // destruction des objets de l'application
    int quit( )
    {
        return m_jeu.quit(); 
    }
    // est applée a chaque delta
    int update( const float time, const float delta )
    {
        if(m_jeu.update() != 0)
        {
            return 0;
        }
        quit();
        exit(0);
        return 0;
    }

    int render( )
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // int mx, my;
        // unsigned int mb = SDL_GetRelativeMouseState(&mx, &my);
        // // deplace la camera
        // if(mb & SDL_BUTTON(1))
        //     m_camera.rotation(mx, my);      // tourne autour de l'objet
        // else if(mb & SDL_BUTTON(3))
        //     m_camera.translation((float) mx / (float) window_width(), (float) my / (float) window_height()); // deplace le point de rotation
        // else if(mb & SDL_BUTTON(2))
        //     m_camera.move(mx);           // approche / eloigne l'objet
        
        if(key_state('q')){
            m_jeu.getPlayer().turnLeft();
        }
        if(key_state('d')){
            m_jeu.getPlayer().turnRight();
        }
        if(key_state('z')){
            
        }
        if(key_state('s')){
        }

        m_jeu.render();
        
        return 1;
    }
};


int main( int argc, char **argv )
{
    srand(time(NULL));
    Main tp;
    tp.run();
    
    return 0;
}

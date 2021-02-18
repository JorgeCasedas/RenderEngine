
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                             *
 *  Started by Ángel on may of 2014                                            *
 *                                                                             *
 *  This is free software released into the public domain.                     *
 *                                                                             *
 *  angel.rodriguez@esne.edu                                                   *
 *                                                                             *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**********************************************************************
*Project           : Enviroment Render
*
*Editor : Jorge C�sedas
*
*Starting date : 21/05/2020
*
*Ending date : 24/05/2020
*
*Purpose : Rendering an enviroment with Opengl
*
**********************************************************************/

#include <cassert>
#include <GL/glew.h>            // Debe incluirse antes que gl.h
#include "MyView.hpp"
#include "ElevationMesh.hpp"
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>



using namespace sf;
using namespace example;

int main ()
{
    // Se crea la ventana de SFML, que es donde se creará el contexto de OpenGL:

    Window window(VideoMode(1280, 720), "OpenGL Task: EnviromentRender", Style::Default, ContextSettings(32));

    // Una vez se ha creado el contexto de OpenGL ya se puede inicializar Glew:

    GLenum glew_initialization =  glewInit ();

    assert(glew_initialization == GLEW_OK);

    // Glew se inicializa antes de crear view porque view ya usa extensiones de OpenGL:

    MyView view(1280, 720);

    window.setVerticalSyncEnabled (true);

    bool running = true;


    do
    {
        Event event;

        while (window.pollEvent (event))
        {
            switch (event.type)
            {
                case Event::Closed:
                {
                    running = false;
                    break;
                }

                case Event::Resized:
                {
                    Vector2u window_size = window.getSize ();

                    view.resize (window_size.x, window_size.y);

                    break;
                }
            }
        }

       // glClear (GL_COLOR_BUFFER_BIT);
        glClearColor(100.f/225.f, 150.f / 225.f, 150.f / 225.f, 1.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view.update ();
        
        //view.frameBuffer.BeforeDrawingScene(); //COMENTED BECAUSE IT DOESNT WORK
        view.render();
        view.renderSkybox();
        //view.frameBuffer.AfterDrawingScene(view.framebuffer_program_id); //COMENTED BECAUSE IT DOESNT WORK
        window.display();
    }
    while (running);

    return (EXIT_SUCCESS);
}

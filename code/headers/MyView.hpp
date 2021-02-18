
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
*Editor : Jorge Cásedas
*
*Starting date : 21/05/2020
*
*Ending date : 24/05/2020
*
*Purpose : Rendering an enviroment with Opengl
*
**********************************************************************/

#ifndef VIEW_HEADER
#define VIEW_HEADER

    #include <GL/glew.h>            // Debe incluirse antes que gl.h
    #include "Model.hpp"
    #include "Camera.hpp"
    #include "Skybox.hpp"
    #include"FrameBuffer.hpp"
    #include <string>
    #include"ElevationMesh.hpp"

    namespace example
    {

        class MyView
        {
        private:

            static const std::string   vertex_shader_code;
            static const std::string fragment_shader_code;

            GLint  model_view_matrix_id;
            GLint  projection_matrix_id;
            GLint normal_matrix_id;
            GLint model_matrix_id;
            GLint view_pos_vec_id;
            GLint hasTextue_id;

            Skybox  skybox;
            Camera  camera;
            Model   transparentInstagramLogoCube;
            Model   bambi;
            Model   car;
            ElevationMesh terrain;

            float angleX;
            float  angleY;

            float testClock;

            GLuint skybox_program_id;
            GLuint objects_program_id;

        public:

            int width;
            int height;

            FrameBuffer frameBuffer;
            GLuint framebuffer_program_id;
        public:

            MyView(int width, int height);

            void   update ();
            void   sceneUpdate();
            void   render ();
            void   renderSkybox ();
            void   resize (int width, int height);

        private:

            GLuint compile_shaders        ();
            GLuint compile_shaders        (string vertexShader, string fragmentShader);
            void   show_compilation_error (GLuint  shader_id);
            void   show_linkage_error     (GLuint program_id);

        };

    }

#endif

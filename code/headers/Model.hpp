
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

#ifndef Model_HEADER
#define Model_HEADER

    #include <SFML/OpenGL.hpp>
    #include <SFML/Graphics.hpp>
    #include "tiny_obj_loader.h"
    #include <glm/glm.hpp>                          // vec3, vec4, ivec4, mat4
    #include <glm/gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective
    #include <glm/gtc/type_ptr.hpp>                 // value_ptr
    #include <vector>
    #include<iostream>
    
using namespace std;
using namespace sf;

    namespace example
    {

        class Model
        {
        private:

            // Índices para indexar el array vbo_ids:

            enum
            {
                COORDINATES_VBO,
                TEXTURE_VBO,
                COLORS_VBO,
                NORMALS_VBO,
                INDICES_IBO,
                VBO_COUNT
            };

            // Arrays de datos del cubo base:

           /* static const GLfloat coordinates[];
            static const GLfloat colors     [];
            static const GLubyte indices    [];*/

            string inputFile;

            vector<GLfloat> myVertices;            
            vector<GLfloat> myColors;
            vector<GLfloat> myNormals;
            vector<GLfloat> myTextures;

            vector<GLuint> myTriangles; //index
            
            

            GLuint vbo_ids[VBO_COUNT];      // Ids de los VBOs que se usan
            GLuint vao_id;                  // Id del VAO del cubo
            GLuint texture_id;              // TextureId

            sf::Image img_data;

            vector<Model*> children;

        public:

            glm::vec3 objectPosition;
            glm::vec3 objectRotation;
            float rotAngle;
            glm::vec3 objectScale;

            glm::vec3 movementVec;

            Model* parent;

        public:

            Model(string obj, string tex, glm::vec3 objPos, glm::vec3 objScale);
           ~Model();

            void GetModelData(string modelFile);
            float lerpNumber(float a, float b, float f);
            
            void render ();
            void render(glm::mat4* model_view_matrix, GLint model_view_matrix_id, GLint hasTexture_id);
            void Update();

            void Move(glm::vec3 movement);
            void Rotate(float angle, glm::vec3 rotVec);

            void Parent(Model* _parent);
            void AddChild(Model* child);
            void RemoveChild(Model* _child);
        };

    }

#endif

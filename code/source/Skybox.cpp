
/**********************************************************************
*Project           : Enviroment Render
*
*Author : Jorge Cásedas
*
*Starting date : 21/05/2020
*
*Ending date : 24/05/2020
*
*Purpose : Rendering an enviroment with Opengl
*
**********************************************************************/

#include "Skybox.hpp"
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

GLfloat skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

 const string Skybox::sky_vertex_shader_code =

    "#version 330\n"
    ""
    "layout(location = 0) in vec3 aPos;"

    "out vec3 TexCoords;"

    "uniform mat4 projection;"
    "uniform mat4 view;"

    "void main()" 
    "{"
    "   TexCoords = aPos;"  
    "   vec4 pos = projection * view * vec4(aPos, 1.0);"
    "   gl_Position = pos.xyww;"
    "}";
       

 const string Skybox::sky_fragment_shader_code =

    "#version 330\n"
    ""
    "in vec3 TexCoords;"
    ""
    "out vec4 FragColor;"
    ""
    "uniform samplerCube skybox;"
    ""
    "void main()"
    "{"
    "   FragColor = texture(skybox, TexCoords);"
    "}";

   

Skybox::Skybox()
{
    faces = 
    {
        /*"back.tga",
        "left.tga",
        "front.tga",
        "right.tga",
        "top.tga",
        "bottom.tga"*/
       
        "Assets/back.png",
        "Assets/left.png",
        "Assets/front.png",
        "Assets/right.png",
        "Assets/top.png",
        "Assets/bottom.png"

    };


    glGenBuffers(VBO_COUNT, vbo_ids);
    glGenVertexArrays(1, &vao_id);

    // Se activa el VAO del cubo para configurarlo:

    glBindVertexArray(vao_id);

    // Se suben a un VBO los datos de coordenadas y se vinculan al VAO:

    glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    LoadSkybox();

}

void Skybox::LoadSkybox()
{
    glEnable(GL_TEXTURE_CUBE_MAP);
    glGenTextures(1, &textureID);
    glActiveTexture (GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
   
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    static const GLenum texture_target[] =
    {
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    };

    for (size_t texture_index = 0; texture_index < 6; texture_index++)
    {
        sf::Image img_data;
        img_data.loadFromFile(faces[texture_index]);

        glTexImage2D
        (
            texture_target[texture_index],
            0,
            GL_RGBA,
            img_data.getSize().x,
            img_data.getSize().y,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            img_data.getPixelsPtr()
            );

    }
}
void Skybox::render(int width, int height)
{
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);
    glm::mat4 model_view_matrix;
    model_view_matrix = glm::rotate(model_view_matrix, angleX, cameraXRotation);
    model_view_matrix = glm::rotate(model_view_matrix, angleY, cameraYRotation);
    glm::mat4 projection_matrix = glm::perspective(20.f, GLfloat(width) / height, 1.f, 1000.f);

    glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(model_view_matrix));
    glUniformMatrix4fv(projection_matrix_id, 1, GL_FALSE, glm::value_ptr(projection_matrix));

    glBindVertexArray(vao_id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
}


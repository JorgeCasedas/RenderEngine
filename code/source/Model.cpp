
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

#include <GL/glew.h>            // Debe incluirse antes que gl.h
#include "Model.hpp"
#include <SFML/OpenGL.hpp>
#include <vector>

namespace example
{

    Model::Model(string obj, string tex, glm::vec3 objPos, glm::vec3 objScale)
    {
        objectPosition = objPos;// glm::vec3(0.f, 0.5f, 0.f);
        objectRotation = glm::vec3(0, 1, 0);
        rotAngle = 0;
        objectScale = objScale;// glm::vec3(1.01f, 1.01f, 1.01f);
        movementVec = glm::vec3(0.f, 0.f, 0.f);
        texture_id = 0;
        parent = nullptr;

        GetModelData(obj);

        // Se generan índices para los VBOs del cubo:

        glGenBuffers (VBO_COUNT, vbo_ids);
        glGenVertexArrays (1, &vao_id);

        // Se activa el VAO del cubo para configurarlo:

        glBindVertexArray (vao_id);
        
        // Se suben a un VBO los datos de coordenadas y se vinculan al VAO:

        glBindBuffer (GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(coordinates), coordinates, GL_STATIC_DRAW);
        glBufferData (GL_ARRAY_BUFFER, myVertices.size() * sizeof(GLfloat), myVertices.data(), GL_STATIC_DRAW); //&vertices.front() - &vertices[0]

        glEnableVertexAttribArray (0);
        glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Se suben a un VBO los datos de textura y se vinculan al VAO:

        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[TEXTURE_VBO]);
        glBufferData(GL_ARRAY_BUFFER, myTextures.size() * sizeof(GLfloat), myTextures.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

        // Se suben a un VBO los datos de color y se vinculan al VAO:

        glBindBuffer (GL_ARRAY_BUFFER, vbo_ids[COLORS_VBO]);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
        glBufferData(GL_ARRAY_BUFFER, myColors.size() * sizeof(GLfloat), myColors.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray (2); 
        glVertexAttribPointer (2, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Se suben a un VBO los datos de normales y se vinculan al VAO:

        glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[NORMALS_VBO]);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
        glBufferData(GL_ARRAY_BUFFER, myNormals.size() * sizeof(GLfloat), myNormals.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
       
        // Se suben a un IBO los datos de índices:

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_IBO]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, myTriangles.size() * sizeof(GLuint), myTriangles.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);


        //TEXTURE CREATION
        
        if (tex != "NoTexture" || img_data.getPixelsPtr()!=nullptr) 
        {  
            img_data.loadFromFile(tex);
        }
        
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexImage2D
        (
            GL_TEXTURE_2D, 
            0, 
            GL_RGBA,
            img_data.getSize().x, 
            img_data.getSize().y,
            0,
            GL_RGBA, 
            GL_UNSIGNED_BYTE, 
            img_data.getPixelsPtr()
        );

        cout << "BreakPoint" << endl;
    }

    void Model::GetModelData(string modelFile)
    {

        inputFile = modelFile;
        tinyobj::attrib_t attrib;
        vector<tinyobj::shape_t> shapes;
        vector<tinyobj::material_t> materials;

        string warn;
        string err;

        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputFile.c_str());

        if (!warn.empty()) {
            cout << warn << endl;
        }

        if (!err.empty()) {
            cerr << err << endl;
        }

        if (!ret) {
            exit(1);
        }

        for (int i = 0; shapes[0].mesh.indices.size() > i; i++)
        {
            // VERTEX
            myVertices.push_back((attrib.vertices[3 * shapes[0].mesh.indices[i].vertex_index + 0]) * objectScale.x + objectPosition.x);
            myVertices.push_back((attrib.vertices[3 * shapes[0].mesh.indices[i].vertex_index + 1]) * objectScale.y + objectPosition.y);
            myVertices.push_back((attrib.vertices[3 * shapes[0].mesh.indices[i].vertex_index + 2]) * objectScale.z + objectPosition.z);

            if (attrib.texcoords.size() > 0) 
            {
                myTextures.push_back(attrib.texcoords[2 * shapes[0].mesh.indices[i].texcoord_index + 0]);
                myTextures.push_back(1.0f - attrib.texcoords[2 * shapes[0].mesh.indices[i].texcoord_index + 1]);
            }
            else //This is done for all the models with no texture coordinates 
            {
                myTextures.push_back(0);
                myTextures.push_back(0);
            }
            
            // NORMALS
            myNormals.push_back(attrib.normals[3 * shapes[0].mesh.indices[i].normal_index + 0]);
            myNormals.push_back(attrib.normals[3 * shapes[0].mesh.indices[i].normal_index + 1]);
            myNormals.push_back(attrib.normals[3 * shapes[0].mesh.indices[i].normal_index + 2]);

            // VERTEX COLOR
            tinyobj::real_t red = attrib.colors[3 * shapes[0].mesh.indices[i].vertex_index + 0];
            tinyobj::real_t green = attrib.colors[3 * shapes[0].mesh.indices[i].vertex_index + 1];
            tinyobj::real_t blue = attrib.colors[3 * shapes[0].mesh.indices[i].vertex_index + 2];
            red = lerpNumber(0, 255, red);
            green = lerpNumber(0, 255, green);
            blue = lerpNumber(0, 255, blue);
            myColors.push_back((float)red / 255.f);
            myColors.push_back((float)green / 255.f);
            myColors.push_back((float)blue / 255.f);

            // VERTEX INDEX
            myTriangles.push_back((GLuint)(i));
        }

        cout << "BreakPoint" << endl;
    }

    float Model::lerpNumber(float a, float b, float f)
    {
        return a + f * (b - a);
    }

    Model::~Model()
    {
        // Se liberan los VBOs y el VAO usados:

        glDeleteVertexArrays (1, &vao_id);
        glDeleteBuffers      (VBO_COUNT, vbo_ids);
    }


    void Model::render()
    {
        //glEnable(GL_TEXTURE_2D);
        if (texture_id!=0)
        {
            glBindTexture(GL_TEXTURE_2D, texture_id);
        }

        // Se selecciona el VAO que contiene los datos del objeto y se dibujan sus elementos:

        glBindVertexArray(vao_id);
        glDrawElements(GL_TRIANGLES, myTriangles.size() * sizeof(GLuint), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void Model::render(glm::mat4* model_view_matrix, GLint model_view_matrix_id, GLint hasTextue_id)
    {
        //Change object pos - Approach 1 - This approach is useful for runtime
        glm::mat4 tempMat = *model_view_matrix;
        tempMat = glm::translate(tempMat, objectPosition);
        if(rotAngle!=0)
            tempMat = glm::rotate(tempMat, rotAngle, objectRotation);

        glUniformMatrix4fv(model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(tempMat));
        if(img_data.getPixelsPtr()!=nullptr)
            glUniform1i(hasTextue_id, 1);   //Has texture
        else
            glUniform1i(hasTextue_id, 0);   //Doesnt have texture

        render();
    }
    void Model::Update()
    {
        //if (movementVec.x == 0)
        //    movementVec.x = 1;
        ////Simple Forward-BackWards movement
        //if (objectPosition.x > 10)
        //{
        //    movementVec.x = -1;
        //}
        //else if (objectPosition.x < 0)
        //{
        //    movementVec.x = 1;
        //}
        //
        //objectPosition.x += 0.01f * movementVec.x;
    }
    void Model::Move(glm::vec3 movement)
    {
        objectPosition += movement;
        if (children.size() > 0) 
        {
            for (Model* child : children)
            {
                if(child!=nullptr)
                    child->Move(movement);
            }
        }
    }
    void Model::Rotate(float angle, glm::vec3 rotVec)
    {
        rotAngle = angle;
        objectRotation = rotVec;
    }
    void Model::Parent(Model* _parent)
    {
        if (_parent == nullptr && parent!=nullptr)
        {
            parent->RemoveChild(this);
            parent = nullptr;
        }
        else if (_parent != nullptr && parent == nullptr)
        {
            _parent->AddChild(this);
            parent = _parent;
        }
        else if (_parent != nullptr && parent != nullptr) 
        {
            parent->RemoveChild(this);
            parent = nullptr;
            _parent->AddChild(this);
            parent = _parent;
        }
    }
    void Model::AddChild(Model* child)
    {
        children.push_back(child);
    }
    void Model::RemoveChild(Model* _child)
    {
        for (int i = 0; i < children.size(); i++)
        {
            if (children[i] == _child) 
            {
                children[i] = nullptr;
                break;
            }
        }
        
        children.erase(std::remove(children.begin(), children.end(), nullptr), children.end());
    }

}

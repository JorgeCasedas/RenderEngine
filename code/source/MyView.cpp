
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                             *
 *  Started by √Ångel on may of 2014                                            *
 *                                                                             *
 *  This is free software released into the public domain.                     *
 *                                                                             *
 *  angel.rodriguez@esne.edu                                                   *
 *                                                                             *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**********************************************************************
*Project           : Enviroment Render
*
*Editor : Jorge C·sedas
*
*Starting date : 21/05/2020
*
*Ending date : 24/05/2020
*
*Purpose : Rendering an enviroment with Opengl
*
**********************************************************************/

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc

#include "MyView.hpp"
#include <iostream>
#include <cassert>

#include <glm/glm.hpp>                          // vec3, vec4, ivec4, mat4
#include <glm/gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective
#include <glm/gtc/type_ptr.hpp>                 // value_ptr

#include <SFML/Window/Keyboard.hpp>

namespace example
{

    using namespace std;

   /* const string MyView::vertex_shader_code =

        "#version 330\n"
        ""
        "uniform mat4 model_view_matrix;"
        "uniform mat4 projection_matrix;"
        "uniform mat4     normal_matrix;"
        ""
        "layout (location = 0) in vec3 vertex_coordinates;"
        "layout (location = 1) in vec3 vertex_color;"
        "layout (location = 2) in vec3 normals;"
        ""
        "out vec3 front_color;"
        "out vec4 normal;"
        ""
        "void main()"
        "{"
        "   normal   = normal_matrix * vec4(normals, 1.0);"
        "   gl_Position = projection_matrix * model_view_matrix * vec4(vertex_coordinates, 1.0);"
        "   front_color = vertex_color;"
        "}";*/

  //Vertex Shader 2∫ try
    const string MyView::vertex_shader_code =

        "#version 330\n"
        ""
        "uniform mat4 model;"
        "uniform mat4 model_view_matrix;"
        "uniform mat4 projection_matrix;"
        "uniform mat4 normal_matrix;"
        ""
        "layout (location = 0) in vec3 vertex_coordinates;"
        "layout (location = 1) in vec2 texture_Coordinates;"
        "layout (location = 2) in vec3 vertex_color;"
        "layout (location = 3) in vec3 normals;"
        
        ""
        "out vec3 front_color;"
        "out vec3 FragPos;"
        "out vec3 Normal;"
        "out vec2 texCoord;"
        ""
        "void main()"
        "{"
        "   FragPos = vec3(model * vec4(vertex_coordinates, 1.0));"
        "   Normal   = mat3(transpose(inverse(model))) * normals;"
        "   gl_Position = projection_matrix * model_view_matrix * vec4(vertex_coordinates, 1.0);"
        "   front_color = vertex_color;"
        "   texCoord = texture_Coordinates;"
        "}";

    /*const string MyView::fragment_shader_code =

        "#version 330\n"
        ""
        "in  vec3    front_color;"
        "in  vec4    normal;"
        "out vec4 fragment_color;"
        ""
        "void main()"
        "{"
        "   vec4 eyeNormal = normalize(normal);"
        "   vec4 lightDir = vec4(1.0, 1.0, 0.0, 0.0);"
        "   float nDotVP = max(0.0, dot(eyeNormal, normalize(lightDir)));"
        "   fragment_color = vec4(front_color, 1.0) * nDotVP * 5;"
        "}";*/

//Fragment Shader 2∫ try
    const string MyView::fragment_shader_code =

        "#version 330\n"
        "uniform vec3 viewPos;"
        "uniform sampler2D myTexture;"
        "uniform bool hasTexture;"
        ""
        "in  vec3    front_color;"
        "in  vec3    FragPos;"
        "in  vec3    Normal;"
        "in  vec2    texCoord;"
        ""
        "out vec4    fragment_color;"
        ""
        "void main()"
        "{"
        //ambient
        "float ambientStrength = 0.3;"
        "vec3 lightColor = vec3(0.976, 0.964, 0.921);"
        "vec3 ambient = ambientStrength * lightColor;"

        //diffuse
        "vec3 norm = normalize(Normal);"
        "vec3 lightPos = vec3 (-50, 500.0, 100);"
        "vec3 lightDir = normalize(lightPos-FragPos);"
        "float diff = max(dot(norm, lightDir), 0.0);"
        "vec3 diffuse = diff * lightColor;"

        //Specular
        "float specularStrength = 0.2;"
        "vec3 viewDir = normalize(viewPos - FragPos);"
        "vec3 reflectDir = reflect(-lightDir, norm);"
        "float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);"
        "vec3 specular = specularStrength * spec * lightColor; "
        
        //Result
        "vec3 result = (ambient + diffuse + specular) * front_color;"

        //Textures
        "vec4 texColor = texture(myTexture, texCoord);"
        "if (texColor.a < 0.1){discard;}"
        "if(hasTexture){fragment_color = texColor * vec4(result, 1.0);}"
        "else{fragment_color = vec4(result, 1.0);}"
        "}";

    MyView::MyView(int width, int height)
        :
        testClock(0),
        angleX(10),
        angleY(50),
        height(height),
        width(width),
        transparentInstagramLogoCube(Model("Assets/Box.obj", "Assets/AlphaTest2.png", glm::vec3(-20.f, 20.f, 45.f), glm::vec3(1.f, 1.f, 1.f))),
        bambi(Model("Assets/bigDeerColor.obj", "NoTexture", glm::vec3(30.f, 1.2f, 4.f), glm::vec3(0.003f, 0.003f, 0.003f))),
        car(Model("Assets/Car.obj", "Assets/CarTexture.png", glm::vec3(-10.f, 1.25f,510.f), glm::vec3(1.f, 1.f, 1.f))),
        skybox(Skybox())
    {
        //transparentInstagramLogoCube.Parent(&terrain);

        car.Rotate(170, glm::vec3(0, 1, 0));

        
        // Se establece la configuracion basica:

       /* glEnable     (GL_CULL_FACE);
        glClearColor (0.f, 0.f, 0.f, 1.f);*/

        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LEQUAL);
        glDepthRange(0.0f, 1.0f);

        // Se compilan y se activan los shaders:
        objects_program_id = compile_shaders(vertex_shader_code, fragment_shader_code);
        skybox_program_id = compile_shaders(skybox.sky_vertex_shader_code, skybox.sky_fragment_shader_code);
        framebuffer_program_id = compile_shaders(frameBuffer.quad_vertex_shader_code, frameBuffer.quad_fragment_shader_code);

        glUseProgram(objects_program_id);

        skybox.model_view_matrix_id = glGetUniformLocation(skybox_program_id, "view");
        skybox.projection_matrix_id = glGetUniformLocation(skybox_program_id, "projection");

        model_view_matrix_id = glGetUniformLocation(objects_program_id, "model_view_matrix");
        projection_matrix_id = glGetUniformLocation(objects_program_id, "projection_matrix");
        normal_matrix_id = glGetUniformLocation(objects_program_id, "normal_matrix");
        model_matrix_id = glGetUniformLocation(objects_program_id, "model");
        view_pos_vec_id = glGetUniformLocation(objects_program_id, "viewPos");
        hasTextue_id = glGetUniformLocation(objects_program_id, "hasTexture");

        resize (width, height);
        
    }

    void MyView::update ()
    {

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            camera.cameraDisplacement.x += 0.5f;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            camera.cameraDisplacement.x -= 0.5f;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            camera.cameraDisplacement.z += 0.5f;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            camera.cameraDisplacement.z -= 0.5f;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            camera.cameraDisplacement.y -= 0.5f;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        {
            camera.cameraDisplacement.y += 0.5f;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            angleX += 0.5f;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) 
        {
            angleX -= 0.5f;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            angleY += 0.5f;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            angleY -= 0.5f;
        }
        camera.cameraXRotation = glm::vec3(1.f, 0.f, 0.f);
        camera.cameraYRotation = glm::vec3(0.f, 1.f, 0.f);
        skybox.angleX = angleX;
        skybox.angleY = angleY;
        skybox.cameraXRotation = camera.cameraXRotation;
        skybox.cameraYRotation = camera.cameraYRotation;
        camera.Update();

        // AUTOMATIC ANIMATION - CAR CRASHES INTO DEER AND PUSH IT TILL IT STOPS
        sceneUpdate();
    }

    void MyView::sceneUpdate()
    {
        testClock += 0.01;
        if (testClock <= 1.13)
        {
            car.Move(glm::vec3(-0.3, 0, -0.008));
        }
        else if (bambi.parent != nullptr)
        {
            bambi.Parent(nullptr);
        }
        //bambi.Move(glm::vec3(0.03f, 0, 0));
        if (testClock >= 0.789 && testClock < 0.8 && bambi.parent == nullptr)
        {
            bambi.Parent(&car);
        }
    }

    void MyView::renderSkybox() 
    {
        //SKYBOX
        glUseProgram(skybox_program_id);
        skybox.render(width,height);
        glUseProgram(0);
    }
    void MyView::render ()
    {
        glUseProgram(objects_program_id);
        glClear (GL_COLOR_BUFFER_BIT);

        // Se rota el cubo y se empuja hacia el fondo:

        glm::mat4 model_view_matrix;

        model_view_matrix = glm::translate (model_view_matrix, camera.cameraDisplacement);
        model_view_matrix = glm::rotate(model_view_matrix, angleX, camera.cameraXRotation);
        model_view_matrix = glm::rotate(model_view_matrix, angleY, camera.cameraYRotation);

        glUniformMatrix4fv (model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(model_view_matrix));

        glm::mat4 normal_matrix = glm::transpose(glm::inverse(model_view_matrix));

        glUniformMatrix4fv(normal_matrix_id, 1, GL_FALSE, glm::value_ptr(normal_matrix));

        glm::mat4 model = glm::mat4(1.0f);
        glUniformMatrix4fv(model_matrix_id, 1, GL_FALSE, glm::value_ptr(model));

        glm::vec3 viewPos = camera.cameraDisplacement;
        glUniformMatrix4fv(view_pos_vec_id, 1, GL_FALSE, glm::value_ptr(viewPos));
        
        // Se dibuja el cubo:

        terrain.render(hasTextue_id);
        transparentInstagramLogoCube.render (&model_view_matrix, model_view_matrix_id, hasTextue_id);
        bambi.render(&model_view_matrix, model_view_matrix_id, hasTextue_id);
        car.render(&model_view_matrix, model_view_matrix_id, hasTextue_id);
    }

    void MyView::resize (int _width, int _height)
    {
        width = _width;
        height = _height;
        glm::mat4 projection_matrix = glm::perspective (20.f, GLfloat(_width) / _height, 1.f, 1000.f);

        glUniformMatrix4fv (projection_matrix_id, 1, GL_FALSE, glm::value_ptr(projection_matrix));

        glViewport (0, 0, _width, _height);
    }
    GLuint MyView::compile_shaders ()
    {
        GLint succeeded = GL_FALSE;

        // Se crean objetos para los shaders:

        GLuint   vertex_shader_id = glCreateShader (GL_VERTEX_SHADER  );
        GLuint fragment_shader_id = glCreateShader (GL_FRAGMENT_SHADER);

        // Se carga el codigo de los shaders:

        const char *   vertex_shaders_code[] = {   vertex_shader_code.c_str () };
        const char * fragment_shaders_code[] = { fragment_shader_code.c_str () };
        const GLint    vertex_shaders_size[] = {   vertex_shader_code.size  () };
        const GLint  fragment_shaders_size[] = { fragment_shader_code.size  () };

        glShaderSource  (  vertex_shader_id, 1,   vertex_shaders_code,   vertex_shaders_size);
        glShaderSource  (fragment_shader_id, 1, fragment_shaders_code, fragment_shaders_size);

        // Se compilan los shaders:

        glCompileShader (  vertex_shader_id);
        glCompileShader (fragment_shader_id);

        // Se comprueba que si la compilacion ha tenido exito:

        glGetShaderiv   (  vertex_shader_id, GL_COMPILE_STATUS, &succeeded);
        if (!succeeded) show_compilation_error (  vertex_shader_id);

        glGetShaderiv   (fragment_shader_id, GL_COMPILE_STATUS, &succeeded);
        if (!succeeded) show_compilation_error (fragment_shader_id);

        // Se crea un objeto para un programa:

        GLuint program_id = glCreateProgram ();

        // Se cargan los shaders compilados en el programa:

        glAttachShader  (program_id,   vertex_shader_id);
        glAttachShader  (program_id, fragment_shader_id);

        // Se linkan los shaders:

        glLinkProgram   (program_id);

        // Se comprueba si el linkage ha tenido exito:

        glGetProgramiv  (program_id, GL_LINK_STATUS, &succeeded);
        if (!succeeded) show_linkage_error (program_id);

        // Se liberan los shaders compilados una vez se han linkado:

        glDeleteShader (  vertex_shader_id);
        glDeleteShader (fragment_shader_id);

        return (program_id);
    }
    GLuint MyView::compile_shaders(string vertexShader, string fragmentShader)
    {
        GLint succeeded = GL_FALSE;

        // Se crean objetos para los shaders:

        GLuint   vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

        // Se carga el codigo de los shaders:

        const char* vertex_shaders_code[] = { vertexShader.c_str() };
        const char* fragment_shaders_code[] = { fragmentShader.c_str() };
        const GLint    vertex_shaders_size[] = { vertexShader.size() };
        const GLint  fragment_shaders_size[] = { fragmentShader.size() };

        glShaderSource(vertex_shader_id, 1, vertex_shaders_code, vertex_shaders_size);
        glShaderSource(fragment_shader_id, 1, fragment_shaders_code, fragment_shaders_size);

        // Se compilan los shaders:

        glCompileShader(vertex_shader_id);
        glCompileShader(fragment_shader_id);

        // Se comprueba que si la compilacion ha tenido exito:

        glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &succeeded);
        if (!succeeded) show_compilation_error(vertex_shader_id);

        glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &succeeded);
        if (!succeeded) show_compilation_error(fragment_shader_id);

        // Se crea un objeto para un programa:

        GLuint program_id = glCreateProgram();

        // Se cargan los shaders compilados en el programa:

        glAttachShader(program_id, vertex_shader_id);
        glAttachShader(program_id, fragment_shader_id);

        // Se linkan los shaders:

        glLinkProgram(program_id);

        // Se comprueba si el linkage ha tenido exito:

        glGetProgramiv(program_id, GL_LINK_STATUS, &succeeded);
        if (!succeeded) show_linkage_error(program_id);

        // Se liberan los shaders compilados una vez se han linkado:

        glDeleteShader(vertex_shader_id);
        glDeleteShader(fragment_shader_id);

        return (program_id);
    }
    void MyView::show_compilation_error (GLuint shader_id)
    {
        string info_log;
        GLint  info_log_length;

        glGetShaderiv (shader_id, GL_INFO_LOG_LENGTH, &info_log_length);

        info_log.resize (info_log_length);

        glGetShaderInfoLog (shader_id, info_log_length, NULL, &info_log.front ());

        cerr << info_log.c_str () << endl;

        #ifdef _MSC_VER
            OutputDebugStringA (info_log.c_str ());
        #endif

        assert(false);
    }
    void MyView::show_linkage_error (GLuint program_id)
    {
        string info_log;
        GLint  info_log_length;

        glGetProgramiv (program_id, GL_INFO_LOG_LENGTH, &info_log_length);

        info_log.resize (info_log_length);

        glGetProgramInfoLog (program_id, info_log_length, NULL, &info_log.front ());

        cerr << info_log.c_str () << endl;

        #ifdef _MSC_VER
            OutputDebugStringA (info_log.c_str ());
        #endif

        assert(false);
    }

}

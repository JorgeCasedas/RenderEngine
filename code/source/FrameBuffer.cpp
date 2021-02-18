
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

#include "FrameBuffer.hpp"

float quadVertices[] = {
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
};

const string FrameBuffer::quad_vertex_shader_code =

"#version 330\n"
""
"layout(location = 0) in vec2 aPos;"
"layout(location = 1) in vec2 aTexCoords;"

"out vec2 TexCoords;"

"void main()"
"{"
"	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);"
"	TexCoords = aTexCoords;"
"}";


const string FrameBuffer::quad_fragment_shader_code =

"#version 330\n"
""
"out vec4 FragColor;"

"in vec2 TexCoords;"

"uniform sampler2D screenTexture;"

"void main()"
"{"
"	FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);" //Show negative colors
"}";


FrameBuffer::FrameBuffer()
{
	//COMENTED BECAUSE IT DOESNT WORK

	/*CreateFrameBuffer();
	CreateTexture();
	CreateRenderObject();
	CreateQuad();
	ConfigurateFrameBuffer();*/
}

void FrameBuffer::CreateFrameBuffer()
{
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	//while (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) { /*FRAMEBUFFER IS NOT COMPLETED*/ }
}

void FrameBuffer::CreateTexture()
{
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1280, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glBindTexture(GL_TEXTURE_2D, 0);

	// attach it to currently bound framebuffer object
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);
}

void FrameBuffer::CreateRenderObject()
{
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1280, 720);
	//glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
}

void FrameBuffer::CreateQuad()
{

	glGenBuffers(VBO_COUNT, vbo_ids);
	glGenVertexArrays(1, &vao_id);

	// Se activa el VAO del cubo para configurarlo:

	glBindVertexArray(vao_id);

	// Se suben a un VBO los datos de coordenadas y se vinculan al VAO:

	glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

}

void FrameBuffer::ConfigurateFrameBuffer()
{

	// Se configura el framebuffer:
	{
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texColorBuffer, 0);

		const GLenum draw_buffer = GL_COLOR_ATTACHMENT0;

		glDrawBuffers(1, &draw_buffer);
	}

	// Se comprueba que el framebuffer est√° listo:

	assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

	// Se desvincula el framebuffer:

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void FrameBuffer::BeforeDrawingScene()
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
	//glEnable(GL_DEPTH_TEST);
}


void FrameBuffer::AfterDrawingScene(GLuint program_id)
{

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(program_id);

	glBindVertexArray(vao_id);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glDrawArrays(GL_LINE_STRIP, 0, 6);

}

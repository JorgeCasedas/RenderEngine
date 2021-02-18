
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

#include <GL/glew.h> 
#include "ElevationMesh.hpp"
#include <SFML/OpenGL.hpp>


ElevationMesh::ElevationMesh() 
{
	//heightMapImage.loadFromFile("Heightmap.png");

	objectPosition = glm::vec3(0.f, 0.f, 0.f);
	objectScale = glm::vec3(2.f, 2.f, 2.f);
	
	genTerrain(150, 150);
}

ElevationMesh::ElevationMesh(int widthResolution, int lengthResolution) 
{
	genTerrain(widthResolution, lengthResolution);
}



void ElevationMesh::genTerrain(int wRes , int lRes )
{
	const int size = wRes * lRes;

	vector<int[3]> tris;
	
	const float w = 100.f, h=20.f, l = 100.f;
	heightMap.loadFromFile("Assets/Heightmap.png");
	//normalMap.loadFromFile("Height1NormalMap.png");
	//get Vertices
	int i = 0;
	for (int z = 0; z < lRes; z++) 
	{
		for (int x = 0; x < wRes; x++) 
		{
			
			float v[3] { x/(float)wRes, 1, z / (float)lRes }; //Normalizar las coordenadas
					

			// We get the y coordinate for each vertex from the heightMap
			float heightValue = 0;
			int ix = 0, iy = 0;
			//Get the relative coordinates from the heighMap
			ix = (int)roundf(x * (heightMap.getSize().x / wRes));
			iy = (int)roundf(z * (heightMap.getSize().y / lRes));
			//Get the height
			heightValue = getPixel(heightMap, ix, iy).r / 255.0f;
			v[1] = heightValue * h;

			v[0] *= w;
			v[2] *= l;
			v[0] -= w/2;
			v[2] -= l/2;			

			//Change object position Approach - 2  - This approach is useful for Initialize
			verts.push_back((v[0] * objectScale.x)+ objectPosition.x);
			//float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			verts.push_back((v[1] * objectScale.y) + objectPosition.y);
			verts.push_back((v[2] * objectScale.z) + objectPosition.z);

			colors.push_back(1);
			colors.push_back(1);
			colors.push_back(1);

			//Normals - Normal Map Approach
			
			// We get the normals for each vertex from the NormalMap
			/*Color normalValueColors = getPixel(heightMap, ix, iy);
			float nx = normalValueColors.r / 255.f;
			float ny = normalValueColors.g / 255.f;
			float nz = normalValueColors.b / 255.f;
			
			norms.push_back(nx);
			norms.push_back(ny);
			norms.push_back(nz);*/

			//Normal - Calculation Approach

			glm::vec3 offset = glm::vec3(1.0, 1.0, 0.0); //Ofset to get neightbor vertex
			
			int newIx = ix;
			int newIy = iy;

			//height(P.xy - off.xz);
			if(x - offset.x >=0)
				newIx = (int)roundf((x - offset.x) * (heightMap.getSize().x / wRes));
			if(z - offset.z >= 0)
				newIy = (int)roundf((z - offset.z) * (heightMap.getSize().y / lRes));

			heightValue = getPixel(heightMap, newIx, newIy).r / 255.0f;
			float hL = heightValue; 
			
			// height(P.xy + offset.xz);
			if (x + offset.x >= 0)
				newIx = (int)roundf((x + offset.x) * (heightMap.getSize().x / wRes));
			if (z + offset.z >= 0)
				newIy = (int)roundf((z + offset.z) * (heightMap.getSize().y / lRes));

			heightValue = getPixel(heightMap, newIx, newIy).r / 255.0f;
			float hR = heightValue;

			//height(P.xy - off.zy);
			if (x - offset.z >= 0)
				newIx = (int)roundf((x - offset.z) * (heightMap.getSize().x / wRes));
			if (z - offset.y >= 0)
				newIy = (int)roundf((z - offset.y) * (heightMap.getSize().y / lRes));

			heightValue = getPixel(heightMap, newIx, newIy).r / 255.0f;
			float hD = heightValue; 

			//height(P.xy + off.zy);
			if (x + offset.z >= 0)
				newIx = (int)roundf((x + offset.z) * (heightMap.getSize().x / wRes));
			if (z + offset.y >= 0)
				newIy = (int)roundf((z + offset.y) * (heightMap.getSize().y / lRes));

			heightValue = getPixel(heightMap, newIx, newIy).r / 255.0f;
			float hU = heightValue;

			// deduce terrain normal
			glm::vec3 N;
			N.x = hL - hR;
			N.y = hD - hU;
			N.z = 2.0;
			N = glm::normalize(N);

			norms.push_back(N.x);
			norms.push_back(N.y);
			norms.push_back(N.z);

			i+=3;
		}
	}



	//get index - First Approach
	//i = 0;
	//for (int z = 0; z < lRes; z++)
	//{
	//	for (int x = 0; x < wRes; x++)
	//	{
	//		if ((i + 1 % wRes) != 0 && z + 1 < lRes)
	//		{
	//			/*indices.push_back(i);
	//			indices.push_back(i + wRes);
	//			indices.push_back(i + wRes + 1);

	//			indices.push_back(i);
	//			indices.push_back(i + wRes + 1);
	//			indices.push_back(i + 1);*/

	//			/*int tri[3]{ i, i + wRes, i + wRes + 1 };
	//			int tri2[3]{ i, i + wRes + 1, i + 1 };

	//			tris.push_back(tri);
	//			tris.push_back(tri2);  CANT DO THIS  Error C3074*/
	//		}

	//		i++;
	//	}
	//}
	
	//get index - Second Approach

	for (int y = 0; y < lRes-1; ++y)
	{
		for (int x = 0; x < wRes-1; ++x)
		{
			int start = y * wRes + x;
			indices.push_back(start);
			indices.push_back(start + 1);
			indices.push_back(start + wRes);

			indices.push_back(start + 1);
			indices.push_back(start + wRes);
			indices.push_back(start + wRes + 1);
		}
	}

	

	indices.push_back(i);
	indices.push_back(i + 1);
	indices.push_back(i + wRes);

	indices.push_back(i + 1);
	indices.push_back(i + wRes);
	indices.push_back(i + wRes + 1);


	glGenBuffers(VBO_COUNT, vbo_ids);
	glGenVertexArrays(1, &vao_id);

	// Se activa el VAO del cubo para configurarlo:

	glBindVertexArray(vao_id);

	// Se suben a un VBO los datos de coordenadas y se vinculan al VAO:

	glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(coordinates), coordinates, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(GLfloat), verts.data(), GL_STATIC_DRAW); //&vertices.front() - &vertices[0]

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Se suben a un VBO los datos de color y se vinculan al VAO:

	glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COLORS_VBO]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Se suben a un VBO los datos de normales y se vinculan al VAO:

	glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[NORMALS_VBO]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, norms.size() * sizeof(GLfloat), norms.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Se suben a un IBO los datos de índices:

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_IBO]);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

Color ElevationMesh::getPixel(Image heightMap, int x, int y)
{
	return heightMap.getPixel(x, y);
}

void ElevationMesh::render(GLint hasTextue_id)
{
	glUniform1i(hasTextue_id, 0);

	glBindVertexArray(vao_id);
	glDrawElements(GL_TRIANGLES, indices.size() * sizeof(GLuint), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

//void ElevationMesh::AddChild(Model* child)
//{
//	children.push_back()
//}


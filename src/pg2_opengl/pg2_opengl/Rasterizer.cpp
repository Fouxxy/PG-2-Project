#include "pch.h"
#include "Rasterizer.h"
#include "utils.h"
#include "glutils.h"
#include "matrix4x4.h"
#include "color.h"
#include "texture.h"
#include "objloader.h"
#include <OpenImageDenoise/oidn.h>
#include <iostream>
#include "Camera.h"
#include "My_Shader.h"
#include "My_Math.h"

// STBI - Possibly replace when I figure out how to use other ways of loading images
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
bool firstMouse = true;

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
Camera _camera = Camera();


unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;

void renderCube()
{
	// initialize (if necessary)
	if (cubeVAO == 0)
	{
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}



// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	_camera.ProcessMouseMovement(xoffset, yoffset);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		_camera.ProcessKeyboard(Camera::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		_camera.ProcessKeyboard(Camera::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		_camera.ProcessKeyboard(Camera::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		_camera.ProcessKeyboard(Camera::RIGHT, deltaTime);
}

bool check_gl(const GLenum error)
{
	if (error != GL_NO_ERROR)
	{
		//const GLubyte * error_str;
		//error_str = gluErrorString( error );
		//printf( "OpenGL error: %s\n", error_str );

		return false;
	}

	return true;
}
/* glfw callback */
void glfw_callback(const int error, const char* description)
{
	printf("GLFW Error (%d): %s\n", error, description);
}
/* OpenGL messaging callback */
void GLAPIENTRY gl_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user_param)
{
	printf("GL %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "Error" : "Message"),
		type, severity, message);
}
/* invoked when window is resized */
void framebuffer_resize_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

std::string LoadAsciiFile(const std::string& file_name)
{
	std::ifstream file(file_name, std::ios::in);

	if (file)
	{
		return (std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()));
	}
	else
	{
		return "";
	}
}
/* check shader for completeness */

int Rasterizer::InitOpenGL(const int width, const int height, GLFWwindow* &window)
{
	glfwSetErrorCallback(glfw_callback);

	if (!glfwInit())
	{
		return(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);

	window = glfwCreateWindow(width, height, "PG2 OpenGL", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
	glfwMakeContextCurrent(window);
	
	// NEW mouse movement
	glfwSetCursorPosCallback(window, mouse_callback);
	
	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		if (!gladLoadGL())
		{
			return EXIT_FAILURE;
		}
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(gl_callback, nullptr);   // velice dulezite at vime co se deje na GPU

	printf("OpenGL %s, ", glGetString(GL_VERSION));
	printf("%s", glGetString(GL_RENDERER));
	printf(" (%s)\n", glGetString(GL_VENDOR));
	printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glEnable(GL_MULTISAMPLE);

	// map from the range of NDC coordinates <-1.0, 1.0>^2 to <0, width> x <0, height>
	
	glViewport(0, 0, width, height); // toto je ten prevod z obrazoveho rozmezi -1, 1 do pixelu
	
	// default je lower_left, tedy koordinaty obrazu zacinaji v levem dolnim rohu 0, 0
	// GL_LOWER_LEFT (OpenGL) or GL_UPPER_LEFT (DirectX, Windows) and GL_NEGATIVE_ONE_TO_ONE or GL_ZERO_TO_ONE
	glClipControl(GL_UPPER_LEFT, GL_NEGATIVE_ONE_TO_ONE); // coordinaty zacinaji v levem hornim rohu, 
	//GL_NEGATIVE_ONE_TO_ONE urcuje jak bude nastavena ta pseudohloubka
}

void Rasterizer::LoadMesh(const std::string file_name)
{
	
	//MaterialLibrary materials;
	
	

	LoadOBJ(file_name, _scene, _materials);

	for (SceneGraph::iterator iter = _scene.begin(); iter != _scene.end(); ++iter)
	{
		const std::string& node_name = iter->first;
		const auto& node = iter->second;
		const auto& mesh = std::static_pointer_cast<Mesh>(node);
		if (mesh)
		{
			for (Mesh::iterator iter = mesh->begin(); iter != mesh->end(); ++iter)
			{

				const auto& src_triangle = Triangle3i(**iter);
				std::shared_ptr<Material> material = iter.triangle_material();
				const int material_index = int(std::distance(std::begin(_materials), _materials.find(material->name())));
				
				Triangle3 triangle;
				for (int i = 0; i < 3; ++i)
				{
					triangle.vertices[i].position = src_triangle.position(i);
					triangle.vertices[i].normal = src_triangle.normal(i);
					triangle.vertices[i].color = Vector3(1.0f, 1.0f, 1.0f);
					triangle.vertices[i].texture_coord = Vector2(src_triangle.texture_coord(i).x, src_triangle.texture_coord(i).y);
					triangle.vertices[i].material_index = material_index;
					// tangent from obj
					//triangle.vertices[i].tangent = src_triangle.tangent(i);
					//triangle.vertices[i].bitangent = triangle.vertices[i].normal.CrossProduct(triangle.vertices[i].tangent);
					// calculated tangent
					
				}		
				
				// Calculate tangent and bitangent per triangle
				Vector3 e1 = triangle.vertices[1].position - triangle.vertices[0].position;
				Vector3 e2 = triangle.vertices[2].position - triangle.vertices[0].position;
				Vector2 uv1 = triangle.vertices[1].texture_coord - triangle.vertices[0].texture_coord;
				Vector2 uv2 = triangle.vertices[2].texture_coord - triangle.vertices[0].texture_coord;
				Vector3 tangent;
				Vector3 bitangent;
				float eqPart1 = 1.0f / (uv1.x * uv2.y - uv2.x * uv1.y);
				//tangent.x = eqPart1 * (uv2.y * e1.x - uv1.y * e2.x);
				//tangent.y = eqPart1 * (uv2.y * e1.y - uv1.y * e2.y);
				//tangent.z = eqPart1 * (uv2.y * e1.z - uv1.y * e2.z);
				tangent = eqPart1 * (e1 * uv2.y - e2 * uv1.y);
				//bitangent.x = eqPart1 * (-uv2.x * e1.x + uv1.x * e2.x);
				//bitangent.y = eqPart1 * (-uv2.x * e1.y + uv1.x * e2.y);
				//bitangent.z = eqPart1 * (-uv2.x * e1.z + uv1.x * e2.z);
				bitangent = eqPart1 * (e2 * uv1.x - e1 * uv2.x);
				/*
				// Gram-Schmidt
				Vector3 normal = triangle.vertices[0].normal;
				//normal.Normalize();
				Vector3 T = tangent - (tangent.DotProduct(normal) * normal);
				T.Normalize();
				Vector3 B = normal.CrossProduct(T);

				triangle.vertices[0].tangent = T;
				triangle.vertices[1].tangent = T;
				triangle.vertices[2].tangent = T;
				triangle.vertices[0].bitangent = B;
				triangle.vertices[1].bitangent = B;
				triangle.vertices[2].bitangent = B;
				
				*/
				//bitangent = triangle.vertices[0].normal.CrossProduct(tangent);
				triangle.vertices[0].tangent = tangent;
				triangle.vertices[1].tangent = tangent;
				triangle.vertices[2].tangent = tangent;
				triangle.vertices[0].bitangent = bitangent;
				triangle.vertices[1].bitangent = bitangent;
				triangle.vertices[2].bitangent = bitangent;
				
				_triangles.push_back(triangle);
				
			}
			std::vector<Triangle3> triangles2 = _triangles;
			for (int i = 0; i < _triangles.size(); i++) {
				for (int j = 0; j < triangles2.size(); j++) {
					if (_triangles.data()->vertices[i].position == triangles2.data()->vertices[j].position && _triangles.data()->vertices[i].texture_coord == triangles2.data()->vertices[j].texture_coord && i!=j) {
						_triangles.data()->vertices[i].tangent += triangles2.data()->vertices[j].tangent;
						_triangles.data()->vertices[i].bitangent += triangles2.data()->vertices[j].bitangent;
					}
				}
			} 

			
		}
	}	
}

void Rasterizer::InitBuffersCubes(GLuint& vao, GLuint& vbo, GLuint& ebo)
{
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	// tedy nemusim pak vypisovat stejny vertex vicekrat u navazujicich trojuhelniku


	//const int no_vertices = 3;
	//const int no_vertices = sizeof(Triangle3);
	//const int vertex_stride = sizeof(vertices) / no_vertices;
	//const int vertex_stride = sizeof(Vertex3);
	// sizeof(Triangle3)
	// optional index array

	/*
	
	unsigned int indices[] =
	{
		0, 1, 2
	};
	*/
	

	// Jak to dostaneme do GPU, stale jsme v RAM, vytvorime buffery:
	// VAO a VBO, VAO = Vertex Array Object, VBO = Vertex Buffer Object
	// GLuint vao = 0; // VAO udrzuje popis struktury sceny - vertexy, normala, indexovani ... atd
	glGenVertexArrays(1, &vao); // 1 je ID, primarni klic v databazi 
	glBindVertexArray(vao); // nabinduju VAO pro urcitou operaci
	//GLuint vbo = 0; // VBO 
	glGenBuffers(1, &vbo); // generate vertex buffer object (one of OpenGL objects) and get the unique ID corresponding to that buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // bind the newly created buffer to the GL_ARRAY_BUFFER target

	//for(int i = 0; i < _triangles.pop_back();


	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // copies the previously defined vertex data into the buffer's memory
	

	// vertex position 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); //toto vicemene urcuje o kolik bajtu musim skocit, abych se dostal na dalsi pixel
	// protoze vertices jsou jen hodnoty oddelene carkou, takze opengl nevi, kolik informaci patri k jednomu vertexu (x,y,z, normala, texturoaci souradnice ..atd)
	// 0 - ID prvku ve strukture
	// 3 - skok
	glEnableVertexAttribArray(0); // tedka ten atribute array jeste povolim
	// vertex texture coordinates 
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));; // timto urcim ze dalsi 2 hodnoty jsou texturovaci souracnice
	glEnableVertexAttribArray(1); // zase to bindnu
	//GLuint ebo = 0; // optional buffer of indices

	/**/
	/*
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	*/
}

void Rasterizer::InitBuffersMesh(GLuint &vao, GLuint &vbo, GLuint &ebo)
{
	
	const int vertex_stride =  sizeof(Vertex3);
	
	glGenVertexArrays(1, &vao); // 1 je ID, primarni klic v databazi 
	glBindVertexArray(vao); // nabinduju VAO pro urcitou operaci
	//GLuint vbo = 0; // VBO 
	glGenBuffers(1, &vbo); // generate vertex buffer object (one of OpenGL objects) and get the unique ID corresponding to that buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // bind the newly created buffer to the GL_ARRAY_BUFFER target
	glBufferData(GL_ARRAY_BUFFER, vertex_stride * 3 * _triangles.size(), _triangles.data() , GL_STATIC_DRAW); // copies the previously defined vertex data into the buffer's memory
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertex_stride, 0); //toto vicemene urcuje o kolik bajtu musim skocit, abych se dostal na dalsi pixel
	glEnableVertexAttribArray(0);
	// Texture Coords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertex_stride, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
	// Normal
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, vertex_stride, (void*)(sizeof(float) * 5)); 
	glEnableVertexAttribArray(2); 
	// Tangent
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, vertex_stride, (void*)(sizeof(float) * 8));
	glEnableVertexAttribArray(3);
	// Bitangent
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, vertex_stride, (void*)(sizeof(float) * 11));
	glEnableVertexAttribArray(4);
	// Color
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, vertex_stride, (void*)(sizeof(float) * 14));
	glEnableVertexAttribArray(5); 
}

void Rasterizer::LoadTexturesCube(GLuint &shader_program, unsigned int &texture) {

	// Load texture using stbi 
	// TODO make into function

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int tex_width, tex_height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load("container.jpg", &tex_width, &tex_height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	// texture 2
	// ---------
	

	glUniform1i(glGetUniformLocation(shader_program, "texture1"), 0);
	glUseProgram(shader_program);
};

void Rasterizer::test_printMatrix(Matrix4x4& matrix) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			printf("%0.02f ", matrix.get(j, i));
		}
		printf("\n");
	}
}

int Rasterizer::MainLoop(int width, int height) {
	_screen_width = width;
	_screen_height = height;
	
	GLFWwindow* window = nullptr;

	InitOpenGL(_screen_width, _screen_height, window);
	
	// color_shader("normal_shader.vert", "normal_shader.frag");
	My_Shader pbr_shader("pbr_shader.vert", "pbr_shader.frag");
	My_Shader sphericalToCube("sphericalToCube.vert", "sphericalToCube.frag");
	My_Shader cubeMap("cubeMap.vert", "cubeMap.frag");
	
	
	// Create buffers
	GLuint vao = 0;
	GLuint vbo = 0;
	GLuint ebo = 0;
	//InitBuffersCubes(vao, vbo, ebo);
	//LoadMesh("../../../data/6887_allied_avenger_gi2.obj");
	//LoadMesh("../../../data/6887_allied_avenger_gi2.obj");
	//LoadMesh("../../../data/piece_02.obj");
	//LoadMesh("../../../data/unit_cube.obj");
	// Load models and textures
	LoadMesh("../../../data/piece_02.obj");
	/*
	unsigned int fake_texture = loadTexture("../../../data/fake_texture.png");
	unsigned int diffuse_texture = loadTexture("../../../data/scuffed-plastic2-alb.png");
	unsigned int specular_texture = loadTexture("../../../data/scuffed-plastic2-alb.png");
	
	unsigned int albedo = loadTexture("../../../data/Textures/rustediron_C.png");
	unsigned int normal = loadTexture("../../../data/Textures/rustediron_N.png");
	unsigned int metallic = loadTexture("../../../data/Textures/rustediron_M.png");
	unsigned int roughness = loadTexture("../../../data/Textures/rustediron_R.png");
	unsigned int AO = loadTexture("../../../data/Textures/rustediron_AO.png");
	*/

	unsigned int albedo = loadTexture("../../../data/scuffed-plastic6-alb.png");
	unsigned int normal = loadTexture("../../../data/scuffed-plastic-normal.png");
	unsigned int metallic = loadTexture("../../../data/plastic_02_rma.png");
	unsigned int roughness = loadTexture("../../../data/plastic_02_rma.png");
	unsigned int AO = loadTexture("../../../data/plastic_02_rma.png");
	// enviroment maps
	

	pbr_shader.use();
	pbr_shader.setInt("material.albedo", 0);
	pbr_shader.setInt("material.normal", 1);
	pbr_shader.setInt("material.metallic", 2);
	pbr_shader.setInt("material.roughness", 3);
	pbr_shader.setInt("material.AO", 4);
	
	
	// pbr: setup framebuffer
	// ----------------------
	unsigned int captureFBO;
	unsigned int captureRBO;
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);
	
	// load maps
	unsigned int EXR_irradiance = LoadIrradianceMap("../../../data/enviroment/lebombo_prefiltered_env_map_001_2048.exr");
	std::vector<std::string> fileNames;
	fileNames.push_back("../../../data/enviroment/lebombo_prefiltered_env_map_001_2048.exr");
	fileNames.push_back("../../../data/enviroment/lebombo_prefiltered_env_map_010_1024.exr");
	fileNames.push_back("../../../data/enviroment/lebombo_prefiltered_env_map_100_512.exr");
	fileNames.push_back("../../../data/enviroment/lebombo_prefiltered_env_map_250_256.exr");
	fileNames.push_back("../../../data/enviroment/lebombo_prefiltered_env_map_500_128.exr");
	fileNames.push_back("../../../data/enviroment/lebombo_prefiltered_env_map_750_64.exr");
	fileNames.push_back("../../../data/enviroment/lebombo_prefiltered_env_map_999_32.exr");
	unsigned int EXR_prefiltered = LoadPrefilteredEnvMap(pbr_shader, fileNames);
	unsigned int EXR_GGX = LoadGGXIntegrMap("../../../data/enviroment/brdf_integration_map_ct_ggx.exr");

	unsigned int envCubemap;
	glGenTextures(1, &envCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	// pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
	// ----------------------------------------------------------------------------------------------
	Matrix4x4 captureProjection = _camera.BuildProjectionMatrix(1.0f, 90.0f, 0.1f, 10.0f);
		
	
	Matrix4x4 captureViews[] =
	{
		_camera.BuildViewMatrix(Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f,  0.0f,  0.0f), Vector3(0.0f, -1.0f,  0.0f)),
		_camera.BuildViewMatrix(Vector3(0.0f, 0.0f, 0.0f), Vector3(-1.0f,  0.0f,  0.0f), Vector3(0.0f, -1.0f,  0.0f)),
		_camera.BuildViewMatrix(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f,  1.0f,  0.0f), Vector3(0.0f,  0.0f,  1.0f)),
		_camera.BuildViewMatrix(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, -1.0f,  0.0f), Vector3(0.0f,  0.0f, -1.0f)),
		_camera.BuildViewMatrix(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f,  0.0f,  1.0f), Vector3(0.0f, -1.0f,  0.0f)),
		_camera.BuildViewMatrix(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f,  0.0f, -1.0f), Vector3(0.0f, -1.0f,  0.0f)),
			
	};

	// pbr: convert HDR equirectangular environment map to cubemap equivalent
	// ----------------------------------------------------------------------
	sphericalToCube.use();
	sphericalToCube.setInt("equirectangularMap", 0);
	SetMatrix4x4(sphericalToCube._shader_program, captureProjection.data(), "projection");
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, EXR_irradiance);

	glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	for (unsigned int i = 0; i < 6; ++i)
	{
		sphericalToCube.setMat4("view", captureProjection);
		sphericalToCube.setMat4("view", captureViews[i]);
				
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderCube();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// then let OpenGL generate mipmaps from first mip face (combatting visible dots artifact)
	glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		
	//	Material material;
	//material = _materials.find("1")->first;
	//Color3f diffuse = material.albedo();
	InitBuffersMesh(vao, vbo, ebo);
	//_materials;
	//_triangles[1].vertices[1].position;
	/*
	unsigned int texture1;
	LoadTexturesCube(color_shader._shader_program, texture1);
	*/
	// main loop
	Vector3 lightPosition = Vector3(50.0f, 1.0f, 100.0f);

	Matrix4x4 projection = _camera.BuildProjectionMatrix((float)_screen_width / (float)_screen_height, 45, 0.1f, 100.0f);
	cubeMap.use();
	cubeMap.setInt("cubeMap", 0);
	cubeMap.setMat4("projection", projection);

	while (!glfwWindowShouldClose(window))
	{
		glViewport(0, 0, width, height);
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// input
		processInput(window);
		// NASTAVIM PLATNO, orto projekce, bez kamery
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // state setting function
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // state using function

		//GLint viewport[4]; // https://docs.gl/es2/glViewport
		//glGetIntegerv(GL_VIEWPORT, viewport); // timto asi dostanu pocatecni pozici (roh) vykreslovaciho okna na pozici (0 = x, 1 = y, 2 = width, 3 = height)

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture1);
	

		// activate shader
		pbr_shader.use();
		// setting uniforms
		pbr_shader.setVec3("cameraPosition", _camera._camera_position.x, _camera._camera_position.y, _camera._camera_position.z);
	
		// light attributes
		pbr_shader.setVec3("light.position", lightPosition.x, lightPosition.y, lightPosition.z);		
		pbr_shader.setVec3("light.color", 5000.0f, 5000.0f, 5000.0f); // darken diffuse light a bit
	
			
		// bind textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, albedo);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, metallic);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, roughness);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, AO);
		// enviroment textures binding
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
		glActiveTexture(GL_TEXTURE6);

		glBindTexture(GL_TEXTURE_2D, EXR_irradiance);
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, EXR_prefiltered);
		glActiveTexture(GL_TEXTURE8);
		glBindTexture(GL_TEXTURE_2D, EXR_GGX);
		
		// model view projection
		Matrix4x4 model = Matrix4x4(); // toto vyplni matici jako identitu
		//Camera::Scale_uniform(model, 10.0f);
		Matrix4x4 view = _camera.GetViewMatrix();
		
		Matrix4x4 P = Matrix4x4(); // nastavuje uniformni promennou P v shaderu, takto presypavam pro kazdy frame data z CPU na GPU
				
		P = projection * view * model;

		//SetMatrix4x4(pbr_shader._shader_program, model.data(), "model");
		SetMatrix4x4(pbr_shader._shader_program, P.data(), "P");
		
		// render MESH
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, _triangles.size() * sizeof(Vertex3));

		cubeMap.use();
		cubeMap.setMat4("view", view);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
		renderCube();
						
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	
	pbr_shader.Delete(); 

	//glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glfwTerminate();
	
	return EXIT_SUCCESS;
	//return 0;
}

void Rasterizer::DrawMesh()
{
}

GLuint Rasterizer::LoadIrradianceMap(std::string fileName)
{
	Texture3f irradiance_map = Texture3f(fileName);

	GLuint ID;
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	if (glIsTexture(ID)) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, irradiance_map.width(), irradiance_map.height(), 0, GL_RGB, GL_FLOAT, irradiance_map.data());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	return ID;
}

GLuint Rasterizer::LoadPrefilteredEnvMap(My_Shader &shader, std::vector<std::string> fileNames)
{
	const GLint maxLevel = GLint(fileNames.size()) - 1; // assume we have a list of images representing different levels of a map
	GLuint ID;
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);
	if (glIsTexture(ID)) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, maxLevel);
		int width, height;
		GLint level;
		for (level = 0; level < GLint(fileNames.size()); ++level) {
			Texture3f prefiltered_env_map = Texture3f(fileNames[level]);
			// for HDR images use GL_RGB32F or GL_RGB16F as internal format !!!
			glTexImage2D(GL_TEXTURE_2D, level, GL_RGB32F, prefiltered_env_map.width(), prefiltered_env_map.height(), 0, GL_RGB, GL_FLOAT,
				prefiltered_env_map.data());
			width = prefiltered_env_map.width() / 2;
			height = prefiltered_env_map.height() / 2;
		}
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	shader.setInt("max_level", maxLevel);
	return ID;
}

GLuint Rasterizer::LoadGGXIntegrMap(std::string fileName)
{
	Texture3f ggxIntegrMap = Texture3f(fileName);
	GLuint ID;
	glGenTextures(1, &ID);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, ID);

	if (glIsTexture(ID)) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, ggxIntegrMap.width(), ggxIntegrMap.height(), 0, GL_RGB, GL_FLOAT, ggxIntegrMap.data());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	return ID;
}

unsigned int Rasterizer::loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

unsigned int Rasterizer::loadEXR(char const* path)
{
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrComponents;
	float* data = stbi_loadf(path, &width, &height, &nrComponents, 0);
	unsigned int hdrTexture;
	if (data)
	{
		glGenTextures(1, &hdrTexture);
		glBindTexture(GL_TEXTURE_2D, hdrTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Failed to load HDR image." << std::endl;
	}

	return hdrTexture;
}


#ifndef RASTERIZER_H_
#define RASTERIZER_H_


#include "utils.h"
#include "glutils.h"
#include "matrix4x4.h"
#include "color.h"
#include "texture.h"
#include "objloader.h"
#include "Camera.h"
#include "My_Shader.h"



struct Vertex3
{
	Vector3 position; /* vertex position */
	Vector2 texture_coord; /* vertex texture coordinate */
	Vector3 normal; /* vertex normal */
	Vector3 tangent; /* vertex tangent */
	Vector3 bitangent;
	Vector3 color; /* vertex color */
	int material_index; /* material index */
};
struct Triangle3
{
	// v tomto jsou tada pro sest vertexu, kde 3 z toho jsou sousedici
	std::array<Vertex3, 3> vertices;
};

struct TriangleWithAdjacency
{
	// v tomto jsou tada pro sest vertexu, kde 3 z toho jsou sousedici
	std::array<Vertex3, 6> vertices;
};
/*
bool check_gl(const GLenum error = glGetError());
void glfw_callback(const int error, const char* description);
void GLAPIENTRY gl_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user_param);
void framebuffer_resize_callback(GLFWwindow* window, int width, int height);
*/

class Rasterizer {
public:
	
	int InitOpenGL(const int width, const int height, GLFWwindow*& window);
	void LoadMesh(const std::string file_name);
	void InitBuffersCubes(GLuint& vao, GLuint& vbo, GLuint& ebo);
	//tutorial 7
	void InitBuffersMesh(GLuint& vao, GLuint& vbo, GLuint& ebo);
	void LoadTexturesCube(GLuint& shader_program, unsigned int &texture);
	int MainLoop(const int width, const int height);
	// euklidovksa inverze pomoci opengl funkce inverse	
	void DrawMesh();
	GLuint LoadIrradianceMap(std::string fileName);

	GLuint LoadPrefilteredEnvMap(My_Shader& shader, std::vector<std::string> fileNames);

	GLuint LoadGGXIntegrMap(std::string fileName);
	
	unsigned int loadTexture(char const* path);

	unsigned int loadEXR(char const* path);
	
private:
	
	std::vector<Triangle3> _triangles;
	SceneGraph _scene;
	MaterialLibrary _materials;
	int _screen_width = 1920;
	int _screen_height = 1080;
	void test_printMatrix(Matrix4x4& print);

};



#endif
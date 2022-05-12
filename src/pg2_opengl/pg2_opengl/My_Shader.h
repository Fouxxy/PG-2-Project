#include <matrix4x4.h>
#ifndef SHADER_H
#define SHADER_H
#pragma once



class My_Shader
{
public:
	GLuint _shader_program;
	My_Shader(const char* vertexPath, const char* fragmentPath);
	void use();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	//void setVec3(const std::string& name, const Vector3& value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	void setMat4(const std::string& name, Matrix4x4 value);
	void Delete();
private:
	GLint CheckShader(const GLenum shader, const char* name);
	void CheckLink(GLuint shader_program);
};

#endif
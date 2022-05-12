#include "pch.h"
#include "My_Shader.h"
#include "utils.h"
//#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector3.h>
#include "glutils.h"

My_Shader::My_Shader(const char* vertexPath, const char* fragmentPath)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "Error reading shader file: " << e.what() << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // vertex shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vShaderCode, NULL);
    glCompileShader(vertex_shader);
    CheckShader(vertex_shader, "vertex shader");

    //Fragment shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);        
    glShaderSource(fragment_shader, 1, &fShaderCode, nullptr);
    glCompileShader(fragment_shader);
    CheckShader(fragment_shader, "fragment_shader");

    //Shader Program
    _shader_program = glCreateProgram();
    glAttachShader(_shader_program, vertex_shader);
    glAttachShader(_shader_program, fragment_shader);
    glLinkProgram(_shader_program);
    CheckLink(_shader_program);
    
    
    //settings
    glPointSize(10.0f); // nastaveni velikosti bodu pro glDrawArrays
    glLineWidth(1.0f); // nastaveni sirky cary pro glDrawArrays
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    // delete unnecesarry data
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void My_Shader::use()
{
    glUseProgram(_shader_program);
}

void My_Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(_shader_program, name.c_str()), (int)value);
}

void My_Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(_shader_program, name.c_str()), value);
}

void My_Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(_shader_program, name.c_str()), value);
}
/*
void My_Shader::setVec3(const std::string& name, const Vector3& value) const
{
    glUniform3fv(glGetUniformLocation(_shader_program, name.c_str()), 1, &value[0]);
}
*/
void My_Shader::setVec3(const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(_shader_program, name.c_str()), x, y, z);
}

void My_Shader::setMat4(const std::string& name, Matrix4x4 value)
{
    SetMatrix4x4(_shader_program, value.data(), name.c_str());
}

void My_Shader::Delete()
{
    glDeleteProgram(_shader_program);
}

GLint My_Shader::CheckShader(const GLenum shader, const char* name)
{
    GLint status = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    printf("Shader compilation of %s ", name);
    printf("%s.\n", (status == GL_TRUE) ? "was successful" : "FAILED");

    if (status == GL_FALSE)
    {
        int info_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_length);
        char* info_log = new char[info_length];
        memset(info_log, 0, sizeof(*info_log) * info_length);
        glGetShaderInfoLog(shader, info_length, &info_length, info_log);

        printf("Error log: %s\n", info_log);

        SAFE_DELETE_ARRAY(info_log);
    }
    return status;
}

void My_Shader::CheckLink(GLuint shader_program) {
    int success;
    char infoLog[1024];

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader_program, 1024, NULL, infoLog);
        std::cout << "ERROR::PROGRAM_LINKING_ERROR "  << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }
}
#include "pch.h"
#include "glutils.h"


void SetInt( const GLuint program, GLint value, const char * int_name )
{
	const GLint location = glGetUniformLocation( program, int_name );

	if ( location == -1 )
	{
		printf( "Integer value '%s' not found in active shader.\n", int_name );
	}
	else
	{
		glUniform1i( location, value );
	}
}

void SetSampler( const GLuint program, GLenum texture_unit, const char * sampler_name )
{
	const GLint location = glGetUniformLocation( program, sampler_name );

	if ( location == -1 )
	{
		printf( "Texture sampler '%s' not found in active shader.\n", sampler_name );
	}
	else
	{
		glUniform1i( location, texture_unit );
	}
}

void SetMatrix4x4( const GLuint program, const GLfloat * data, const char * matrix_name )
{	
	const GLint location = glGetUniformLocation( program, matrix_name );

	if ( location == -1 )
	{
		printf( "Matrix '%s' not found in active shader.\n", matrix_name );
	}
	else
	{
		glUniformMatrix4fv( location, 1, GL_TRUE, data );
	}
}

void SetVector3( const GLuint program, const GLfloat * data, const char * vector_name )
{
	const GLint location = glGetUniformLocation( program, vector_name );

	if ( location == -1 )
	{
		printf( "Vector '%s' not found in active shader.\n", vector_name );
	}
	else
	{
		glUniform3fv( location, 1, data );
	}
}

void SetVector2( const GLuint program, const GLfloat * data, const char * vector_name )
{
	const GLint location = glGetUniformLocation( program, vector_name );

	if ( location == -1 )
	{
		printf( "Vector '%s' not found in active shader.\n", vector_name );
	}
	else
	{
		glUniform2fv( location, 1, data );
	}
}

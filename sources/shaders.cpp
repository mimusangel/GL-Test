#include <stdexcept>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "shaders.hpp"

static GLint	get_program_log(GLuint shader_id)
{
	GLint	result;
	char	*buf;
	int		info_log_length;

	glGetProgramiv(shader_id, GL_LINK_STATUS, &result);
	if (result == 0)
	{
		glGetProgramiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
		if (!info_log_length)
			return (result);
		buf = (char*)malloc((size_t)info_log_length);
		glGetProgramInfoLog(shader_id, info_log_length, NULL, buf);
		std::cout << buf << '\n';
		free(buf);
	}
	return (result);
}

static GLint	get_shader_log(GLuint shader_id)
{
	GLint	result;
	char	*buf;
	int		info_log_length;

	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
	if (result == 0)
	{
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
		if (!info_log_length)
			return (result);
		buf = (char*)malloc((size_t)info_log_length);
		glGetShaderInfoLog(shader_id, info_log_length, NULL, buf);
		std::cout << buf << '\n';
		free(buf);
	}
	return (result);
}

Shaders::Shaders(int size)
{
	_program = 0;
	_shaders = new GLuint[size];
	_size = size;
}

Shaders::~Shaders()
{
	if (_program)
		glDeleteProgram(_program);
	delete[] _shaders;
}

std::string		Shaders::GetFileContent(const char *path)
{
	std::ifstream ifs(path);
  	std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	return (content);
}

int				Shaders::load(ShadersType type, const char *script)
{
	if (type == GEOMETRY_SHADER)
		_shaders[type] = glCreateShader(GL_GEOMETRY_SHADER);
	else if (type == VERTEX_SHADER)
		_shaders[type] = glCreateShader(GL_VERTEX_SHADER);
	else
		_shaders[type] = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(_shaders[type], 1, (const GLchar *const *)&script, NULL);
	glCompileShader(_shaders[type]);
	return (get_shader_log(_shaders[type]));
}

int				Shaders::build(void)
{
	_program = glCreateProgram();
	if (!_program)
		return (0);
	for (size_t i = 0; i < _size; i++)
		glAttachShader(_program, _shaders[i]);
	glLinkProgram(_program);
	if (!get_program_log(_program))
		return (0);
	for (size_t i = 0; i < _size; i++)
	{
		glDetachShader(_program, _shaders[i]);
		glDeleteShader(_shaders[i]);
	}
	return (1);
}

void			Shaders::bind(void)
{
	glUseProgram(_program);
}

void			Shaders::uniformMat4(GLchar *name, GLfloat *mat)
{
	GLint id = glGetUniformLocation(_program, name);
	glUniformMatrix4fv(id, 1, GL_FALSE, mat);
}

void			Shaders::uniform1f(GLchar *name, GLfloat value)
{
	GLint id = glGetUniformLocation(_program, name);
	glUniform1f(id, value);
}

void			Shaders::uniform2f(GLchar *name, GLfloat *value)
{
	GLint id = glGetUniformLocation(_program, name);
	glUniform2fv(id, 1, value);
}

void			Shaders::uniform2fv(GLchar *name, GLfloat x, GLfloat y)
{
	GLint id = glGetUniformLocation(_program, name);
	glUniform2f(id, x, y);
}

void			Shaders::uniform3f(GLchar *name, GLfloat *value)
{
	GLint id = glGetUniformLocation(_program, name);
	glUniform3fv(id, 1, value);
}

void			Shaders::uniform4f(GLchar *name, GLfloat *value)
{
	GLint id = glGetUniformLocation(_program, name);
	glUniform4fv(id, 1, value);
}

void			Shaders::uniform1i(GLchar *name, GLint value)
{
	GLint id = glGetUniformLocation(_program, name);
	glUniform1i(id, value);
}

void			Shaders::uniform1ia(GLchar *name, GLsizei count, const GLint *value)
{
	GLint id = glGetUniformLocation(_program, name);
	glUniform1iv(id, count, value);
}

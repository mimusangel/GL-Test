#ifndef SHADERS_HPP
# define SHADERS_HPP

# include "global.hpp"

# ifndef BUFF_SIZE
#  define BUFF_SIZE 8192
# endif

#define GLSL_SCRIPT(src) "#version 330 core\n" #src

enum ShadersType {
	FRAGMENT_SHADER = 0,
	VERTEX_SHADER = 1,
	GEOMETRY_SHADER = 2
};

class Shaders {
private:
	GLuint			_program;
	GLuint			*_shaders;
	int				_size;
public:
	Shaders(int size = 2);
	~Shaders();
	int				load(ShadersType type, const char *script);
	int				build(void);
	void			bind(void);
	void			uniformMat4(GLchar *name, GLfloat *mat);
	void			uniform1f(GLchar *name, GLfloat value);
	void			uniform2f(GLchar *name, GLfloat *value);
	void			uniform2fv(GLchar *name, GLfloat x, GLfloat y);
	void			uniform3f(GLchar *name, GLfloat *value);
	void			uniform4f(GLchar *name, GLfloat *value);
	void			uniform1i(GLchar *name, GLint value);
	void			uniform1ia(GLchar *name, GLsizei count, const GLint *value);

	static std::string			GetFileContent(const char *path);
};
#endif

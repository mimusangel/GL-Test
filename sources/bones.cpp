#include "bones.hpp"

/* ******************** */
/* *	  STATIC	  * */
/* ******************** */
Mesh			*Bones::_renderer = nullptr;
void			Bones::LoadRenderer(void)
{
	_renderer = new Mesh(2);
	_renderer->begin();
	float	vertexBonesPos[] = {
		0.1, -.1, -0.1,	-.1, -.1, -0.1,	0.0, 0.0, 0.0,
		-.1, 0.1, -0.1,	0.1, 0.1, -0.1,	0.0, 0.0, 0.0,
		0.1, 0.1, -0.1,	0.1, -.1, -0.1,	0.0, 0.0, 0.0,
		-.1, -.1, -0.1,	-.1, 0.1, -0.1,	0.0, 0.0, 0.0,

		-.1, -.1, -0.1,	0.1, -.1, -0.1,	0.0, 0.0, -1.0,
		0.1, 0.1, -0.1,	-.1, 0.1, -0.1,	0.0, 0.0, -1.0,
		-.1, 0.1, -0.1,	-.1, -.1, -0.1,	0.0, 0.0, -1.0,
		0.1, -.1, -0.1,	0.1, 0.1, -0.1,	0.0, 0.0, -1.0,
	};

	float	vertexBonesColor[] = {
		1.0, 1.0, 1.0,	1.0, 1.0, 1.0,	1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,	1.0, 1.0, 1.0,	1.0, 1.0, 1.0,
		0.9, 0.9, 0.9,	0.9, 0.9, 0.9,	0.9, 0.9, 0.9,
		0.9, 0.9, 0.9,	0.9, 0.9, 0.9,	0.9, 0.9, 0.9,

		0.9, 0.9, 0.9,	0.9, 0.9, 0.9,	0.9, 0.9, 0.9,
		0.9, 0.9, 0.9,	0.9, 0.9, 0.9,	0.9, 0.9, 0.9,
		0.8, 0.8, 0.8,	0.8, 0.8, 0.8,	0.8, 0.8, 0.8,
		0.8, 0.8, 0.8,	0.8, 0.8, 0.8,	0.8, 0.8, 0.8,
	};
	_renderer->add(0, GL_FLOAT, 3, (void *)vertexBonesPos, 24);
	_renderer->add(1, GL_FLOAT, 3, (void *)vertexBonesColor, 24);
	_renderer->end();
}

void			Bones::UnLoadRenderer(void)
{
	if (_renderer != nullptr)
		delete _renderer;
}

/* ******************** */
/* *	  MEMBER	  * */
/* ******************** */
Bones::Bones(glm::vec3 pos, glm::vec3 rot, float length) : _pos(pos), _rot(rot), _length(length)
{
	_parent = nullptr;
	_controller = false;
}

Bones::~Bones()
{
	while (!_children.empty())
	{
		delete _children.back();
    	_children.pop_back();
	}
}

Bones		&Bones::addChild(Bones *bones)
{
	_children.push_back(bones);
	bones->setParent(this);
	return (*this);
}

Bones		&Bones::setParent(Bones *bones)
{
	_parent = bones;
	return (*this);
}

Bones		&Bones::setController(bool controller)
{
	_controller = controller;
	return (*this);
}

glm::mat4	Bones::getMatrix(bool byChild)
{
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), _pos);
	translate = translate * glm::eulerAngleXYZ(glm::radians(_rot[0]), glm::radians(_rot[1]), glm::radians(_rot[2]));
	// translate = glm::rotate(translate, glm::radians(_rot[2]), glm::vec3(0, 0, 1));
	// translate = glm::rotate(translate, glm::radians(_rot[1]), glm::vec3(0, 1, 0));
	// translate = glm::rotate(translate, glm::radians(_rot[0]), glm::vec3(1, 0, 0));
	if (_controller)
	{
		translate = glm::translate(translate, glm::vec3(0.0, 0.0, _length));
	}
	if (!byChild)
	{
		translate = glm::scale(translate, glm::vec3(1.0f, 1.0f, 1.0f * _length));
	}
	else
	{
		translate = glm::translate(translate, glm::vec3(0.0f, 0.0f, -_length));
	}
	if (_parent != nullptr)
	{
		return (_parent->getMatrix(true) * translate);
	}
	return (translate);
}

void		Bones::render(Shaders *shaders)
{
	glm::mat4 translate = getMatrix();
	shaders->uniformMat4((GLchar *)"model", (GLfloat *)glm::value_ptr(translate));
	_renderer->render();

	for (std::vector<Bones *>::iterator it = _children.begin() ; it != _children.end(); ++it)
	{
		(*it)->render(shaders);
	}
}

void		Bones::test(void)
{
	_rot[1] += (45.0 / 60.0);
}

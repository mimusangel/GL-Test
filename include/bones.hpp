#pragma once

# include <vector>
# include "global.hpp"
# include "mesh.hpp"
# include "shaders.hpp"

class Bones {
/* ******************** */
/* *	  STATIC	  * */
/* ******************** */
public:
	static void			LoadRenderer(void);
	static void			UnLoadRenderer(void);

private:
	static Mesh			*_renderer;

/* ******************** */
/* *	  MEMBER	  * */
/* ******************** */
private:
	glm::vec3				_pos;
	glm::vec3				_rot;
	float					_length;
	Bones					*_parent;
	std::vector<Bones *>	_children;
	bool					_controller;

public:
	Bones(glm::vec3 pos = glm::vec3(0.0), glm::vec3 rot = glm::vec3(0.0), float length = 1.0f);
	~Bones();

	Bones		&addChild(Bones *bones);
	Bones		&setParent(Bones *bones);
	Bones		&setController(bool controller);
	glm::mat4	getMatrix(bool byChild = false);
	void		render(Shaders *shaders);

	void		test(void);
};

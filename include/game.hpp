#pragma once

# include "global.hpp"
# include "mesh.hpp"
# include "shaders.hpp"
# include "texture.hpp"
# include "bones.hpp"

class Game {
private:
	int			_fps;
	int			_ups;

	Shaders		*_shaders;
	Shaders		*_orthoShaders;
	Mesh		*_mesh;
	// Mesh		*_bones;
	Bones		*_bones;

	void	load(void);
	void	init(void);
public:
	GLFWwindow	*window;
	glm::mat4	projection;
	glm::vec3	camPos;
	float		camRot;
	float		animRot;
	float		animDir;

	Game();
	~Game();
	void	loop(void);
	void	update(void);
	void	initRender3D(Shaders *shader);
	void	initRender2D(Shaders *shader);
	void	render(void);
	void	render2D(void);
};

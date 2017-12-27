#include "game.hpp"

Game::Game()
{
	_shaders = nullptr;
	_orthoShaders = nullptr;
}

Game::~Game()
{
	if (_shaders != nullptr)
	{
		delete _shaders;
		_shaders = nullptr;
	}
	if (_orthoShaders != nullptr)
	{
		delete _orthoShaders;
		_orthoShaders = nullptr;
	}
}

void	Game::init(void)
{
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glDepthRangef(0.0,1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glEnable(GL_BLEND);
	glfwSwapInterval(0);

	camPos = glm::vec3(0.0, 0.0, 0.0);
	camRot = 0.0f;

	animRot = -45.0f;
	animDir = 1.0f;

	_shaders = new Shaders();
	if (!_shaders->load(FRAGMENT_SHADER, Shaders::GetFileContent("shaders/basic.frag").c_str()))
		throw std::logic_error("Erreur chargement basic.frag");
	if (!_shaders->load(VERTEX_SHADER, Shaders::GetFileContent("shaders/basic.vert").c_str()))
		throw std::logic_error("Erreur chargement basic.vert");
	if (!_shaders->build())
		throw std::logic_error("Erreur compilation shader(basic)");
	/* ***************************************************
	** Shader Order
	** VERTEX_SHADER -> GEOMETRY_SHADER -> FRAGMENT_SHADER
	** *************************************************** */
	_orthoShaders = new Shaders(3);
	if (!_orthoShaders->load(FRAGMENT_SHADER, Shaders::GetFileContent("shaders/ortho.frag").c_str()))
		throw std::logic_error("Erreur chargement ortho.frag");
	if (!_orthoShaders->load(VERTEX_SHADER, Shaders::GetFileContent("shaders/ortho.vert").c_str()))
		throw std::logic_error("Erreur chargement ortho.vert");
	if (!_orthoShaders->load(GEOMETRY_SHADER, Shaders::GetFileContent("shaders/ortho.geom").c_str()))
		throw std::logic_error("Erreur chargement ortho.geom");
	if (!_orthoShaders->build())
		throw std::logic_error("Erreur compilation shader(ortho)");

	load();
}

void	Game::loop(void)
{
	init();
	double timeLast = glfwGetTime();
	double timeSecLast = glfwGetTime();
	double timeAccumulator = 0.0;
	const double tickPerSec = 1.0 / 60.0;
	int tick = 0;
	int frame = 0;
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS
		&& !glfwWindowShouldClose(window))
	{
		/* ******************** */
		/* * UPDATE           * */
		/* ******************** */
		double now = glfwGetTime();
		timeAccumulator += (now - timeLast);
		timeLast = now;
		while (timeAccumulator >= tickPerSec)
		{
			timeAccumulator -= tickPerSec;
			update();
			tick++;
		}
		/* ******************** */
		/* * RENDU            * */
		/* ******************** */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		render();
		render2D();
	    glfwSwapBuffers(window);
	    glfwPollEvents();
		frame++;
		if (now - timeSecLast >= 1.0)
		{
			timeSecLast = now;
			std::cout << "UPS: " << tick << ", FPS: " << frame << std::endl;
			_fps = frame;
			_ups = tick;
			tick = 0;
			frame = 0;
		}
	}
	Bones::UnLoadRenderer();
}


void	Game::update(void)
{
	/*
		*** VIEW ***
		UP			 0.0 -1.0  0.0
		RIGHT		-1.0  0.0  0.0
		FORWARD		 0.0  0.0  1.0
	*/

	animRot += animDir;
	if (animRot >= 45.0f)
	{
		animRot = 45.0f;
		animDir = -1.0f;
	}
	if (animRot <= -45.0f)
	{
		animRot = -45.0f;
		animDir = 1.0f;
	}


	float rotSpeed = (90.0 / 60.0);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		camRot -= rotSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		camRot += rotSpeed;
	}
	glm::mat4 camRotMat = glm::eulerAngleY(glm::radians(camRot));
	glm::vec3 camDir = glm::vec3(0.0);
	float speed = (2.0 / 60.0);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		speed *= 2.0;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		// camPos += glm::vec3(0.0, 0.0, 1.0) * speed;
		camDir = glm::vec3(0.0, 0.0, 1.0);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		// camPos += glm::vec3(0.0, 0.0, -1.0) * speed;
		camDir = glm::vec3(0.0, 0.0, -1.0);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		// camPos += glm::vec3(1.0, 0.0, 0.0) * speed;
		camDir = glm::vec3(1.0, 0.0, 0.0);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		// camPos += glm::vec3(-1.0, 0.0, 0.0) * speed;
		camDir = glm::vec3(-1.0, 0.0, 0.0);
	}
	glm::vec4 transform = glm::vec4(camDir, 1.0) * camRotMat;
	camPos += glm::vec3(transform[0], transform[1], transform[2]) * speed;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		camPos += glm::vec3(0.0, -1.0, 0.0) * speed ;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		camPos += glm::vec3(0.0, 1.0, 0.0) * speed ;
	}
	_bones->test();
}

void	Game::initRender3D(Shaders *shader)
{
	glUseProgram(0);
	glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	shader->bind();
}

void	Game::initRender2D(Shaders *shader)
{
	glUseProgram(0);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glCullFace(GL_FRONT_AND_BACK);
	shader->bind();
}

void	Game::render(void)
{
	initRender3D(_shaders);
	_shaders->uniformMat4((GLchar *)"projection", (GLfloat *)glm::value_ptr(projection));
	glm::mat4 camPosMat = glm::translate(glm::mat4(1.0f), camPos);
	camPosMat = glm::eulerAngleY(glm::radians(camRot)) * camPosMat;
	_shaders->uniformMat4((GLchar *)"view", (GLfloat *)glm::value_ptr(camPosMat));
	_bones->render(_shaders);
}

void	Game::render2D(void)
{
	initRender2D(_orthoShaders);
	_mesh->render(GL_POINTS);
}

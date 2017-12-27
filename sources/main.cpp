#include "game.hpp"

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Game	*game;

	game = (Game *)glfwGetWindowUserPointer(window);
	if (!game)
		return ;
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_F)
		{
			GLint polygonMode;
			glGetIntegerv(GL_POLYGON_MODE, &polygonMode);
			if (polygonMode == GL_LINE)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glEnable(GL_CULL_FACE);
				glCullFace(GL_FRONT);
			}
			else if (polygonMode == GL_FILL)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDisable(GL_CULL_FACE);
			}
		}
	}
}

static void win_resize_callback(GLFWwindow *window, int width, int height)
{
	Game	*game;

	game = (Game *)glfwGetWindowUserPointer(window);
	if (!game)
		return ;
	glViewport(0, 0, width, height);
	game->projection = glm::perspective(glm::radians(70.0f), (float)width / (float)height, 0.001f, 100.0f);
}

int		main(void)
{
	Game game;

	if (!glfwInit())
		std::cout << "Failed to initialize GLFW\n";
	else
	{
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_DEPTH_BITS, 32);
		game.window = glfwCreateWindow(1280, 720, "GL Test", NULL, NULL);
		if (game.window == NULL)
			std::cout << "Failed to open GLFW window. If you have an Intel GPU, they are not 4.1 compatible.\n";
		else
		{
			game.projection = glm::perspective(glm::radians(70.0f), 1280.0f / 720.0f, 0.001f, 100.0f);
			glfwMakeContextCurrent(game.window);
			glfwSetWindowUserPointer(game.window, &game);
			glfwSetKeyCallback(game.window, key_callback);
			glfwSetFramebufferSizeCallback(game.window, win_resize_callback);
			glfwSwapInterval(0);
			glewExperimental=true;
			if (glewInit() != GLEW_OK)
				std::cout << "Failed to initialize GLEW\n";
			else
				game.loop();
		}
		glfwTerminate();
	}
	return (0);
}

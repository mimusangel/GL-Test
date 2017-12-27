#include "game.hpp"


void	Game::load(void)
{
	_mesh = new Mesh(2);
	_mesh->begin();
	float	vertexPointPos[] = {
		640.0, 320.0,
	};
	float	vertexPointColor[] = {
		1.0f, 0.0f, 0.0f,
	};
	_mesh->add(0, GL_FLOAT, 2, (void *)vertexPointPos, 1);
	_mesh->add(1, GL_FLOAT, 3, (void *)vertexPointColor, 1);
	_mesh->end();
	/*
		UP			 0.0  1.0  0.0
		RIGHT		 1.0  0.0  0.0
		FORWARD		 0.0  0.0 -1.0
	*/
	Bones::LoadRenderer();
	_bones = new Bones(glm::vec3(0.0, 0.0, -5.0), glm::vec3(0.0, 180.0, 0.0));
	_bones->setController(true);

	Bones *child = new Bones(glm::vec3(0.0, 0.0, 0.0), glm::vec3(90.0, 0.0, 0.0), 1.5);
	_bones->addChild(child);

	Bones *childH = new Bones(glm::vec3(0.0), glm::vec3(-11.25, 0.0, 0.0), 0.75);
	child->addChild(childH);

	Bones *child1 = new Bones(glm::vec3(0.0), glm::vec3(11.25, 45.0, 0.0), 0.5);
	child->addChild(child1);
	Bones *child2 = new Bones(glm::vec3(0.0), glm::vec3(11.25, 67.5, 11.25), 0.5);
	child1->addChild(child2);
	Bones *child3 = new Bones(glm::vec3(0.0), glm::vec3(0.0, 67.5, 0.0));
	child2->addChild(child3);
	Bones *child4 = new Bones(glm::vec3(0.0), glm::vec3(-45.0, 0.0, 0.0));
	child3->addChild(child4);

	Bones *child11 = new Bones(glm::vec3(0.0), glm::vec3(11.25, -45.0, 0.0), 0.5);
	child->addChild(child11);
	Bones *child12 = new Bones(glm::vec3(0.0), glm::vec3(11.25, -67.5, -11.25), 0.5);
	child11->addChild(child12);
	Bones *child13 = new Bones(glm::vec3(0.0), glm::vec3(0.0, -67.5, 0.0));
	child12->addChild(child13);
	Bones *child14 = new Bones(glm::vec3(0.0), glm::vec3(-45.0, 0.0, 0.0));
	child13->addChild(child14);

	Bones *child21 = new Bones(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, -90.0, 0.0), 0.5);
	_bones->addChild(child21);
	Bones *child22 = new Bones(glm::vec3(0.0, 0.0, 0.0), glm::vec3(-78.75, 22.5, 0.0), 1.2);
	child21->addChild(child22);
	Bones *child23 = new Bones(glm::vec3(0.0, 0.0, 0.0), glm::vec3(-11.25, -22.5, 0.0), 1.1);
	child22->addChild(child23);

	Bones *child31 = new Bones(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 90.0, 0.0), 0.5);
	_bones->addChild(child31);
	Bones *child32 = new Bones(glm::vec3(0.0, 0.0, 0.0), glm::vec3(-78.75, -22.5, 0.0), 1.2);
	child31->addChild(child32);
	Bones *child33 = new Bones(glm::vec3(0.0, 0.0, 0.0), glm::vec3(-11.25, 22.5, 0.0), 1.1);
	child32->addChild(child33);
}

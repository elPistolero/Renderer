/*
 * Renderer.h
 *
 *  Created on: Dec 30, 2011
 *      Author: isaak
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include "SceneGraph/SceneGraph.h"
#include "SceneGraph/glm/glm/glm.hpp"
#include "SceneGraph/glm/glm/gtc/matrix_transform.hpp"
#include "SceneGraph/glm/glm/gtc/type_ptr.hpp"
#include "ShaderHelper/ShaderHelper.hpp"
#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"

const int WIDTH = 800;
const int HEIGHT = 600;
const int BITSPERPIXEL = 32;

class Renderer {
public:
	Renderer();
	virtual ~Renderer();
	bool init();
	void quit();
	void drawScreen();
	void mainLoop();

private:
	SDL_Event m_event;
	glm::mat4 m_projection;

	void handleInput();
	bool initGL();
};

#endif /* RENDERER_H_ */

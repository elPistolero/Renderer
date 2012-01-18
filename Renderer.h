/*
 * Renderer.h
 *
 *  Created on: Dec 30, 2011
 *      Author: isaak
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include "GL/glew.h"
#include "SceneGraph/SceneGraph.h"
#include "SceneGraph/glm/glm/glm.hpp"
#include "SceneGraph/glm/glm/gtc/matrix_transform.hpp"
#include "SceneGraph/glm/glm/gtc/type_ptr.hpp"
#include "ShaderHelper/ShaderHelper.hpp"
#include "SDL/SDL.h"

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

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
	glm::vec2 m_oldMousePos;

	void handleInput();
	void initSquare(GLint vertexLoc);
	void initGrid(GLint vertexLoc);
	bool initGL();
};

#endif /* RENDERER_H_ */

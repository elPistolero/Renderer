/*
 * Renderer.h
 *
 *  Created on: Dec 30, 2011
 *      Author: isaak
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include "GL/glew.h"
#include "ProjectWideHeaders.h"
#include "SceneGraph/SceneGraph.h"
#include "GLSLProgram/GLSLProgram.hpp"
#include "Importer/OBJImporter.h"
#include "SDL/SDL.h"

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

const int WIDTH = 800;
const int HEIGHT = 600;
const int BITSPERPIXEL = 32;

using namespace Scene;

class Renderer {
public:
	Renderer();
	virtual ~Renderer();
	bool init();
	void quit();
	void drawScreen();
	void mainLoop();

private:
	SDL_Event mEvent;
	glm::mat4 mProjection;
	glm::vec2 mOldMousePos;
	SceneGraph* mGraph;
	SceneCamera* mCamera;

	void handleInput();
	void initVAO(GLint vertexLoc, GLint vertexNormalLoc, GLint texCoordLoc);
	void initGrid(GLint vertexLoc);
	bool initGL();
};

#endif /* RENDERER_H_ */

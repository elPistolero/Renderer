/*
 * Renderer.cpp
 *
 *  Created on: Dec 30, 2011
 *      Author: isaak
 */

#include "Renderer.h"

Renderer::Renderer() : m_event(), m_projection(1.0) {
}

Renderer::~Renderer() {
}

bool Renderer::init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return false;

	if (!SDL_SetVideoMode(WIDTH, HEIGHT, BITSPERPIXEL, SDL_OPENGL))
		return false;

	if (!initGL())
		return false;

	return true;
}

bool Renderer::initGL() {
	m_projection = glm::perspective(90.0f, (float)WIDTH/(float)HEIGHT, 1.0f, 1000.0f);

	glClearColor(0.3, 0.3, 0.3, 1);

	return true;
}

void Renderer::quit() {
	SDL_Quit();
}

void Renderer::drawScreen() {
	SDL_GL_SwapBuffers();
}

void Renderer::handleInput() {
	while (SDL_PollEvent(&m_event)) {
		if (m_event.type == SDL_QUIT)
			quit();
	}
}

void Renderer::mainLoop() {
	while(true) {
		handleInput();
		drawScreen();
	}
}


int main() {
	Renderer renderer;
	renderer.init();
	renderer.mainLoop();

	return 0;
}

/*
 * Renderer.cpp
 *
 *  Created on: Dec 30, 2011
 *      Author: isaak
 */

#include "Renderer.h"

Renderer::Renderer() : m_event(), m_projection(1.0), m_oldMousePos(WIDTH/2,HEIGHT/2) {
}

Renderer::~Renderer() {
}

bool Renderer::init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return false;

	if (!SDL_SetVideoMode(WIDTH, HEIGHT, BITSPERPIXEL, SDL_OPENGL))
		return false;

	SDL_ShowCursor(1);
	SDL_WM_GrabInput(SDL_GRAB_ON);
	SDL_WarpMouse(WIDTH/2, HEIGHT/2);

	GLenum error = glewInit();
	if (GLEW_OK != error) {
		std::cout << glewGetErrorString(error) << std::endl;
		return false;
	}

	if (!initGL())
		return false;

	return true;
}

GLint simpleShader = 0;
SceneNodeVAO node;
SceneNodeVAO node2;
SceneNodeVAO child;
SceneNodeVAO grandchild;
SceneCamera camera;

void Renderer::initSquare(GLint vertexLoc) {
	GLuint squareVAO = 0;
	GLuint squareIBO = 0;
	GLuint squareVBO = 0;

	GLfloat vertexBuffer[] = { -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, -1.0f, -1.0f, 0.0f };
	// 0 - 1
	// |    |
	// 3 - 2
	GLuint indexBuffer[] = { 1, 0, 2, 3 };


	glGenVertexArrays(1, &squareVAO);
	glBindVertexArray(squareVAO);

	glGenBuffers(1, &squareVBO);
	glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * 4, vertexBuffer,
			GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexLoc);
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(0));

	glGenBuffers(1, &squareIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 4, indexBuffer,
			GL_STATIC_DRAW);

	glBindVertexArray(0);

	node.setVAOPointer(squareVAO);
	node2.setVAOPointer(squareVAO);
	child.setVAOPointer(squareVAO);
	grandchild.setVAOPointer(squareVAO);
}

bool Renderer::initGL() {
	ShaderHelper shader;
	simpleShader = shader.compileAndLinkShaders("Shader/Simple.vert", "Shader/Simple.frag");
	GLint vertexLoc = glGetAttribLocation(simpleShader, "vertex");

	initSquare(vertexLoc);

	m_projection = glm::perspective(60.0f, (float)WIDTH/(float)HEIGHT, 1.0f, 1000.0f);

	camera.attachNode(node);
	camera.attachNode(node2);
	node.attachNode(child);
	child.attachNode(grandchild);

	glClearColor(0.75, 0.75, 0.75, 1);

	return true;
}

void Renderer::quit() {
	SDL_Quit();
}

float angle = 0.0f;
void Renderer::drawScreen() {
	glClear(GL_COLOR_BUFFER_BIT);

	angle += 0.01;

	camera.updateCamera();

	node.setTransformation(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)));
	node2.setTransformation(glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 4.0f, 0.0f)));

	child.setTransformation(glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.0f, 0.0f))
						* glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f))
						* glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f)));

	grandchild.setTransformation(glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.0f, 0.0f))
						* glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f))
						* glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f)));

	camera.update();

	glUseProgram(simpleShader);
	GLint mvpLoc = glGetUniformLocation(simpleShader, "mvp");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(m_projection * node.getTransformation()));
	glBindVertexArray(node.getVAOPointer());
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(m_projection * node2.getTransformation()));
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(m_projection * child.getTransformation()));
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(m_projection * grandchild.getTransformation()));
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
	glBindVertexArray(0);
	glUseProgram(0);

	SDL_GL_SwapBuffers();
}

void Renderer::handleInput() {
	int xPos = 0;
	int yPos = 0;
	while (SDL_PollEvent(&m_event)) {
		switch (m_event.type) {

		case SDL_QUIT:
			quit();
			break;

		case SDL_KEYDOWN:
			switch (m_event.key.keysym.sym) {
			case SDLK_a:
				camera.setXVelocity(-0.01f);
				break;
			case SDLK_d:
				camera.setXVelocity(0.01f);
				break;
			case SDLK_s:
				camera.setZVelocity(-0.01f);
				break;
			case SDLK_w:
				camera.setZVelocity(0.01f);
				break;
			case SDLK_ESCAPE:
				quit();
				break;
			default:
				break;
			}
			break;

			case SDL_KEYUP:
				switch(m_event.key.keysym.sym) {
				case SDLK_a:
					if (camera.movingLeft())
						camera.setXVelocity(0.0f);
					break;
				case SDLK_d:
					if (camera.movingRight())
						camera.setXVelocity(0.0f);
					break;
				case SDLK_w:
					if (camera.movingForward())
						camera.setZVelocity(0.0f);
					break;
				case SDLK_s:
					if (camera.movingBackward())
						camera.setZVelocity(0.0f);
					break;
				default:
					break;
				}
				break;

			case SDL_MOUSEMOTION:
				// get current mouse cursor position
				SDL_GetMouseState(&xPos, &yPos);

				// rotate left
				if (xPos < m_oldMousePos.x) {
					camera.incXRotation(0.5f);
				}
				// rotate right
				if (xPos > m_oldMousePos.x) {
					camera.incXRotation(-0.5f);
				}
				// rotate up
				if (yPos < m_oldMousePos.y) {
					camera.incYRotation(0.5f);
				}
				// rotate down
				if (yPos > m_oldMousePos.y) {
					camera.incYRotation(-0.5f);
				}

				// save mouse cursor position
				m_oldMousePos.x = xPos;
				m_oldMousePos.y = yPos;
				break;

			default:
				break;
		}
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

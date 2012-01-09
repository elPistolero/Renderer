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

bool Renderer::initGL() {
	GLuint squareVAO = 0;
	GLuint squareIBO = 0;
	GLuint squareVBO = 0;

	m_projection = glm::perspective(90.0f, (float)WIDTH/(float)HEIGHT, 1.0f, 1000.0f);

	ShaderHelper shader;
	simpleShader = shader.compileAndLinkShaders("Shader/Simple.vert", "Shader/Simple.frag");

	GLfloat vertexBuffer[] = { -1, 1, 1, 1, 1, -1, -1, -1 };
	// 0 - 1
	// |    |
	// 3 - 2
	GLuint indexBuffer[] = { 1, 0, 2, 3 };

	GLint vertexLoc = glGetAttribLocation(simpleShader, "vertex");

	glGenVertexArrays(1, &squareVAO);
	glBindVertexArray(squareVAO);

	glGenBuffers(1, &squareVBO);
	glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * 4, vertexBuffer,
			GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexLoc);
	glVertexAttribPointer(vertexLoc, 2, GL_FLOAT, GL_FALSE, 0,
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

	camera.attachNode(node);
	camera.attachNode(node2);
	node.attachNode(child);
	child.attachNode(grandchild);

	camera.setTransformation(glm::lookAt(glm::vec3(camera.m_posX, camera.m_posY, camera.m_posZ),
							 	 	 	 glm::vec3(camera.m_posX, camera.m_posY, -1.0f),
							 	 	 	 glm::vec3(0.0f, 1.0f, 0.0f)));

	glClearColor(0.75, 0.75, 0.75, 1);

	return true;
}

void Renderer::quit() {
	SDL_Quit();
}

float angle = 0.0f;
void Renderer::drawScreen() {
	glClear(GL_COLOR_BUFFER_BIT);

	// update camera position
	camera.m_posX += camera.m_xVel;
	camera.m_posY += camera.m_yVel;
	camera.m_posZ += camera.m_zVel;

	angle += 0.01;

	camera.setTransformation(glm::lookAt(glm::vec3(camera.m_posX, camera.m_posY, camera.m_posZ),
							 	 	 	 glm::vec3(camera.m_posX, camera.m_posY, -1.0f),
							 	 	 	 glm::vec3(0.0f, 1.0f, 0.0f)));

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
	while (SDL_PollEvent(&m_event)) {
		switch (m_event.type) {

		case SDL_QUIT:
			quit();
			break;

		case SDL_KEYDOWN:
			switch (m_event.key.keysym.sym) {
			case SDLK_a:
				camera.m_xVel = -0.01f;
				break;
			case SDLK_d:
				camera.m_xVel = 0.01f;
				break;
			case SDLK_s:
				camera.m_zVel = 0.01f;
				break;
			case SDLK_w:
				camera.m_zVel = -0.01f;
				break;
			case SDLK_q:
				camera.m_yVel = 0.01f;
				break;
			case SDLK_e:
				camera.m_yVel = -0.01f;
				break;
			default:
				break;
			}
			break;

			case SDL_KEYUP:
				switch(m_event.key.keysym.sym) {
				case SDLK_a:
					if (camera.m_xVel < 0)
						camera.m_xVel = 0.0f;
					break;
				case SDLK_d:
					if (camera.m_xVel > 0)
						camera.m_xVel = 0.0f;
					break;
				case SDLK_w:
					if (camera.m_zVel < 0)
						camera.m_zVel = 0.0f;
					break;
				case SDLK_s:
					if (camera.m_zVel > 0)
						camera.m_zVel = 0.0f;
					break;
				case SDLK_q:
					if (camera.m_yVel > 0)
						camera.m_yVel = 0.0f;
					break;
				case SDLK_e:
					if (camera.m_yVel < 0)
						camera.m_yVel = 0.0f;
					break;
				default:
					break;
				}
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

/*
 * Renderer.cpp
 *
 *  Created on: Dec 30, 2011
 *      Author: isaak
 */

#include "Renderer.h"

Renderer::Renderer() : m_event(), m_projection(1.0), m_oldMousePos(WIDTH/2,HEIGHT/2), m_graph(0), m_camera(0) {
	m_graph = new SceneGraph();
	m_camera = m_graph->getRoot();
}

Renderer::~Renderer() {
	delete m_graph;
}

bool Renderer::init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return false;

	if (!SDL_SetVideoMode(WIDTH, HEIGHT, BITSPERPIXEL, SDL_OPENGL))
		return false;

	SDL_ShowCursor(1);
	//SDL_WM_GrabInput(SDL_GRAB_ON);
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

void Renderer::initSquare(GLint vertexLoc) {
	OBJImporter::OBJImporter objReader;
	std::vector<GLfloat> vbo;
	std::vector<GLuint> ibo;
	objReader.readFile("./Resources/ball.obj", vbo, ibo);

	std::vector<GLfloat>::iterator vIt;
	for (vIt = vbo.begin(); vIt != vbo.end(); ++vIt) {
		std::cout << "vertex: " << *vIt;
		++vIt;
		std::cout << ", " << *vIt;
		++vIt;
		std::cout << ", " << *vIt << std::endl;
	}
	std::cout << "vbo size: " << sizeof(GLfloat)*vbo.size() << std::endl;

	std::vector<GLuint>::iterator iIt;
	for (iIt = ibo.begin(); iIt != ibo.end(); ++iIt) {
		std::cout << "index: " << *iIt;
		++iIt;
		std::cout << ", " << *iIt;
		++iIt;
		std::cout << ", " << *iIt << std::endl;
	}
	std::cout << "ibo size: " << sizeof(GLuint)*ibo.size() << std::endl;

	GLuint squareVAO = 0;
	GLuint squareIBO = 0;
	GLuint squareVBO = 0;

	glGenVertexArrays(1, &squareVAO);
	glBindVertexArray(squareVAO);

	glGenBuffers(1, &squareVBO);
	glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vbo.size(), &vbo[0],
			GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexLoc);
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(0));

	glGenBuffers(1, &squareIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * ibo.size(), &ibo[0],
			GL_STATIC_DRAW);

	glBindVertexArray(0);

	std::list<SceneNode*> children = m_camera->getChildren();
	std::list<SceneNode*>::iterator it;
	for (it = children.begin(); it != children.end(); ++it) {
		SceneNodeVAO* vao = dynamic_cast<SceneNodeVAO*>(*it);
		if (vao) {
			vao->setVAOPointer(squareVAO);
			vao->setNumberOfFaces(ibo.size());
		}
	}
}

bool Renderer::initGL() {
	ShaderHelper shader;
	simpleShader = shader.compileAndLinkShaders("Shader/Simple.vert", "Shader/Simple.frag");
	GLint vertexLoc = glGetAttribLocation(simpleShader, "vertex");

	SceneNodeVAO* node = new SceneNodeVAO();
	m_camera->attachNode(*node);

	initSquare(vertexLoc);

	m_projection = glm::perspective(60.0f, (float)WIDTH/(float)HEIGHT, 1.0f, 1000.0f);

	glClearColor(0.75, 0.75, 0.75, 1);

	return true;
}

void Renderer::quit() {
	SDL_Quit();
}

void Renderer::drawScreen() {
	glClear(GL_COLOR_BUFFER_BIT);

	m_camera->update();

	glUseProgram(simpleShader);
	GLint mvpLoc = glGetUniformLocation(simpleShader, "mvp");
	std::list<SceneNode*> children = m_camera->getChildren();
	std::list<SceneNode*>::iterator it;
	for (it = children.begin(); it != children.end(); ++it) {
		SceneNodeVAO* vao = dynamic_cast<SceneNodeVAO*>(*it);
		if (vao) {
			glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(m_projection * vao->getGlobalTransformation()));
			glBindVertexArray(vao->getVAOPointer());
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawElements(GL_TRIANGLES, vao->getNumberOfFaces()*3, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
		}
	}
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
				m_camera->setXVelocity(-0.01f);
				break;
			case SDLK_d:
				m_camera->setXVelocity(0.01f);
				break;
			case SDLK_s:
				m_camera->setZVelocity(-0.01f);
				break;
			case SDLK_w:
				m_camera->setZVelocity(0.01f);
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
					if (m_camera->movingLeft())
						m_camera->setXVelocity(0.0f);
					break;
				case SDLK_d:
					if (m_camera->movingRight())
						m_camera->setXVelocity(0.0f);
					break;
				case SDLK_w:
					if (m_camera->movingForward())
						m_camera->setZVelocity(0.0f);
					break;
				case SDLK_s:
					if (m_camera->movingBackward())
						m_camera->setZVelocity(0.0f);
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
					m_camera->incXRotation(0.5f);
				}
				// rotate right
				if (xPos > m_oldMousePos.x) {
					m_camera->incXRotation(-0.5f);
				}
				// rotate up
				if (yPos < m_oldMousePos.y) {
					m_camera->incYRotation(0.5f);
				}
				// rotate down
				if (yPos > m_oldMousePos.y) {
					m_camera->incYRotation(-0.5f);
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

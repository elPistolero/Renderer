/*
 * Renderer.cpp
 *
 *  Created on: Dec 30, 2011
 *      Author: isaak
 */

#include "Renderer.h"

using namespace Scene;

Renderer::Renderer() : mEvent(), mProjection(1.0), mOldMousePos(WIDTH/2,HEIGHT/2), mGraph(0), mCamera(0) {
	mGraph = new SceneGraph();
	mCamera = mGraph->getRoot();
}

Renderer::~Renderer() {
	delete mGraph;
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

void Renderer::initVAO(GLint vertexLoc, GLint vertexNormalLoc, GLint texCoordLoc) {
	Importer::OBJImporter objReader;
	std::vector<GLfloat> vbo;
	std::vector<GLfloat> vnbo;
	std::vector<GLfloat> vtbo;
	std::vector<GLuint> ibo;
	objReader.readFile("./Resources/test.obj", vbo, vnbo, vtbo, ibo);

	std::vector<GLfloat>::iterator vIt;
	for (vIt = vbo.begin(); vIt != vbo.end(); ++vIt) {
		std::cout << "vertex: " << *vIt;
		++vIt;
		std::cout << ", " << *vIt;
		++vIt;
		std::cout << ", " << *vIt << std::endl;
	}
	std::cout << "vbo size: " << sizeof(GLfloat)*vbo.size() << std::endl;

	for (vIt = vnbo.begin(); vIt != vnbo.end(); ++vIt) {
		std::cout << "vertex normal: " << *vIt;
		++vIt;
		std::cout << ", " << *vIt;
		++vIt;
		std::cout << ", " << *vIt << std::endl;
	}
	std::cout << "vnbo size: " << sizeof(GLfloat)*vnbo.size() << std::endl;

	for (vIt = vtbo.begin(); vIt != vtbo.end(); ++vIt) {
		std::cout << "texture coord: " << *vIt;
		++vIt;
		std::cout << ", " << *vIt << std::endl;
	}
	std::cout << "vtbo size: " << sizeof(GLfloat)*vtbo.size() << std::endl;

	std::vector<GLuint>::iterator iIt;
	for (iIt = ibo.begin(); iIt != ibo.end(); ++iIt) {
		std::cout << "index: " << *iIt;
		++iIt;
		std::cout << ", " << *iIt;
		++iIt;
		std::cout << ", " << *iIt << std::endl;
	}
	std::cout << "ibo size: " << sizeof(GLuint)*ibo.size() << std::endl;

	GLuint hVAO = 0;
	GLuint hIBO = 0;
	GLuint hVBO[3];

	glGenVertexArrays(1, &hVAO);
	glBindVertexArray(hVAO);

	glGenBuffers(3, &hVBO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, hVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vbo.size(), &vbo[0],
			GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexLoc);
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, hVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vnbo.size(), &vnbo[0],
			GL_STATIC_DRAW);
	glEnableVertexAttribArray(vertexNormalLoc);
	glVertexAttribPointer(vertexNormalLoc, 3, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, hVBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vtbo.size(), &vtbo[0],
			GL_STATIC_DRAW);
	glEnableVertexAttribArray(texCoordLoc);
	glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(0));

	glGenBuffers(1, &hIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, hIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * ibo.size(), &ibo[0],
			GL_STATIC_DRAW);

	glBindVertexArray(0);

	std::list<SceneNode*> children = mCamera->getChildren();
	std::list<SceneNode*>::iterator it;
	for (it = children.begin(); it != children.end(); ++it) {
		SceneNodeTriangleMesh* triMesh = dynamic_cast<SceneNodeTriangleMesh*>(*it);
		if (triMesh) {
			triMesh->setVAOPointer(hVAO);
			triMesh->setNumberOfFaces(ibo.size());
		}
	}
}

bool Renderer::initGL() {
	Shader::ShaderHelper shader;
	simpleShader = shader.compileAndLinkShaders("Shader/Simple.vert", "Shader/Simple.frag");
	GLint vertexLoc = glGetAttribLocation(simpleShader, "vertex");
	GLint vertexNormalLoc = glGetAttribLocation(simpleShader, "vertexNormal");
	GLint texCoordLoc = glGetAttribLocation(simpleShader, "texCoord");

	SceneNodeTriangleMesh* node = new SceneNodeTriangleMesh();
	mCamera->attachNode(*node);

	initVAO(vertexLoc, vertexNormalLoc, texCoordLoc);

	mProjection = glm::perspective(60.0f, (float)WIDTH/(float)HEIGHT, 1.0f, 1000.0f);

	glClearColor(0.75, 0.75, 0.75, 1);
	glEnable(GL_DEPTH_TEST);

	return true;
}

void Renderer::quit() {
	SDL_Quit();
}

void Renderer::drawScreen() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mCamera->update();

	glUseProgram(simpleShader);
	GLint modelviewLoc = glGetUniformLocation(simpleShader, "modelview");
	GLint projectionLoc = glGetUniformLocation(simpleShader, "projection");
	GLint mvpLoc = glGetUniformLocation(simpleShader, "mvp");
	GLint normalMatrixLoc = glGetUniformLocation(simpleShader, "normalMatrix");
	GLint lightPosLoc = glGetUniformLocation(simpleShader, "lightPosition");

	std::list<SceneNode*> children = mCamera->getChildren();
	std::list<SceneNode*>::iterator it;
	for (it = children.begin(); it != children.end(); ++it) {
		SceneNodeTriangleMesh* triMesh = dynamic_cast<SceneNodeTriangleMesh*>(*it);
		if (triMesh) {
			glUniformMatrix4fv(modelviewLoc, 1, GL_FALSE, glm::value_ptr(triMesh->getGlobalTransformation()));
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(mProjection));
			glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mProjection * triMesh->getGlobalTransformation()));
			glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(triMesh->getNormalMatrix()));
			glm::vec4 worldLight = mCamera->getGlobalTransformation() * glm::vec4(5.0f, 5.0f, 2.0f, 1.0f);
			glUniform4f(lightPosLoc, worldLight.x, worldLight.y, worldLight.z, worldLight.w);
			glBindVertexArray(triMesh->getVAOPointer());
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawElements(GL_TRIANGLES, triMesh->getNumberOfFaces()*3, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
		}
	}
	glBindVertexArray(0);
	glUseProgram(0);

	SDL_GL_SwapBuffers();
}

void Renderer::handleInput() {
	int xPos = 0;
	int yPos = 0;
	while (SDL_PollEvent(&mEvent)) {
		switch (mEvent.type) {

		case SDL_QUIT:
			quit();
			break;

		case SDL_KEYDOWN:
			switch (mEvent.key.keysym.sym) {
			case SDLK_a:
				mCamera->setXVelocity(-0.01f);
				break;
			case SDLK_d:
				mCamera->setXVelocity(0.01f);
				break;
			case SDLK_s:
				mCamera->setZVelocity(-0.01f);
				break;
			case SDLK_w:
				mCamera->setZVelocity(0.01f);
				break;
			case SDLK_ESCAPE:
				quit();
				break;
			default:
				break;
			}
			break;

			case SDL_KEYUP:
				switch(mEvent.key.keysym.sym) {
				case SDLK_a:
					if (mCamera->movingLeft())
						mCamera->setXVelocity(0.0f);
					break;
				case SDLK_d:
					if (mCamera->movingRight())
						mCamera->setXVelocity(0.0f);
					break;
				case SDLK_w:
					if (mCamera->movingForward())
						mCamera->setZVelocity(0.0f);
					break;
				case SDLK_s:
					if (mCamera->movingBackward())
						mCamera->setZVelocity(0.0f);
					break;
				default:
					break;
				}
				break;

			case SDL_MOUSEMOTION:
				// get current mouse cursor position
				SDL_GetMouseState(&xPos, &yPos);

				// rotate left
				if (xPos < mOldMousePos.x) {
					mCamera->incXRotation(0.5f);
				}
				// rotate right
				if (xPos > mOldMousePos.x) {
					mCamera->incXRotation(-0.5f);
				}
				// rotate up
				if (yPos < mOldMousePos.y) {
					mCamera->incYRotation(0.5f);
				}
				// rotate down
				if (yPos > mOldMousePos.y) {
					mCamera->incYRotation(-0.5f);
				}

				// save mouse cursor position
				mOldMousePos.x = xPos;
				mOldMousePos.y = yPos;
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

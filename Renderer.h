/*
 * Renderer.h
 *
 *  Created on: Dec 30, 2011
 *      Author: isaak
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include "GL/glew.h"
#include <QApplication>
#include <QGLWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include "ProjectWideHeaders.h"
#include "SceneGraph/SceneGraph.h"
#include "GLSLProgram/GLSLProgram.hpp"
#include "Importer/OBJImporter.h"

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

const int WIDTH = 800;
const int HEIGHT = 600;
const int BITSPERPIXEL = 32;

using namespace Scene;

class Renderer : public QGLWidget {
Q_OBJECT

public:
Renderer(const QGLFormat& format, QWidget* parent);
~Renderer();

protected:
void initializeGL();
void paintGL();
void resizeGL(int w, int h);
void checkGLErrors(const char* file, int line);
void mousePressEvent(QMouseEvent* event);
void mouseMoveEvent(QMouseEvent* event);
void mouseReleaseEvent(QMouseEvent* event);
void wheelEvent(QWheelEvent* event);
bool mapToSphere(glm::vec2& point, glm::vec3& vec);

private:
glm::mat4 mProjection;
glm::vec2 mOldMousePos;
SceneGraph* mGraph;
SceneCamera* mCamera;
Shader::GLSLProgram* mSimpleShader;
bool mMouseTrackingLeft;
bool mMouseTrackingRight;
int mWidth;
int mHeight;

void handleInput();
void initVAO(GLint vertexLoc, GLint vertexNormalLoc, GLint texCoordLoc);
void initGrid(GLint vertexLoc);
bool initGL();
};

#endif /* RENDERER_H_ */

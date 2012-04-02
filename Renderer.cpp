/*
 * Renderer.cpp
 *
 *  Created on: Dec 30, 2011
 *      Author: isaak
 */

#include "Renderer.h"

using namespace Scene;

Renderer::Renderer(const QGLFormat& format, QWidget* parent) : QGLWidget(format, parent), mProjection(1.0), mOldMousePos(WIDTH/2,HEIGHT/2), mGraph(0), mCamera(0), mSimpleShader(0) {
    mGraph = new SceneGraph();
    mCamera = mGraph->getRoot();
    this->setMinimumSize(800,600);
}

Renderer::~Renderer() {
    delete mGraph;
    delete mSimpleShader;
}

void Renderer::initializeGL() {
    glewExperimental = GL_TRUE;
    GLenum error = glewInit();
    if (GLEW_OK != error) {
        std::cout << glewGetErrorString(error) << std::endl;
    }

    mSimpleShader = new Shader::GLSLProgram();
    mSimpleShader->compileAndLinkShaders("Shader/Simple.vert", "Shader/Simple.frag");
    mSimpleShader->bindAttribLocation(0, "vertex");
    mSimpleShader->bindAttribLocation(1, "vertexNormal");
    mSimpleShader->bindAttribLocation(2, "texCoord");

    SceneNodeTriangleMesh* node = new SceneNodeTriangleMesh();
    mCamera->attachNode(*node);

    initVAO(0, 1, 2);

    mProjection = glm::perspective(60.0f, (float)WIDTH/(float)HEIGHT, 1.0f, 1000.0f);

    glClearColor(0.75, 0.75, 0.75, 1);
    glEnable(GL_DEPTH_TEST);
}

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

void Renderer::resizeGL(int w, int h) {
    mProjection = glm::perspective(60.0f, (float)WIDTH/(float)HEIGHT, 1.0f, 1000.0f);
}

void Renderer::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mCamera->update();

    mSimpleShader->use();

    std::list<SceneNode*> children = mCamera->getChildren();
    std::list<SceneNode*>::iterator it;
    for (it = children.begin(); it != children.end(); ++it) {
        SceneNodeTriangleMesh* triMesh = dynamic_cast<SceneNodeTriangleMesh*>(*it);
        if (triMesh) {
            mSimpleShader->setUniform("modelview", triMesh->getGlobalTransformation());
            //shader.setUniform("projection", mProjection);
            mSimpleShader->setUniform("mvp", mProjection * triMesh->getGlobalTransformation());
            mSimpleShader->setUniform("normalMatrix", triMesh->getNormalMatrix());
            glm::vec4 worldLight = mCamera->getGlobalTransformation() * glm::vec4(5.0f, 5.0f, 2.0f, 1.0f);
            mSimpleShader->setUniform("lightPosition", worldLight);
            glBindVertexArray(triMesh->getVAOPointer());
            glDrawElements(GL_TRIANGLES, triMesh->getNumberOfFaces()*3, GL_UNSIGNED_INT, BUFFER_OFFSET(0));
        }
    }
    glBindVertexArray(0);
    glUseProgram(0);

    glFinish();
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QWidget mainWindow;

    QGLFormat format;
    format.setVersion(4,0);
    format.setProfile(QGLFormat::CoreProfile);

    Renderer renderer(format, &mainWindow);

    mainWindow.show();

    return app.exec();
}

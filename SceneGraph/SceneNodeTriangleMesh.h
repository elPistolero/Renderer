/*
 * SceneNodeVAO.h
 *
 *  Created on: Jan 8, 2012
 *      Author: isaak
 */

#ifndef SCENENODEVAO_H_
#define SCENENODEVAO_H_

#include "SceneNode.h"

namespace Scene {

class SceneNodeTriangleMesh : public SceneNode {
public:
SceneNodeTriangleMesh();
virtual ~SceneNodeTriangleMesh();

GLuint getVAOPointer() {
    return mHVao;
}
void setVAOPointer(GLuint vao) {
    mHVao = vao;
}
int getNumberOfFaces() {
    return mNumFaces;
}
void setNumberOfFaces(int faces) {
    mNumFaces = faces;
}

private:
GLuint mHVao;
int mNumFaces;
};

}

#endif /* SCENENODEVAO_H_ */

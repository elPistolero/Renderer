/*
 * Scenegraph.h
 *
 *  Created on: Dec 25, 2011
 *      Author: isaak
 */

#ifndef SCENEGRAPH_H_
#define SCENEGRAPH_H_

#include "SceneNode.h"
#include "SceneNodeTriangleMesh.h"
#include "SceneCamera.h"
#include "iostream"

namespace Scene {

class SceneGraph {
public:
	SceneGraph();
	virtual ~SceneGraph();
	SceneCamera* getRoot() { return mRoot; }
	void setRoot(SceneCamera* root) { mRoot = root; }

private:
	SceneCamera* mRoot;
};

}

#endif /* SCENEGRAPH_H_ */

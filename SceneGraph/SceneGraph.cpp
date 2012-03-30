/*
 * Scenegraph.cpp
 *
 *  Created on: Dec 25, 2011
 *      Author: isaak
 */

#include "SceneGraph.h"

namespace Scene {

SceneGraph::SceneGraph() : mRoot(0) {
	mRoot = new SceneCamera();
}

SceneGraph::~SceneGraph() {
	delete mRoot;
}

}

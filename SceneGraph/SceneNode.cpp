/*
 * SceneNode.cpp
 *
 *  Created on: Dec 25, 2011
 *      Author: isaak
 */

#include "SceneNode.h"

namespace Scene {

SceneNode::SceneNode()
: mParent(0), mChildren(0), mGlobalTransformation(1.0f), mLocalTransformation(1.0f), mUpdated(false) {
}

SceneNode::~SceneNode() {
	// delete children
	while (!mChildren.empty()) {
		SceneNode* first = mChildren.front();
		mChildren.pop_front();
		delete first;
	}
}

void SceneNode::detachNode(SceneNode& child) {
	std::list<SceneNode*>::iterator it;
	for (it = mChildren.begin(); it != mChildren.end(); ++it) {
		if (&child == *it) {
			mChildren.erase(it);
			break;
		}
	}
	child.mParent = 0;
	child.mGlobalTransformation = glm::mat4(1);
}

void SceneNode::attachNode(SceneNode& child) {
	child.mParent = this;
	mChildren.push_back(&child);
	child.mUpdated = false;
}

void SceneNode::deleteNode(SceneNode& child) {
	std::list<SceneNode*>::iterator it;
	for (it = mChildren.begin(); it != mChildren.end(); ++it) {
		if (&child == *it) {
			delete *it;
			mChildren.erase(it);
			break;
		}
	}
}

std::list<SceneNode*>& SceneNode::getChildren() {
	return mChildren;
}

void SceneNode::setLocalTransformation(const glm::mat4& trans) {
	mLocalTransformation = trans;
	mUpdated = false;
}

glm::mat4& SceneNode::getLocalTransformation() {
	return mLocalTransformation;
}

void SceneNode::setGlobalTransformation(const glm::mat4& trans) {
	mGlobalTransformation = trans;
	setNormalMatrix();
	mUpdated = false;
}

glm::mat4& SceneNode::getGlobalTransformation() {
	return mGlobalTransformation;
}

void SceneNode::setNormalMatrix() {
	mNormalMatrix = glm::inverseTranspose(glm::mat3(mGlobalTransformation));
}

glm::mat3& SceneNode::getNormalMatrix() {
	return mNormalMatrix;
}

void SceneNode::update() {
	// if not up to date update from parent
	if (!mUpdated) {
		// update from parent only if we are not the root
		if (mParent)
			mGlobalTransformation = mParent->getGlobalTransformation() * mLocalTransformation;

		// update possible children
		if (!mChildren.empty()) {
			std::list<SceneNode*>::iterator it;
			for (it = mChildren.begin(); it != mChildren.end(); ++it) {
				if (!mUpdated)
					(*it)->mUpdated = false;
				(*it)->update();
			}
		}
		mUpdated = true;
	// we are up to date
	} else {
		// check possible children
		if (!mChildren.empty()) {
			std::list<SceneNode*>::iterator it;
			for (it = mChildren.begin(); it != mChildren.end(); ++it) {
				(*it)->update();
			}
		} else {
			return;
		}
	}
}

std::ostream& operator<< (std::ostream& out, const SceneNode& node) {
	if (node.mParent)
		out << "parent: 1\n";
	else
		out << "parent: 0\n";
	out << "children: " << node.mChildren.size() << std::endl;
	out << "updated: " << node.mUpdated << std::endl;
	out << "[" << node.mGlobalTransformation[0][0] << ", " << node.mGlobalTransformation[0][1] << ", " << node.mGlobalTransformation[0][2] << ", " << node.mGlobalTransformation[0][3] << "]\n";
	out << "[" << node.mGlobalTransformation[1][0] << ", " << node.mGlobalTransformation[1][1] << ", " << node.mGlobalTransformation[1][2] << ", " << node.mGlobalTransformation[1][3] << "]\n";
	out << "[" << node.mGlobalTransformation[2][0] << ", " << node.mGlobalTransformation[2][1] << ", " << node.mGlobalTransformation[2][2] << ", " << node.mGlobalTransformation[2][3] << "]\n";
	out << "[" << node.mGlobalTransformation[3][0] << ", " << node.mGlobalTransformation[3][1] << ", " << node.mGlobalTransformation[3][2] << ", " << node.mGlobalTransformation[3][3] << "]\n";

	return out;
}

}

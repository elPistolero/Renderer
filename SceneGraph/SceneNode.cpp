/*
 * SceneNode.cpp
 *
 *  Created on: Dec 25, 2011
 *      Author: isaak
 */

#include "SceneNode.h"

namespace Scene {

SceneNode::SceneNode()
    : mParent(0), mChildren(0), mTransformation(1.0f), mUpdated(false), mLocalTransformation(1.0f) {
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
    child.mTransformation = glm::mat4(1);
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

void SceneNode::setTransformation(const glm::mat4& trans) {
    mTransformation = trans;
    setNormalMatrix();
    mUpdated = false;
}

glm::mat4& SceneNode::getTransformation() {
    return mTransformation;
}

void SceneNode::setNormalMatrix() {
    mNormalMatrix = glm::inverseTranspose(glm::mat3(mTransformation));
}

glm::mat3& SceneNode::getNormalMatrix() {
    return mNormalMatrix;
}

void SceneNode::rotate(const glm::vec3& axis, float degrees) {
    mLocalTransformation = glm::rotate(glm::mat4(1.0f), degrees, axis) * mLocalTransformation;
    mUpdated = false;
}

void SceneNode::translate(const glm::vec3& trans) {
    mLocalTransformation = glm::translate(glm::mat4(1.0f), trans) * mLocalTransformation;
    mUpdated = false;
}

void SceneNode::scale(const glm::vec3& scale) {
    mLocalTransformation = glm::scale(glm::mat4(1.0f), scale) * mLocalTransformation;
    mUpdated = false;
}

void SceneNode::update() {
    // if not up to date update from parent
    if (!mUpdated) {
        if (mParent)
            mTransformation = mParent->getTransformation() * mLocalTransformation;

        setNormalMatrix();

        // update possible children
        if (!mChildren.empty()) {
            std::list<SceneNode*>::iterator it;
            for (it = mChildren.begin(); it != mChildren.end(); ++it) {
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
    out << "[" << node.mTransformation[0][0] << ", " << node.mTransformation[0][1] << ", " << node.mTransformation[0][2] << ", " << node.mTransformation[0][3] << "]\n";
    out << "[" << node.mTransformation[1][0] << ", " << node.mTransformation[1][1] << ", " << node.mTransformation[1][2] << ", " << node.mTransformation[1][3] << "]\n";
    out << "[" << node.mTransformation[2][0] << ", " << node.mTransformation[2][1] << ", " << node.mTransformation[2][2] << ", " << node.mTransformation[2][3] << "]\n";
    out << "[" << node.mTransformation[3][0] << ", " << node.mTransformation[3][1] << ", " << node.mTransformation[3][2] << ", " << node.mTransformation[3][3] << "]\n";

    return out;
}

}

/*
 * SceneNode.h
 *
 *  Created on: Dec 25, 2011
 *      Author: isaak
 */

#ifndef SCENENODE_H_
#define SCENENODE_H_

#include <GL/glew.h>
#include <list>
#include "../ProjectWideHeaders.h"

namespace Scene {

class SceneNode {
public:
SceneNode();
virtual ~SceneNode();
SceneNode& getParent();
void attachNode(SceneNode& child);
void detachNode(SceneNode& child);
void deleteNode(SceneNode& child);
std::list<SceneNode*>& getChildren();
void setLocalTransformation(const glm::mat4& trans);
glm::mat4& getLocalTransformation();
void setGlobalTransformation(const glm::mat4& trans);
glm::mat4& getGlobalTransformation();
glm::mat3& getNormalMatrix();
void virtual update();

friend std::ostream& operator<< (std::ostream& out, const SceneNode& node);

protected:
SceneNode* mParent;
std::list<SceneNode*> mChildren;
glm::mat4 mGlobalTransformation;
glm::mat4 mLocalTransformation;
glm::mat3 mNormalMatrix;
bool mUpdated;

private:
void setNormalMatrix();
};

}
#endif /* SCENENODE_H_ */

/*
 * SceneCamera.h
 *
 *  Created on: Jan 9, 2012
 *      Author: isaak
 */

#ifndef SCENECAMERA_H_
#define SCENECAMERA_H_

#include "SceneNode.h"

namespace Scene {

class SceneCamera : public SceneNode {
public:
SceneCamera();
virtual ~SceneCamera();

private:
glm::vec3 mPosition;
glm::vec3 mTarget;
glm::vec3 mUp;
};

}
#endif /* SCENECAMERA_H_ */

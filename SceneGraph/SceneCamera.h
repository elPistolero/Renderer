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

class SceneCamera: public SceneNode {
public:
	SceneCamera();
	virtual ~SceneCamera();

	void update();

	void setXVelocity(float amount) {
		mVelocity[0] = amount;
		mUpdated = false;
	}
	void setYVelocity(float amount) {
		mVelocity[1] = amount;
		mUpdated = false;
	}
	void setZVelocity(float amount) {
		mVelocity[2] = amount;
		mUpdated = false;
	}

	void incXRotation(float degree) {
		mXRotation += degree;
		mUpdated = false;
	}
	void incYRotation(float degree) {
		mYRotation += degree;
		mUpdated = false;
	}

	bool movingLeft() { return mVelocity[0] < 0.0f; }
	bool movingRight() { return mVelocity[0] > 0.0f; }
	bool movingForward() { return mVelocity[2] > 0.0f; }
	bool movingBackward() { return mVelocity[2] < 0.0f; }

private:
	glm::vec3 mVelocity;
	glm::vec3 mPosition;
	glm::vec3 mRight;
	glm::vec3 mTarget;
	glm::vec3 mUp;
	float mXRotation;
	float mYRotation;
};

}
#endif /* SCENECAMERA_H_ */

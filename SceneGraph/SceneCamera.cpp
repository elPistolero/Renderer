/*
 * SceneCamera.cpp
 *
 *  Created on: Jan 9, 2012
 *      Author: isaak
 */

#include "SceneCamera.h"

namespace Scene {

SceneCamera::SceneCamera()
: mVelocity(0.0f), mPosition(0.0f, 0.0f, 5.0f), mRight(1.0f, 0.0f, 0.0f),
  mTarget(0.0f, 0.0f, -1.0f), mUp(0.0f, 1.0f, 0.0f),
  mXRotation(180.0f),
  mYRotation(0.0f)
{
	setLocalTransformation(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f)));
}

SceneCamera::~SceneCamera() {
}

void SceneCamera::update() {
	// calculate camera defining vectors
	float cosX = glm::cos(glm::radians(mXRotation));
	float sinX = glm::sin(glm::radians(mXRotation));
	float cosY = glm::cos(glm::radians(mYRotation));
	float sinY = glm::sin(glm::radians(mYRotation));

	mTarget = glm::vec3(cosY * sinX, sinY, cosY * cosX);
	mRight = glm::vec3(glm::sin(glm::radians(mXRotation - 90.0f)), 0,
			glm::cos(glm::radians(mXRotation - 90.0f)));
	mUp = glm::cross(mRight, mTarget);

	// update position
	if (movingForward() || movingBackward()) {
		mPosition += mTarget * mVelocity.z;
	}

	if (movingLeft() || movingRight()) {
		mPosition += mRight * mVelocity.x;
	}

	glm::mat4 view = glm::lookAt(mPosition, mPosition + mTarget, mUp);

	setGlobalTransformation(view);
	SceneNode::update();
}

}

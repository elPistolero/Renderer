/*
 * SceneCamera.cpp
 *
 *  Created on: Jan 9, 2012
 *      Author: isaak
 */

#include "SceneCamera.h"

namespace Scene {

SceneCamera::SceneCamera() :
    mPosition(0.0f, 0.0f, -5.0f),
    mTarget(0.0f, 0.0f, -1.0f),
    mUp(0.0f, 1.0f, 0.0f)
{
    mTransformation = glm::lookAt(mPosition, mTarget, mUp);
}

SceneCamera::~SceneCamera() {
}

}

/*
 * OBJImporter.h
 *
 *  Created on: Feb 17, 2012
 *      Author: isaak
 */

#ifndef OBJIMPORTER_H_
#define OBJIMPORTER_H_

#include "GL/glew.h"
#include <vector>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <iostream>

namespace Importer {

class OBJImporter {
public:
	OBJImporter();
	virtual ~OBJImporter();

	void readFile(const std::string& filename,
			std::vector<GLfloat>& vertexBuffer,
			std::vector<GLfloat>& vertexNormalBuffer,
			std::vector<GLfloat>& textureCoordBuffer,
			std::vector<GLuint>& indexBuffer);
};

} /* namespace Importer */
#endif /* OBJIMPORTER_H_ */

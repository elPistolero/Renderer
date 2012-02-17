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
#include <string>
#include <iostream>

namespace OBJImporter {

class OBJImporter {
public:
	OBJImporter();
	virtual ~OBJImporter();

	void readFile(const std::string& filename, std::vector<GLfloat>& vertexBuffer, std::vector<GLuint>& indexBuffer);
};

} /* namespace OBJImporter */
#endif /* OBJIMPORTER_H_ */

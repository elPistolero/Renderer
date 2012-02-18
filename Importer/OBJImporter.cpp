/*
 * OBJImporter.cpp
 *
 *  Created on: Feb 17, 2012
 *      Author: isaak
 */

#include "OBJImporter.h"

namespace Importer {

OBJImporter::OBJImporter() {
}

OBJImporter::~OBJImporter() {
}

void OBJImporter::readFile(const std::string& filename, std::vector<GLfloat>& vertexBuffer, std::vector<GLuint>& indexBuffer) {
	vertexBuffer.clear();
	indexBuffer.clear();

	std::ifstream file(filename.c_str());
	std::string line;

	if (file.is_open()) {
		while (file.good()) {
			std::getline(file, line);
			if (line[0] == 'v') {
				float x,y,z = 0.0f;
				std::sscanf(line.c_str(), "v %f %f %f", &x, &y, &z);
				vertexBuffer.push_back(x);
				vertexBuffer.push_back(y);
				vertexBuffer.push_back(z);
			} else if (line[0] == 'f') {
				unsigned int a,b,c = 0;
				std::sscanf(line.c_str(), "f %d %d %d", &a, &b, &c);
				indexBuffer.push_back(a-1);
				indexBuffer.push_back(b-1);
				indexBuffer.push_back(c-1);
			}
		}
	}
}

} /* namespace OBJImporter */

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

void OBJImporter::readFile(const std::string& filename,
		std::vector<GLfloat>& vertexBuffer,
		std::vector<GLfloat>& vertexNormalBuffer,
		std::vector<GLfloat>& textureCoordBuffer,
		std::vector<GLuint>& indexBuffer) {
	vertexBuffer.clear();
	vertexNormalBuffer.clear();
	textureCoordBuffer.clear();
	indexBuffer.clear();

	std::vector<GLuint> normalIndexBuffer;
	std::vector<GLuint> texCoordIndexBuffer;
	std::vector<GLfloat> tmpNormalBuffer;
	std::vector<GLfloat> tmpTexCoordBuffer;

	std::ifstream file(filename.c_str());
	std::string line;

	if (file.is_open()) {
		while (file.good()) {
			std::getline(file, line);

			if (!strlen(line.c_str()))
				continue;

			char* type = strtok(strdup(line.c_str()), " ");
			if (!strcmp(type, "v")) {
				float x, y, z = 0.0f;
				std::sscanf(line.c_str(), "v %f %f %f", &x, &y, &z);
				vertexBuffer.push_back(x);
				vertexBuffer.push_back(y);
				vertexBuffer.push_back(z);
			} else if (!strcmp(type, "vn")) {
				float xn, yn, zn = 0.0f;
				std::sscanf(line.c_str(), "vn %f %f %f", &xn, &yn, &zn);
				tmpNormalBuffer.push_back(xn);
				tmpNormalBuffer.push_back(yn);
				tmpNormalBuffer.push_back(zn);
			} else if (!strcmp(type, "vt")) {
				float u, t = 0.0f;
				std::sscanf(line.c_str(), "vt %f %f", &u, &t);
				tmpTexCoordBuffer.push_back(u);
				tmpTexCoordBuffer.push_back(t);
			} else if (!strcmp(type, "f")) {
				unsigned int v1, v2, v3 = 0;
				unsigned int vn1, vn2, vn3 = 0;
				unsigned int vt1, vt2, vt3 = 0;
				if (std::sscanf(line.c_str(), "f %d %d %d", &v1, &v2, &v3)
						<= 1) {
					if (std::sscanf(line.c_str(), "f %d//%d %d//%d %d//%d", &v1,
							&vn1, &v2, &vn2, &v3, &vn3) <= 1) {
						std::sscanf(line.c_str(),
								"f %d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1,
								&v2, &vt2, &vn2, &v3, &vt3, &vn3);
						texCoordIndexBuffer.push_back(vt1 - 1);
						texCoordIndexBuffer.push_back(vt2 - 1);
						texCoordIndexBuffer.push_back(vt3 - 1);
					}
					normalIndexBuffer.push_back(vn1 - 1);
					normalIndexBuffer.push_back(vn2 - 1);
					normalIndexBuffer.push_back(vn3 - 1);
				}
				indexBuffer.push_back(v1 - 1);
				indexBuffer.push_back(v2 - 1);
				indexBuffer.push_back(v3 - 1);
			}
		}
	}

	std::vector<GLuint>::iterator it;
	for (it = texCoordIndexBuffer.begin(); it != texCoordIndexBuffer.end(); ++it) {
		textureCoordBuffer.push_back(tmpTexCoordBuffer[*it*2]);
		textureCoordBuffer.push_back(tmpTexCoordBuffer[*it*2+1]);
	}

	for (it = normalIndexBuffer.begin(); it != normalIndexBuffer.end(); ++it) {
		vertexNormalBuffer.push_back(tmpNormalBuffer[(*it*3)]);
		vertexNormalBuffer.push_back(tmpNormalBuffer[(*it*3+1)]);
		vertexNormalBuffer.push_back(tmpNormalBuffer[(*it*3+2)]);
	}
}

} /* namespace OBJImporter */

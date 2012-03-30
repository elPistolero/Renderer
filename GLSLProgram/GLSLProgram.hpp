#ifndef GLSLPROGRAM_H
#define GLSLPROGRAM_H

#include <fstream>
#include <GL/gl.h>
#include "../ProjectWideHeaders.h"

namespace Shader {

using std::ifstream;

class GLSLProgram {
    public:
        GLSLProgram();

        bool compileAndLinkShaders(const char* vertexShaderFile, const char* geometryShaderFile, const char* fragmentShaderFile);
        bool compileAndLinkShaders(const char* vertexShaderFile, const char* fragmentShaderFile);

        void use();
        void setUniform(const char* name, float x, float y, float z);
        void setUniform(const char* name, const glm::vec3& v);
        void setUniform(const char* name, const glm::vec4& v);
        void setUniform(const char* name, const glm::mat4& m);
        void setUniform(const char* name, const glm::mat3& m);
        void setUniform(const char* name, float val);
        void setUniform(const char* name, int val);
        void setUniform(const char* name, bool val);
        void bindAttribLocation(GLuint location, const char* name);
        void bindFragDataLocation(GLuint location, const char* name);
        GLint getUniformLocation(const char* name);
        GLint getHandle() { return mHandle; }

    private:
        char* readShaderFromFile(const char* fileName);

        GLint mHandle;
};

}
#endif

#include "GLSLProgram.hpp"

// makes a char array from a given file
namespace Shader {

GLSLProgram::GLSLProgram() : mHandle(-1)
{
}

char* GLSLProgram::readShaderFromFile(const char* fileName) {
    ifstream temp(fileName);
    int count = 0;
    char* buf;

    temp.seekg(0, std::ios::end);
    count = temp.tellg();

    buf = new char[count + 1];
    temp.seekg(0, std::ios::beg);
    temp.read(buf, count);
    buf[count] = 0;
    temp.close();

    return buf;
}

bool GLSLProgram::compileAndLinkShaders(const char* vertexShaderFile, const char* geometryShaderFile, const char* fragmentShaderFile) {
    const char* vertexSrc = readShaderFromFile(vertexShaderFile);
    const char* geomSrc = readShaderFromFile(geometryShaderFile);
    const char* fragSrc = readShaderFromFile(fragmentShaderFile);
    int isCompiledVS, isCompiledGS, isCompiledFS, maxLength, isLinked;
    GLint shader = glCreateProgram();

    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const GLchar**) &vertexSrc, 0);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiledVS);
    if (!isCompiledVS) {
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
        char* vertexInfoLog = new char[maxLength];

        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, vertexInfoLog);
        printf("vertex shader: compile error: ");
        printf("%s", vertexInfoLog);
        delete[] vertexInfoLog;
        return false;
    }

    // compile the geometry shader
    int geomShader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geomShader, 1, (const GLchar**) &geomSrc, 0);
    glCompileShader(geomShader);
    glGetShaderiv(geomShader, GL_COMPILE_STATUS, &isCompiledGS);
    if (!isCompiledGS) {
        glGetShaderiv(geomShader, GL_INFO_LOG_LENGTH, &maxLength);
        char* geomInfoLog = new char[maxLength];

        glGetShaderInfoLog(geomShader, maxLength, &maxLength, geomInfoLog);
        printf("geometry shader: compile error: ");
        printf("%s", geomInfoLog);
        delete[] geomInfoLog;
        return false;
    }

    // compile the fragment shader
    int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, (const GLchar**) &fragSrc, 0);
    glCompileShader(fragShader);
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &isCompiledFS);
    if (!isCompiledFS) {
        glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &maxLength);
        char* fragInfoLog = new char[maxLength];

        glGetShaderInfoLog(fragShader, maxLength, &maxLength, fragInfoLog);
        printf("fragment shader: compile error: ");
        printf("%s", fragInfoLog);
        delete[] fragInfoLog;
        return false;
    }

    // attach and link the shaders to the program
    glAttachShader(shader, vertexShader);
    glAttachShader(shader, geomShader);
    glAttachShader(shader, fragShader);

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, (int*) &isLinked);
    if (!isLinked) {
        glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        char* infoLog = new char[maxLength];

        glGetProgramInfoLog(shader, maxLength, &maxLength,
                            infoLog);
        fprintf(stdout, "shader: link error: ");
        printf("%s", infoLog);
        delete[] infoLog;
        return false;
    }

    mHandle = shader;

    return true;
}

bool GLSLProgram::compileAndLinkShaders(const char* vertexShaderFile, const char* fragmentShaderFile) {
    const char* vertexSrc = readShaderFromFile(vertexShaderFile);
    const char* fragSrc = readShaderFromFile(fragmentShaderFile);
    int isCompiledVS, isCompiledFS, maxLength, isLinked;
    GLint shader = glCreateProgram();

    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const GLchar**) &vertexSrc, 0);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiledVS);
    if (!isCompiledVS) {
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
        char* vertexInfoLog = new char[maxLength];

        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, vertexInfoLog);
        printf("vertex shader: compile error: ");
        printf("%s", vertexInfoLog);
        delete[] vertexInfoLog;
        return false;
    }

    // compile the fragment shader
    int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, (const GLchar**) &fragSrc, 0);
    glCompileShader(fragShader);
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &isCompiledFS);
    if (!isCompiledFS) {
        glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &maxLength);
        char* fragInfoLog = new char[maxLength];

        glGetShaderInfoLog(fragShader, maxLength, &maxLength, fragInfoLog);
        printf("fragment shader: compile error: ");
        printf("%s", fragInfoLog);
        delete[] fragInfoLog;
        return false;
    }

    // attach and link the shaders to the program
    glAttachShader(shader, vertexShader);
    glAttachShader(shader, fragShader);

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, (int*) &isLinked);
    if (!isLinked) {
        glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        char* infoLog = new char[maxLength];

        glGetProgramInfoLog(shader, maxLength, &maxLength,
                            infoLog);
        fprintf(stdout, "shader: link error: ");
        printf("%s", infoLog);
        delete[] infoLog;
        return false;
    }

    mHandle = shader;

    return true;
}

void GLSLProgram::use() {
  glUseProgram(mHandle);
}

void GLSLProgram::setUniform(const char* name, float x, float y, float z) {
  GLint loc = getUniformLocation(name);
  if (loc >= 0)
    glUniform3f(loc, x, y, z);
  else
    std::cerr << "Uniform " << name << " not found.\n";
}

void GLSLProgram::setUniform(const char* name, const glm::vec3& v) {
  GLint loc = getUniformLocation(name);
  if (loc >= 0)
    glUniform3f(loc, v.x, v.y, v.z);
  else
    std::cerr << "Uniform " << name << " not found.\n";
}

void GLSLProgram::setUniform(const char* name, const glm::vec4& v) {
  GLint loc = getUniformLocation(name);
  if (loc >= 0)
    glUniform4f(loc, v.x, v.y, v.z, v.w);
  else
    std::cerr << "Uniform " << name << " not found.\n";
}

void GLSLProgram::setUniform(const char* name, const glm::mat4& m) {
  GLint loc = getUniformLocation(name);
  if (loc >= 0)
    glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
  else
    std::cerr << "Uniform " << name << " not found.\n";
}

void GLSLProgram::setUniform(const char* name, const glm::mat3& m) {
  GLint loc = getUniformLocation(name);
  if (loc >= 0)
    glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
  else
    std::cerr << "Uniform " << name << " not found.\n";
}

void GLSLProgram::setUniform(const char* name, float val) {
  GLint loc = getUniformLocation(name);
  if (loc >= 0)
    glUniform1f(loc, val);
  else
    std::cerr << "Uniform " << name << " not found.\n";
}

void GLSLProgram::setUniform(const char* name, int val) {
  GLint loc = getUniformLocation(name);
  if (loc >= 0)
    glUniform1i(loc, val);
  else
    std::cerr << "Uniform " << name << " not found.\n";
}

void GLSLProgram::setUniform(const char* name, bool val) {
  GLint loc = getUniformLocation(name);
  if (loc >= 0)
    glUniform1i(loc, val);
  else
    std::cerr << "Uniform " << name << " not found.\n";

}

void GLSLProgram::bindAttribLocation(GLuint location, const char* name) {
  glBindAttribLocation(mHandle, location, name);
}

void GLSLProgram::bindFragDataLocation(GLuint location, const char* name) {
  glBindFragDataLocation(mHandle, location, name);
}

GLint GLSLProgram::getUniformLocation(const char* name) {
  return glGetUniformLocation(mHandle, name);
}

}

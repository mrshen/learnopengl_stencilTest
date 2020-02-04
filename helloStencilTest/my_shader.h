#ifndef MY_SHADER_H
#define MY_SHADER_H

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

void initVertexShader(GLuint*, const char * &);
void initFragmentShader(GLuint*, const char * &);
void initShaderProgram(GLuint*, GLuint, GLuint);

class MyShader
{
public:
	MyShader(const char*, const char*);
	void use();

	// uniform utils
	void setInt(const string&, int) const;
	void setBool(const string&, bool) const;
	void setFloat(const string&, float) const;
	void setMat4(const std::string &, const glm::mat4 &) const;
	void MyShader::setMaxtrix(const string &, float *, int, bool) const;

	GLuint getShaderProgramID()
	{
		return this->shaderProgramID;
	}

	void setShaderProgramID(GLuint val)
	{
		this->shaderProgramID = val;
	}

private:
	GLuint shaderProgramID;
};

#endif // !MY_SHADER_H

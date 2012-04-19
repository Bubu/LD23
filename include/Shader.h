#ifndef __Shader_h_included__
#define __Shader_h_included__
#include <string>
#include <gl/glew.h>
//#include <gl/glut.h>
class Shader
{
  public:
	Shader(std::string vertex, std::string fragment);
	inline void BindAttribLocation(GLuint index, std::string attribute)
	{
		glBindAttribLocation(program,index,attribute.c_str());	
		//http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttrib.xml
	}
	inline void setUniform3f(std::string uniform, float x, float y, float z)
	{
		GLint l=glGetUniformLocation(program,uniform.c_str()); 
		glUniform3f(l, x, y, z);
	}
	bool compileAndLink();
	bool use();	
	std::string getLog();
	static void unuse(){glUseProgram(0);}
  private:	
	GLuint program;
    GLuint vertex_main;
    GLuint fragment_main;
    std::string sv;
    std::string sf;
};
#endif

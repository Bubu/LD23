#include <Shader.h>
#include <fstream>
#include <iostream>
Shader::Shader(std::string vertex, std::string fragment):sv(vertex),sf(fragment),compiled(false)
{
    /*char c;
	std::ifstream ivs( vertex.c_str());	
	std::ifstream ifs( fragment.c_str());
	while (ivs.good())
	{
		c = ivs.get(); 
		if (ivs.good())sv+=c;
	}
	ivs.close();
	
	while (ifs.good())
	{
		c = ifs.get(); 
		if (ifs.good())sf+=c;
	}
	ifs.close();*/
	//std::cout<<"Fragment:"<<sf<<"\nVertex:"<<sv<<"\n";
}

bool Shader::compileAndLink()
{
	compiled=true;
	program=glCreateProgram(); 
	vertex_main=glCreateShader(GL_VERTEX_SHADER);
	fragment_main=glCreateShader(GL_FRAGMENT_SHADER);
	
	const GLchar* v=sv.c_str();
	glShaderSource(vertex_main, 1, &v,0);
	glCompileShader(vertex_main);
	GLint Compile_Status;
	glGetShaderiv(vertex_main, GL_COMPILE_STATUS,&Compile_Status); 
	if (!Compile_Status)return false;
	
	const GLchar* f=sf.c_str();
	glShaderSource(fragment_main, 1, &f,0);
	glCompileShader(fragment_main);
	glGetShaderiv(fragment_main, GL_COMPILE_STATUS,&Compile_Status); 
	if (!Compile_Status)return false;
	
	glAttachShader(program,vertex_main);
	glAttachShader(program,fragment_main);
	
	glLinkProgram(program);
	GLint Link_Status;
	glGetProgramiv(program,GL_LINK_STATUS,&Link_Status);
	if (!Link_Status)return false;
  
	return true;
}

bool Shader::use()
{
	if (!compiled)compileAndLink();
	GLint Link_Status;
	glGetProgramiv(program,GL_LINK_STATUS,&Link_Status);
	if (!Link_Status){std::cout<<"Shader:\n"<<getLog()<<"\n";return false; } 
	glUseProgram(program); 
	return true; 	
}	

std::string Shader::getLog()
{
	std::string tmp=sv+"\n\n"+sf;
	{
		std::string tmp2="";
		GLsizei infologLength = 0; 
		GLsizei charsWritten  = 0;
		GLchar *infoLog;
		glGetShaderiv(vertex_main, GL_INFO_LOG_LENGTH,&infologLength);
		if (infologLength > 0)
     	{
			infoLog = new GLchar[infologLength];
			glGetShaderInfoLog(vertex_main, infologLength, &charsWritten, infoLog);
			tmp2=infoLog;
			delete[] infoLog;
     	}
    	tmp+=tmp2+"\n";	
	}
	{
		std::string tmp2="";
		GLsizei infologLength = 0; 
		GLsizei charsWritten  = 0;
		GLchar *infoLog;
		glGetShaderiv(fragment_main, GL_INFO_LOG_LENGTH,&infologLength);
		if (infologLength > 0)
     	{
			infoLog = new GLchar[infologLength];
			glGetShaderInfoLog(fragment_main, infologLength, &charsWritten, infoLog);
			tmp2=infoLog;
			delete[] infoLog;
     	}
    	tmp+=tmp2+"\n";	
	}
	{
		std::string tmp2="";
		GLsizei infologLength = 0; 
		GLsizei charsWritten  = 0;
		GLchar *infoLog;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH,&infologLength);
		if (infologLength > 0)
		{
			infoLog = new GLchar[infologLength];
			glGetProgramInfoLog(program, infologLength, &charsWritten, infoLog);
			tmp2=infoLog;
			delete[] infoLog;
		}
		tmp+=tmp2;
	}
  return tmp;          
            
 }

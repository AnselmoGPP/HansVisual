#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

extern std::string VertexShaderCode, FragmentShaderCode, VertexShader2D;

GLuint LoadShaders(const std::string vertexShader, const std::string fragmentShader);

GLuint LoadShaders_2(const char * vertex_file_path,const char * fragment_file_path);



#endif

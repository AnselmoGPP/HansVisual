#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL/glew.h>

#include <string>

extern std::string VS_basic_3D, FS_basic, VS_basic_2D, VS_shadows, FS_shadows;

GLuint LoadShaders(const std::string vertexShader, const std::string fragmentShader);

GLuint LoadShaders_2(const char * vertex_file_path,const char * fragment_file_path);

#endif

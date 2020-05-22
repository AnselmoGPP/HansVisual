//#include <stdio.h>
#include <vector>
//#include <iostream>
#include <fstream>
//#include <algorithm>
#include <sstream>
//using namespace std;

//#include <stdlib.h>
//#include <string.h>

#include "shader.hpp"


std::string VS_basic_3D =
    "#version 330 core\n"

    "layout(location = 0) in vec3 vertexPosition_modelspace; \n"
    "layout(location = 1) in vec4 vertexColor; \n"

    "out vec4 fragmentColor; \n"

    "uniform mat4 MVP; \n"
    "uniform vec3 Cam_pos; \n"
    "uniform float Pnt_siz; \n"

    "void main() { \n"
    "	gl_Position = MVP * vec4(vertexPosition_modelspace, 1); \n"
    "	fragmentColor = vertexColor; \n"
    "	gl_PointSize = Pnt_siz / sqrt( (Cam_pos[0] - vertexPosition_modelspace[0]) * (Cam_pos[0] - vertexPosition_modelspace[0]) + (Cam_pos[1] - vertexPosition_modelspace[1]) * (Cam_pos[1] - vertexPosition_modelspace[1]) + (Cam_pos[2] - vertexPosition_modelspace[2]) * (Cam_pos[2] - vertexPosition_modelspace[2]) ); \n"
    "} \n";

std::string VS_basic_2D =
    "#version 330 core\n"

    "layout(location = 0) in vec3 vertexPosition; \n"
    "layout(location = 1) in vec4 vertexColor; \n"

    "out vec4 fragmentColor; \n"

    "void main() { \n"
    "	gl_Position = vec4(vertexPosition, 1); \n"
    "	fragmentColor = vertexColor; \n"
    "} \n";

std::string FS_basic =
    "#version 330 core \n"

    "in vec4 fragmentColor; \n"
    "out vec4 color; \n"

    "void main() { \n"
    "	color = fragmentColor; \n"
    "} \n";

std::string VS_shadows =
        "#version 330 core \n"

        // Input vertex data, different for all executions of this shader.
        "layout(location = 0) in vec3 vertexPosition_modelspace; \n"
        //"layout(location = 1) in vec2 vertexUV; \n"
        "layout(location = 2) in vec3 vertexNormal_modelspace; \n"

        // Output data ; will be interpolated for each fragment.
        //"out vec2 UV; \n"
        "out vec3 Position_worldspace; \n"
        "out vec3 Normal_cameraspace; \n"
        "out vec3 EyeDirection_cameraspace; \n"
        "out vec3 LightDirection_cameraspace; \n"

        // Values that stay constant for the whole mesh.
        "uniform mat4 MVP; \n"
        "uniform mat4 V; \n"
        "uniform mat4 M; \n"
        "uniform vec3 LightPosition_worldspace; \n"

        "void main(){ \n"

            "gl_Position =  MVP * vec4(vertexPosition_modelspace,1); \n"                // Output position of the vertex, in clip space : MVP * position

            "Position_worldspace = (M * vec4(vertexPosition_modelspace,1)).xyz; \n"     // Position of the vertex, in worldspace : M * position

            "vec3 vertexPosition_cameraspace = ( V * M * vec4(vertexPosition_modelspace,1)).xyz; \n"    // Vector that goes from the vertex to the camera, in camera space. In camera space, the camera is at the origin (0,0,0).
            "EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace; \n"

            "vec3 LightPosition_cameraspace = ( V * vec4(LightPosition_worldspace,1)).xyz; \n"          // Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
            "LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace; \n"

            "Normal_cameraspace = ( V * M * vec4(vertexNormal_modelspace,0)).xyz; \n"   // Normal of the the vertex, in camera space. Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.

            "UV = vertexUV; \n"             // UV of the vertex. No special space for this one.
        "} \n";

std::string FS_shadows =
        "#version 330 core \n"

        // Interpolated values from the vertex shaders
        "in vec2 UV; \n"
        "in vec3 Position_worldspace; \n"
        "in vec3 Normal_cameraspace; \n"
        "in vec3 EyeDirection_cameraspace; \n"
        "in vec3 LightDirection_cameraspace; \n"

        // Ouput data
        "out vec3 color; \n"

        // Values that stay constant for the whole mesh.
        "uniform sampler2D myTextureSampler; \n"
        "uniform mat4 MV; \n"
        "uniform vec3 LightPosition_worldspace; \n"

        "void main(){ \n"

            // Light emission properties
            // You probably want to put them as uniforms
            "vec3 LightColor = vec3(1,1,1); \n"
            "float LightPower = 50.0f; \n"

            // Material properties
            "vec3 MaterialDiffuseColor = texture( myTextureSampler, UV ).rgb; \n"
            "vec3 MaterialAmbientColor = vec3(0.1,0.1,0.1) * MaterialDiffuseColor; \n"
            "vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3); \n"

            // Distance to the light
            "float distance = length( LightPosition_worldspace - Position_worldspace ); \n"

            // Normal of the computed fragment, in camera space
            "vec3 n = normalize( Normal_cameraspace ); \n"
            // Direction of the light (from the fragment to the light)
            "vec3 l = normalize( LightDirection_cameraspace ); \n"
            // Cosine of the angle between the normal and the light direction,
            // clamped above 0
            //  - light is at the vertical of the triangle -> 1
            //  - light is perpendicular to the triangle -> 0
            //  - light is behind the triangle -> 0
            "float cosTheta = clamp( dot( n,l ), 0,1 ); \n"

            // Eye vector (towards the camera)
            "vec3 E = normalize(EyeDirection_cameraspace); \n"
            // Direction in which the triangle reflects the light
            "vec3 R = reflect(-l,n); \n"
            // Cosine of the angle between the Eye vector and the Reflect vector,
            // clamped to 0
            //  - Looking into the reflection -> 1
            //  - Looking elsewhere -> < 1
            "float cosAlpha = clamp( dot( E,R ), 0,1 ); \n"

            "color = \n"
                // Ambient : simulates indirect lighting
                "MaterialAmbientColor + \n"
                // Diffuse : "color" of the object
                "MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) + \n"
                // Specular : reflective highlight, like a mirror
                "MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance); \n"

        "} \n";

// ----------------------------------------------------------------------------------------------------------

GLuint LoadShaders(const std::string vertexShader, const std::string fragmentShader)
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	GLint Result = GL_FALSE;
    int InfoLogLength;

    // ----------------------------------------------------

	// Compile Vertex Shader
    char const * VertexSourcePointer = vertexShader.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
        std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
    char const * FragmentSourcePointer = fragmentShader.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

    // ----------------------------------------------------

	// Link the program
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
        std::vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
	}

    // ----------------------------------------------------
	
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

GLuint LoadShaders_2(const char * vertex_file_path, const char * fragment_file_path)
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

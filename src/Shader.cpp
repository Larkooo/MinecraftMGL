#include "Graphics/Shader.h"

Shader::Shader(std::string_view vertexPath, std::string_view fragmentPath, std::optional<std::string_view> geometryPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath.data());
        fShaderFile.open(fragmentPath.data());
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
	
	std::string geometryCode;
	std::ifstream gShaderFile;
	std::stringstream gShaderStream;
	if (geometryPath)
	{
		gShaderFile.open(geometryPath->data());
		gShaderStream << gShaderFile.rdbuf();
		geometryCode = gShaderStream.str();
		gShaderFile.close();
	}

    // 2. compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // print compile errors if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED " << fragmentPath << "\n" << infoLog << std::endl;
    };

    // similiar for Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // print compile errors if any
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED " << fragmentPath << "\n" << infoLog << std::endl;
    };

	// geometry shader
	unsigned int geometry;
	if (geometryPath)
	{
		const char* gShaderCode = geometryCode.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);

		// print compile errors if any
		glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(geometry, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED " << geometryPath->data() << "\n" << infoLog << std::endl;
		};
	}

    // shader Program
    m_Id = glCreateProgram();
    
	glAttachShader(m_Id, vertex);
    glAttachShader(m_Id, fragment);
	if (geometryPath)
		glAttachShader(m_Id, geometry);
    
	glLinkProgram(m_Id);
    // print linking errors if any
    glGetProgramiv(m_Id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_Id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
	if (geometryPath)
		glDeleteShader(geometry);
}

Shader::~Shader()
{
    // Since we already flagged our shaders for deletion, we do not need to flag them
    // for deletion again.
    // So we just need to delete the program and OpenGL will free everything 
    // for us
    glDeleteProgram(m_Id);
}

void Shader::Bind() const
{
    glUseProgram(m_Id);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

int Shader::GetUniformLocation(const std::string& name) const
{
    return glGetUniformLocation(m_Id, name.c_str());
}




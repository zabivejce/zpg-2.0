#include "ShaderProgram.hpp"
#include <cstdlib>
#include <iostream>
#include "fstream"
#include <string>
class ShaderCreator
{
    private:
    public:
        ShaderProgram* createProgramFromFiles(const char* vertexPath, const char* fragmentPath)
        {
            std::ifstream vertex(vertexPath);
            std::ifstream fragment(fragmentPath);
            if(!vertex.is_open() || !fragment.is_open())
            {
                if(!vertex.is_open())
                    std::cout << "cannot open one of shaders\t" << vertexPath <<std::endl;
                else
                    std::cout << "cannot open one of shader\t" << fragmentPath <<std::endl;
                exit(1);
            }
            std::string vertexCode((std::istreambuf_iterator<char>(vertex)),std::istreambuf_iterator<char>());
            std::string fragmentCode((std::istreambuf_iterator<char>(fragment)),std::istreambuf_iterator<char>());
            return createProgram(vertexCode.c_str(),fragmentCode.c_str());
        }
        ShaderProgram* createProgram(const char* vertexSh, const char* fragmentSh)
        {return new ShaderProgram(vertexSh,fragmentSh);}
};
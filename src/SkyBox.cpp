#include "SkyBox.hpp"
#include <cstdlib>
#include <glm/ext/matrix_float4x4.hpp>
SkyBox::SkyBox(std::vector<std::string> faces, ShaderProgram* shaderProgram)
{
    shader = shaderProgram;

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skycube), skycube, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO); //generate the VAO
    glBindVertexArray(VAO); //bind the VAO
    glEnableVertexAttribArray(0); //enable vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    tex = SOIL_load_OGL_cubemap(faces[0].c_str(),faces[1].c_str(),faces[2].c_str(),faces[3].c_str(),faces[4].c_str(),faces[5].c_str(),SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

    if(tex == NULL)
    {
        std::cout << "Cube map not loaded :)" << std::endl;
        exit(EXIT_FAILURE);
    }
}
void SkyBox::draw(glm::mat4 projection, glm::mat4 view)
{
    shader->setProgram();
    glActiveTexture(GL_TEXTURE0);
    shader->setUniform("view",view);
    shader->setUniform("projection",projection);
    shader->setUniform("skybox",0);

    glDepthMask(GL_FALSE);
    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, tex);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glDrawArrays(GL_TRIANGLES,0,108);
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);
    shader->resetProgram();
}
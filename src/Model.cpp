#include "Model.hpp"

Model::Model(float* pts, int arrS)
{
    this->points=pts;
    this->arrSize=arrS;

    glGenBuffers(1, &VBO); // generate the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, arrSize, points, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO); //generate the VAO
    glBindVertexArray(VAO); //bind the VAO
    glEnableVertexAttribArray(0); //enable vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void Model::drawModel()
{
    glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, arrSize / 3); //mode,first,count
}
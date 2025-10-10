#include "Model.hpp"

Model::Model(const float* pts, int arrS)
{
    this->points=pts;
    this->arrSize=arrS;

    glGenBuffers(1, &VBO); // generate the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, arrSize, points, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO); //generate the VAO
    glBindVertexArray(VAO); //bind the VAO
    glEnableVertexAttribArray(0); //enable vertex attributes
    glEnableVertexAttribArray(1); //enable vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid *)(3*sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void Model::drawModel()
{
    glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, arrSize / 6); //mode,first,count
}
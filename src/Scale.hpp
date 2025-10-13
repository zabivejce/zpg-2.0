#include "TransformationComponent.hpp"
#include <glm/ext/matrix_transform.hpp>
class Scale : public TransformationComponent
{
    private:
        glm::vec3 scale;
    public:
        Scale(glm::vec3 scale)
        {this->scale = scale;}
        glm::mat4 getModelMatrix() override
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::scale(model,scale);
            return model;
        }
};
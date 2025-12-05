#include "TransformationComponent.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
class DynamicRotation : public TransformationComponent
{
    private:
        float speed;
        float angle = 0.0f;
        glm::vec3 axis;
    public:
        DynamicRotation(glm::vec3 axis, float speed)
        {
            this->speed = speed;
            this->axis = axis;
        }
        glm::mat4 getModelMatrix() override
        {
            glm::mat4 model = glm::mat4(1.0f);

            model = glm::rotate(model, glm::radians(angle),axis);

            return model;
        }
        void update(float delta) override
        {
            angle += speed * delta * 10.0f;
            if(angle > 360.0f)
                angle -= 360.0f;
        }
};
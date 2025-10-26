#include <glm/ext/vector_float3.hpp>
class Light
{
    private:
        glm::vec3 position;
        glm::vec3 dimness;
    public:
        Light(glm::vec3 position, glm::vec3 dimness)
        {
            this->position = position;
            this->dimness - dimness;
        }
        glm::vec3 getPosition()
        {return position;}
        glm::vec3 getDimness()
        {return dimness;}
};
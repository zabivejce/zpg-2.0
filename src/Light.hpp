#include <glm/ext/vector_float3.hpp>
class Light
{
    private:
        glm::vec3 position;
        glm::vec3 attenuation;
    public:
        Light(glm::vec3 position, glm::vec3 attenuation)
        {
            this->position = position;
            this->attenuation = attenuation;
        }
        glm::vec3 getPosition()
        {return position;}
        glm::vec3 getAttenuation()
        {return attenuation;}
};
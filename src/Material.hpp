#pragma once
#include <glm/vec3.hpp>
class Material
{
    private:
        glm::vec3 ra;
        glm::vec3 rd;
        glm::vec3 rs;
        int power;
    public:
        Material(glm::vec3 ra, glm::vec3 rd, glm::vec3 rs, int power)
        {
            this->ra = ra;
            this->rd = rd;
            this->rs = rs;
            this->power = power;
        }
        glm::vec3 getRa()
        {return ra;}
        glm::vec3 getRd()
        {return rd;}
        glm::vec3 getRs()
        {return rs;}
        int getPower()
        {return power;}
};
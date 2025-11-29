#include "SpotLight.hpp"
#include "Observer.hpp"
#include "Camera.hpp"
#include "Subject.hpp"
class FlashLight : public SpotLight, public Observer
{
    private:
        Camera* camera;
        bool active = false;
    public:
        FlashLight(glm::vec3 position, glm::vec3 direction, glm::vec3 attenuation, float angle) : SpotLight(position, direction, attenuation, angle)
        {camera = nullptr;}
        void registerSubject(Subject* subj) override
        {camera = static_cast<Camera*>(subj);}
        void update() override
        {
            position = camera->getEye();
            direction = camera->getDirection();
            active = camera->flastStat();
        }
};
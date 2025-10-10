#pragma once

#include <glm/glm.hpp>
#include "TransformationComponent.hpp"
#include <vector>
class TransformationComposite : public TransformationComponent
{
    private:
        std::vector<TransformationComponent*> transformations;
    public:
        TransformationComposite();
        TransformationComposite(std::vector<TransformationComponent*> transformations)
        {
            this->transformations = transformations;
        }
        void add(TransformationComponent* transformation) override
        {
            transformations.emplace_back(transformation);
        }
        glm::mat4 getModelMatrix() override
        {
            glm::mat4 model = glm::mat4(1.0f);
            for(auto* tr : this->transformations)
                model *= tr->getModelMatrix();
            return model;
        }
};
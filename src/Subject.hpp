#pragma once
#include "Observer.hpp"
#include <vector>
class Observer;
class Subject
{
    private:
        std::vector<Observer*> observers;
    public:
        void notifyObs()
        {
            for(auto* obs : observers)
                obs->update();
        }
        void registerObserver(Observer* observer)
        {
            observers.emplace_back(observer);
            observer->registerSubject(this);
        }
};
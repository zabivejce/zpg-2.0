#pragma once
class Subject;
class Observer
{
    private:
    public:
        virtual void update(){}
        virtual void registerSubject(Subject* sub){}
        virtual ~Observer() = default;
};
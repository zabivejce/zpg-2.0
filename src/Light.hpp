#pragma once
class Light
{
    protected:
        bool active = true;
    public:
        virtual int getType() = 0;
        bool isActive()
        {return active;}
};
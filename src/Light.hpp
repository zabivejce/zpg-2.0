#pragma once
class Light
{
    private:
        bool active = true;
    public:
        virtual int getType() = 0;
        bool isActive()
        {return active;}
};
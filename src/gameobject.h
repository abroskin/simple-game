#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <chrono>

class GameObject
{
public:
    GameObject();
    virtual ~GameObject();

    virtual bool poll( std::chrono::milliseconds time_delta );
};

#endif // GAMEOBJECT_H

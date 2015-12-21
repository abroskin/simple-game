#include "gameobject.h"

GameObject::GameObject()
{

}

GameObject::~GameObject()
{

}

bool GameObject::poll( std::chrono::milliseconds time_delta )
{
    return true;
}


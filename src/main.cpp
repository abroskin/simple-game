#include <iostream>

#include "gameengine.h"

int main()
{
    GameEngine engine;
    if ( !engine.start( 755, 600, false ) )
    {
        return 0;
    }

    while ( engine.poll() )
    {
    }

    return 0;
}


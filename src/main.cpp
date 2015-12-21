#include <iostream>

#include "gameengine.h"

int main()
{
    GameEngine engine;
    if ( !engine.start( { 1366, 768 }, false ) )
    {
        return 0;
    }

    auto prev_time = std::chrono::high_resolution_clock::now();
    auto current_time = prev_time;
    while ( engine.poll( std::chrono::duration_cast< std::chrono::milliseconds >( current_time - prev_time ) ) )
    {
        prev_time = current_time;
        current_time = std::chrono::high_resolution_clock::now();
    }

    return 0;
}

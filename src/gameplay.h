#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "gameobject.h"

class GameEngine;

class GamePlay : public GameObject
{
public:
    GamePlay( GameEngine& game_engine );
    ~GamePlay() override;

    bool start();

    bool poll( std::chrono::milliseconds time_delta ) override;

private:
    GameEngine& m_game_engine;
};

#endif // GAMEPLAY_H

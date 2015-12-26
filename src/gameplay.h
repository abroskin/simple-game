#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "gameobject.h"
#include "brick.h"

#include <vector>

class GameEngine;

class GamePlay : public GameObject
{
public:
    GamePlay( GameEngine& game_engine );
    ~GamePlay() override;

    bool start();

    bool poll( std::chrono::milliseconds time_delta ) override;

private:
    static const size_t MIN_MATCH_NUMBER = 3;
    static const size_t FIELD_WIDTH = 8;
    static const size_t FIELD_HEIGHT = 8;

private:
    void try_swap( std::pair< std::shared_ptr< Brick >, std::shared_ptr< Brick > > swap_pair );
    std::vector< std::shared_ptr< Brick > > check_for_match();

    SDL_Point remove_brick( std::shared_ptr< Brick > brick );
    std::vector< SDL_Point > remove_bricks( std::vector< std::shared_ptr< Brick > > bricks );

    SDL_Point move_bricks_above( std::shared_ptr<Brick> brick );

    std::shared_ptr<Brick> add_brick();

    void init_field();
private:
    GameEngine& m_game_engine;

    std::vector< std::shared_ptr< Brick > > m_bricks;
    std::shared_ptr< Brick > m_highlighted_brick;
};

#endif // GAMEPLAY_H

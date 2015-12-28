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
    static const int MIN_MATCH_NUMBER = 3;
    static const int FIELD_WIDTH = 8;
    static const int FIELD_HEIGHT = 8;
    static const int SESSION_TIME = 60;

private:
    void try_swap( std::pair< std::shared_ptr< Brick >, std::shared_ptr< Brick > > swap_pair );
    std::vector< std::shared_ptr< Brick > > check_for_match( std::vector< std::shared_ptr< Brick > > including_bricks );

    void remove_brick( std::shared_ptr< Brick > brick );
    void remove_bricks( std::vector< std::shared_ptr< Brick > > bricks );

    void replace_brick( std::shared_ptr< Brick > brick );
    void replace_bricks( std::vector< std::shared_ptr< Brick > > bricks );

    SDL_Point move_bricks_above( std::shared_ptr<Brick> brick );

    std::shared_ptr< Brick > get_most_overlapped( const std::shared_ptr< Brick >& overlapping_brick ) const;

    std::shared_ptr<Brick> add_brick();

    void init_field();
private:
    GameEngine& m_game_engine;

    std::vector< std::shared_ptr< Brick > > m_bricks;
    std::shared_ptr< Brick > m_highlighted_brick;
    std::shared_ptr< Brick > m_dragged_brick;

    std::chrono::milliseconds m_session_time;
    int m_score;
};

#endif // GAMEPLAY_H

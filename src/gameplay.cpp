#include "gameplay.h"
#include "gameengine.h"

GamePlay::GamePlay( GameEngine& game_engine ):
    m_game_engine( game_engine )
{
}

GamePlay::~GamePlay()
{
}

bool GamePlay::start()
{
    // Background.
    std::shared_ptr< VisualObject > bg( new VisualObject );
    bg->set_coords( {0, 0} );
    if ( !bg->show_image( "data/BackGround.jpg" ) )
    {
         return false;
    }
    m_game_engine.add_visual_object( bg );

    //
    std::shared_ptr< DraggableObject > brick( new DraggableObject );
    brick->set_coords( {50, 50} );
    if ( !brick->show_image( "data/Red.png" ) )
    {
         return false;
    }
    m_game_engine.add_draggable_object( brick );

    return true;
}

bool GamePlay::poll(std::chrono::milliseconds time_delta)
{
    return true;
}


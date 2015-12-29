#include "gameplay.h"
#include "gameengine.h"

#include <array>
#include <algorithm>

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

    init_field();

    return true;
}



void GamePlay::try_swap( std::pair< std::shared_ptr< Brick >, std::shared_ptr< Brick > > swap_pair )
{
    // swap cell coordinates.
    std::swap( swap_pair.first->m_cell_coord, swap_pair.second->m_cell_coord );

    std::vector< std::shared_ptr< Brick > > including_bricks{ swap_pair.first, swap_pair.second };
    std::vector<std::shared_ptr<Brick> > matches = check_for_match( including_bricks );
    if ( matches.empty() )
    {
        // swap cell coordinates back.
        std::swap( swap_pair.first->m_cell_coord, swap_pair.second->m_cell_coord );
    }
    else
    {
        m_score += matches.size();
        m_highlighted_brick.reset();
        m_dragged_brick.reset();
        swap_pair.first->set_highlight( false );
        swap_pair.second->set_highlight( false );
        swap_pair.first->set_dragged( false );
        swap_pair.second->set_dragged( false );

        // Swap coordinates.
        SDL_Point tmp_point = swap_pair.first->get_coords();
        swap_pair.first->set_coords( swap_pair.second->get_coords() );
        swap_pair.second->set_coords( tmp_point );

        replace_bricks( matches );
    }
}

namespace
{
     void get_sequence( const Brick::Color& color,
                        std::vector< std::shared_ptr< Brick > >::iterator cur_it,
                        std::vector< std::shared_ptr< Brick > >::iterator end_it,
                        const SDL_Point& direction,
                        std::vector< std::shared_ptr< Brick > >& out_sequence )
     {
         if ( (*cur_it)->m_color == color )
         {
             out_sequence.push_back( *cur_it );

             auto next_it = std::find_if( cur_it, end_it, [&]( std::shared_ptr< Brick >& it )
             {
                 return (*cur_it)->m_cell_coord.x + direction.x == it->m_cell_coord.x &&
                        (*cur_it)->m_cell_coord.y + direction.y == it->m_cell_coord.y;
             });
             if ( next_it != end_it )
             {
                 get_sequence( color, next_it, end_it, direction, out_sequence );
             }
         }
     }
}

std::vector<std::shared_ptr<Brick> > GamePlay::check_for_match(
        std::vector< std::shared_ptr< Brick > > including_bricks )
{
    // Sort bricks.
    std::sort( including_bricks.begin(), including_bricks.end() );
    std::sort( m_bricks.begin(), m_bricks.end(),
               []( const std::shared_ptr< Brick >& a, const std::shared_ptr< Brick >& b )
    {
        return ( a->m_cell_coord.y == b->m_cell_coord.y ) ?
                    ( a->m_cell_coord.x < b->m_cell_coord.x ):
                    ( a->m_cell_coord.y < b->m_cell_coord.y );
    });

    // Find sequences.
    std::vector< std::shared_ptr< Brick > > matches;
    for ( auto brick_it = m_bricks.begin(); brick_it != m_bricks.end(); ++brick_it )
    {
        std::array< SDL_Point, 2 > directions{{{0,1}, {1,0}}};
        for( const auto& direction: directions )
        {
            std::vector< std::shared_ptr< Brick > > sequence;
            get_sequence( (*brick_it)->m_color, brick_it, m_bricks.end(), direction, sequence );
            if ( sequence.size() >= MIN_MATCH_NUMBER )
            {
                std::sort( sequence.begin(), sequence.end() );
                std::vector< std::shared_ptr< Brick > > intersection;
                std::set_intersection(  sequence.begin(), sequence.end(),
                                        including_bricks.begin(), including_bricks.end(),
                                        std::back_inserter( intersection ) );
                if ( !intersection.empty() )
                {
                    matches.insert( matches.end(), sequence.begin(), sequence.end() );
                }
            }
        }
    }

    // Remove duplicates.
    std::sort( matches.begin(), matches.end() );
    matches.erase( std::unique( matches.begin(), matches.end() ), matches.end() );

    return matches;
}

void GamePlay::remove_brick(std::shared_ptr<Brick> brick)
{
    m_bricks.erase( std::remove( m_bricks.begin(), m_bricks.end(), brick ), m_bricks.end() );
    m_game_engine.remove_draggable_object( brick );
}

void GamePlay::remove_bricks(std::vector<std::shared_ptr<Brick> > bricks)
{
    for ( auto& brick: bricks )
    {
        remove_brick( brick );
    }
}

SDL_Point GamePlay::move_bricks_above( std::shared_ptr<Brick> brick )
{
    auto brick_above_it = std::find_if( m_bricks.begin(), m_bricks.end(), [&]( std::shared_ptr< Brick >& it )
    {
        return brick->m_cell_coord.x == it->m_cell_coord.x &&
               brick->m_cell_coord.y - 1 == it->m_cell_coord.y;
    });

    std::shared_ptr<Brick> brick_above;
    if ( brick_above_it != m_bricks.end() )
    {
        brick_above = *brick_above_it;
        move_bricks_above( brick_above );
    }
    else
    {
        brick_above = add_brick();
    }

    brick_above->m_cell_coord = brick->m_cell_coord;
    brick_above->set_coords( brick->get_coords() );
}

std::shared_ptr<Brick> GamePlay::get_most_overlapped(const std::shared_ptr<Brick>& overlapping_brick ) const
{
    int square = 0;
    std::shared_ptr< Brick > res_brick;

    for ( auto& brick: m_bricks )
    {
        if ( brick != overlapping_brick )
        {
            SDL_Rect overlapping_rect = overlapping_brick->get_bounding_box();
            SDL_Rect curr_rect = brick->get_bounding_box();
            SDL_Rect intersection;
            if ( SDL_IntersectRect( &overlapping_rect, &curr_rect, &intersection ) )
            {
                if ( intersection.h * intersection.w > square )
                {
                    square = intersection.h * intersection.w;
                    res_brick = brick;
                }
            }
        }
    }
    return res_brick;
}

std::shared_ptr<Brick> GamePlay::add_brick()
{
    std::shared_ptr< Brick > brick( new Brick );
    m_bricks.push_back( brick );
    m_game_engine.add_draggable_object( brick );
    return brick;
}

bool GamePlay::get_swap_pair_by_highlighting(std::pair<std::shared_ptr<Brick>, std::shared_ptr<Brick> >& out_swap_pair)
{
    bool found_pair = false;
    for ( auto& brick: m_bricks )
    {
        if ( brick->is_highlighted() )
        {
            m_game_engine.render_on_top( brick );

            if ( m_highlighted_brick && brick != m_highlighted_brick )
            {
                out_swap_pair.first = brick;
                out_swap_pair.second = m_highlighted_brick;
                found_pair = true;
            }
            m_highlighted_brick = brick;
            break;
        }
    }
    return found_pair;
}

bool GamePlay::get_swap_pair_by_dragging(std::pair<std::shared_ptr<Brick>, std::shared_ptr<Brick> >& out_swap_pair)
{
    bool found_pair = false;

    std::shared_ptr< Brick > cur_dragged_brick;
    for ( auto& brick: m_bricks )
    {
        if ( brick->is_dragged() )
        {
            cur_dragged_brick = brick;
            break;
        }
    }

    if ( cur_dragged_brick )
    {
        m_dragged_brick = cur_dragged_brick;
    }
    else
    {
        if ( m_dragged_brick )
        {
            std::shared_ptr< Brick > overlapped_brick = get_most_overlapped( m_dragged_brick );
            m_dragged_brick->set_coords( m_game_engine.m_mouse_controller.m_dragged_obj_initial_coords );
            if ( overlapped_brick )
            {
                out_swap_pair.first = m_dragged_brick;
                out_swap_pair.second = overlapped_brick;
                found_pair = true;
            }

            m_dragged_brick.reset();
        }
    }
    return found_pair;
}

void GamePlay::replace_brick(std::shared_ptr<Brick> brick)
{
    move_bricks_above( brick );
    remove_brick( brick );
}

void GamePlay::replace_bricks(std::vector<std::shared_ptr<Brick> > bricks)
{
    for ( auto& brick: bricks )
    {
        replace_brick( brick );
    }
}

void GamePlay::init_field()
{
    m_score = 0;
    m_session_time = std::chrono::seconds(0);

    m_dragged_brick.reset();
    m_highlighted_brick.reset();

    remove_bricks( m_bricks );

    const int offset_x = 333u;
    const int step_x = 42u;
    const int offset_y = 103u;
    const int step_y = 42u;
    for( int i = 0u; i < FIELD_HEIGHT; ++i )
    {
        for( int j = 0u; j < FIELD_WIDTH; ++j )
        {
            std::shared_ptr< Brick > brick = add_brick();
            brick->m_cell_coord = { j, i };
            brick->set_coords( { offset_x + j*step_x, offset_y + i*step_y } );
        }
    }
}

bool GamePlay::poll(std::chrono::milliseconds time_delta)
{
    // Check time.
    m_session_time += time_delta;
    if ( std::chrono::duration_cast<std::chrono::seconds>(m_session_time) >= std::chrono::seconds( long(SESSION_TIME) ) )
    {
        std::string score_text = std::string( "You've got " ) + std::to_string( m_score ) +
                std::string( " points!\nNew session started." );

        m_game_engine.show_info_popup( "The session is over", score_text );

        init_field();

        return true;
    }

    // Check if there is a pair of bricks we can swap.
    std::pair< std::shared_ptr< Brick >, std::shared_ptr< Brick > > swap_pair;
    if ( !get_swap_pair_by_highlighting( swap_pair ) )
    {
        if ( !get_swap_pair_by_dragging( swap_pair ) )
        {
            return true;
        }
    }

    if ( SDL_abs( swap_pair.first->m_cell_coord.x - swap_pair.second->m_cell_coord.x ) +
         SDL_abs( swap_pair.first->m_cell_coord.y - swap_pair.second->m_cell_coord.y ) == 1 )
    {
        try_swap( swap_pair );
    }

    return true;
}


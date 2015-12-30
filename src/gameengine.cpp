#include "gameengine.h"
#include "gameplay.h"

#include <algorithm>
#include <iostream>

#include <SDL2/SDL_image.h>

namespace
{
void destroy_window( SDL_Window* window )
{
    SDL_DestroyWindow( window );
}
}

GameEngine::GameEngine():
    m_window( nullptr, destroy_window )
{
}

GameEngine::~GameEngine()
{
    IMG_Quit();
    SDL_Quit();
}

bool GameEngine::start( const int screen_width, const int screen_height, bool fullscreen )
{
    m_fullscreen = false;

    // Load SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "Unable to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }
    else
    {
        //Create window
        m_window.reset( SDL_CreateWindow(   "The game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                            screen_width, screen_height, SDL_WINDOW_SHOWN ) );

        if( !m_window )
        {
            std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
            return false;
        }

        if ( fullscreen )
        {
            if ( !toggle_fullscreen() )
            {
                return false;
            }
        }

        int flags = IMG_INIT_JPG|IMG_INIT_PNG;
        int initted = IMG_Init(flags);
        if( (initted & flags) != flags )
        {
            std::cout << "IMG_Init: Failed to init required jpg and png support! Error: " << IMG_GetError() << std::endl;
        }
    }

    // Start gameplay
    std::shared_ptr< GamePlay > gameplay( new GamePlay( *this ) );
    if ( !gameplay->start() )
    {
        return false;
    }

    add_object( gameplay );

    m_timestamp = std::chrono::high_resolution_clock::now();

    return true;
}

bool GameEngine::poll()
{
    // Timing
    auto current_time = std::chrono::high_resolution_clock::now();
    auto time_delta = std::chrono::duration_cast< std::chrono::milliseconds >( current_time - m_timestamp );
    m_timestamp = current_time;

    // Input
    if ( !handling_events() )
    {
        return false;
    }

    // Poll
    for ( auto& object: m_objects )
    {
        if ( !object->poll( time_delta ) )
        {
            return false;
        }
    }

    // Render
    if ( !render() )
    {
        return false;
    }

    return true;
}

void GameEngine::add_object(std::shared_ptr<GameObject> object)
{
    m_objects.push_back( object );
}

void GameEngine::remove_object( const std::shared_ptr<GameObject>& object)
{
    m_objects.erase( std::remove_if( m_objects.begin(), m_objects.end(), [&]( std::shared_ptr<GameObject>& it )
    {
        return it == object;
    } ), m_objects.end() );
}

void GameEngine::add_visual_object(std::shared_ptr<VisualObject> object)
{
    m_visual_objects.push_back( object );
    add_object( object );
}

void GameEngine::remove_visual_object( const std::shared_ptr<VisualObject>& object)
{
    m_visual_objects.erase( std::remove_if( m_visual_objects.begin(), m_visual_objects.end(),
                                            [&]( std::shared_ptr<VisualObject>& it )
    {
        return it == object;
    } ), m_visual_objects.end() );
    remove_object( object );
}

void GameEngine::add_draggable_object( std::shared_ptr<DraggableObject> object )
{
    m_draggable_objects.push_back( object );
    add_visual_object( object );
}

void GameEngine::remove_draggable_object( const std::shared_ptr<DraggableObject>& object)
{
    m_draggable_objects.erase( std::remove_if( m_draggable_objects.begin(), m_draggable_objects.end(),
                                               [&]( std::shared_ptr<DraggableObject>& it )->bool
    {
        return it == object;
    } ), m_draggable_objects.end() );
    remove_visual_object( object );
}

void GameEngine::render_on_top( std::shared_ptr<VisualObject> object )
{
    auto it = std::find( m_visual_objects.begin(), m_visual_objects.end(), object );
    if ( it != m_visual_objects.end() )
    {
        std::swap( *it, *m_visual_objects.rbegin() );
    }
}

void GameEngine::show_info_popup(const std::string& title, const std::string& message)
{
    auto start_time = std::chrono::high_resolution_clock::now();

    if ( m_fullscreen )
    {
        toggle_fullscreen();
    }

    SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_INFORMATION, title.c_str(), message.c_str(), nullptr );

    // Don't account events and time spent during popup message.
    m_timestamp += std::chrono::high_resolution_clock::now() - start_time;

    SDL_PumpEvents();
    SDL_FlushEvents( std::numeric_limits<Uint32>::min(), std::numeric_limits<Uint32>::max() );
}

bool GameEngine::toggle_fullscreen()
{
    m_fullscreen = !m_fullscreen;
    if ( SDL_SetWindowFullscreen( m_window.get(), m_fullscreen ? SDL_WINDOW_FULLSCREEN : 0 ) != 0 )
    {
        std::cout << "Can't toggle fullscreen mode! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

bool GameEngine::handling_events()
{
    SDL_Event event;
    while( SDL_PollEvent(&event) )
    {
        switch(event.type)
        {
            case SDL_KEYUP:
            {
                if ( event.key.keysym.scancode == SDL_SCANCODE_F )
                {
                    if ( !toggle_fullscreen() )
                    {
                        return false;
                    }
                }
                else if ( event.key.keysym.scancode == SDL_SCANCODE_ESCAPE )
                {
                    return false;
                }
                break;
            }
            case SDL_QUIT:
            {
                return false;
            }
            case SDL_MOUSEBUTTONDOWN:
            {
                m_mouse_controller.on_mouse_button_down(
                            m_draggable_objects, SDL_Point( { event.button.x, event.button.y } ) );
                break;
            }
            case SDL_MOUSEBUTTONUP:
            {
                m_mouse_controller.on_mouse_button_up(
                            m_draggable_objects, SDL_Point( { event.button.x, event.button.y } ) );
                break;
            }
            case SDL_MOUSEMOTION:
            {
                m_mouse_controller.on_mouse_move(
                            m_draggable_objects, SDL_Point( { event.motion.xrel, event.motion.yrel } ) );
                break;
            }
        }
    }
    return true;
}

bool GameEngine::render()
{
    SDL_Surface* screen = SDL_GetWindowSurface( m_window.get() );
    if ( !screen )
    {
        std::cout << "Can not get screen surface! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    for ( auto& object: m_visual_objects )
    {
        if ( !object->render( screen ) )
        {
            std::cout << "Can not render the object! SDL Error: " << SDL_GetError() << std::endl;
            return false;
        }
    }

    if ( SDL_UpdateWindowSurface( m_window.get() ) != 0 )
    {
        std::cout << "Can not update the screen! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}


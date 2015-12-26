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

bool GameEngine::start( const Size& screen_size, bool fullscreen )
{
    m_screen_size = screen_size;

    // Load SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "Unable to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }
    else
    {
        //Create window
        m_window.reset( SDL_CreateWindow(   "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                            screen_size.first, screen_size.second,
                                            fullscreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_OPENGL ) );
        if( !m_window )
        {
            std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
            return false;
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
    return true;
}

bool GameEngine::poll( std::chrono::milliseconds time_delta )
{
    // Input.
    SDL_Event event;
    while( SDL_PollEvent(&event) )
    {
        switch(event.type)
        {
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

    for ( auto& object: m_objects )
    {
        if ( !object->poll( time_delta ) )
        {
            return false;
        }
    }

    // Render
    SDL_Surface* screen = SDL_GetWindowSurface( m_window.get() );
    if ( !screen )
    {
        std::cout << "Can not get screen surface! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    for ( auto& object: m_visual_objects )
    {
        if ( !object->render( screen, m_screen_size ) )
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

void GameEngine::add_object(std::shared_ptr<GameObject> object)
{
    m_objects.push_back( object );
}

void GameEngine::remove_object( std::shared_ptr<GameObject> object)
{
    m_objects.erase( std::remove_if( m_objects.begin(), m_objects.end(), [&]( std::shared_ptr<GameObject>& it )->bool
    {
        return it == object;
    } ), m_objects.end() );
}

void GameEngine::add_visual_object(std::shared_ptr<VisualObject> object)
{
    m_visual_objects.push_back( object );
    add_object( object );
}

void GameEngine::remove_visual_object( std::shared_ptr<VisualObject> object)
{
    m_visual_objects.erase( std::remove_if( m_visual_objects.begin(), m_visual_objects.end(),
                                            [&]( std::shared_ptr<VisualObject>& it )->bool
    {
        return it == object;
    } ), m_visual_objects.end() );
    remove_object( object );
}

void GameEngine::add_draggable_object(std::shared_ptr<DraggableObject> object)
{
    m_draggable_objects.push_back( object );
    add_visual_object( object );
}

void GameEngine::remove_draggable_object( std::shared_ptr<DraggableObject> object)
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


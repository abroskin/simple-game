#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <chrono>
#include <vector>
#include <memory>

#include "gameobject.h"
#include "draggableobject.h"
#include "mousecontroller.h"

class GameEngine: public GameObject
{
public:
    GameEngine();
    ~GameEngine() override;

    bool start( const int screen_width, const int screen_height, bool fullscreen );
    bool poll( std::chrono::milliseconds time_delta ) override;

    void add_object( std::shared_ptr< GameObject > object );
    void remove_object( const std::shared_ptr< GameObject >& object );

    void add_visual_object( std::shared_ptr< VisualObject > object );
    void remove_visual_object( const std::shared_ptr< VisualObject >& object );

    void add_draggable_object( std::shared_ptr< DraggableObject > object );
    void remove_draggable_object( const std::shared_ptr< DraggableObject >& object );

    void render_on_top( std::shared_ptr< VisualObject > object );

public:
    MouseController m_mouse_controller;

private:
    std::vector< std::shared_ptr< GameObject > > m_objects;
    std::vector< std::shared_ptr< VisualObject > > m_visual_objects;
    std::vector< std::shared_ptr< DraggableObject > > m_draggable_objects;

    std::unique_ptr< SDL_Window, void(*)( SDL_Window* ) > m_window;
};

#endif // GAMEENGINE_H

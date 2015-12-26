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

    bool start( const Size& screen_size, bool fullscreen );
    bool poll( std::chrono::milliseconds time_delta ) override;

    void add_object( std::shared_ptr< GameObject > object );
    void remove_object( std::shared_ptr< GameObject > object );

    void add_visual_object( std::shared_ptr< VisualObject > object );
    void remove_visual_object( std::shared_ptr< VisualObject > object );

    void add_draggable_object( std::shared_ptr< DraggableObject > object );
    void remove_draggable_object( std::shared_ptr< DraggableObject > object );

    std::shared_ptr< DraggableObject > get_dragging_object() const;
    std::shared_ptr< DraggableObject > get_highlighted_object() const;

private:
    std::vector< std::shared_ptr< GameObject > > m_objects;
    std::vector< std::shared_ptr< VisualObject > > m_visual_objects;
    std::vector< std::shared_ptr< DraggableObject > > m_draggable_objects;

    std::unique_ptr< SDL_Window, void(*)( SDL_Window* ) > m_window;

    Size m_screen_size;

    MouseController m_mouse_controller;
};

#endif // GAMEENGINE_H

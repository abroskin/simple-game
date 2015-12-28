#ifndef MOUSECONTROLLER_H
#define MOUSECONTROLLER_H

#include "draggableobject.h"

#include <vector>

class MouseController
{
public:
    MouseController();

    void on_mouse_button_down(  std::vector< std::shared_ptr< DraggableObject > >& draggable_objects,
                                const SDL_Point& coords );

    void on_mouse_button_up(  std::vector< std::shared_ptr< DraggableObject > >& draggable_objects,
                              const SDL_Point& coords );

    void on_mouse_move( std::vector< std::shared_ptr< DraggableObject > >& draggable_objects,
                        const SDL_Point& coords );

    void reset();

public:
    SDL_Point m_dragged_obj_initial_coords;
};

#endif // MOUSECONTROLLER_H

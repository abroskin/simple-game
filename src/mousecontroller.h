#ifndef MOUSECONTROLLER_H
#define MOUSECONTROLLER_H

#include "draggableobject.h"

#include <vector>

class MouseController
{
public:
    MouseController();

    void on_mouse_button_down(  std::vector< std::shared_ptr< DraggableObject > >& draggable_objects,
                                const Point& coords );

    void on_mouse_button_up(  std::vector< std::shared_ptr< DraggableObject > >& draggable_objects,
                              const Point& coords );

    void on_mouse_move( std::vector< std::shared_ptr< DraggableObject > >& draggable_objects,
                        const Point& coords );
};

#endif // MOUSECONTROLLER_H

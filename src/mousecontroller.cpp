#include "mousecontroller.h"

MouseController::MouseController()
{

}

namespace
{
bool test_point_in_rect( const SDL_Point& point, const SDL_Rect& rect )
{
    return  SDL_PointInRect( &point, &rect ) == SDL_TRUE;
}
}

void MouseController::on_mouse_button_down(std::vector<std::shared_ptr<DraggableObject> >& draggable_objects,
                                           const SDL_Point& coords)
{
    for ( auto& obj: draggable_objects )
    {
        if ( test_point_in_rect( coords, obj->get_bounding_box() ) )
        {
            obj->set_highlight( true );

            if ( obj->is_movable() )
            {
                obj->set_dragged( true );
                m_dragged_obj_initial_coords = obj->get_coords();
            }
        }
        else
        {
            obj->set_highlight( false );
        }
    }
}

void MouseController::on_mouse_button_up(std::vector<std::shared_ptr<DraggableObject> >& draggable_objects,
                                         const SDL_Point& coords)
{
    for ( auto& obj: draggable_objects )
    {
        obj->set_dragged( false );
    }
}

void MouseController::on_mouse_move(std::vector<std::shared_ptr<DraggableObject> >& draggable_objects,
                                    const SDL_Point& coords)
{
    std::shared_ptr<DraggableObject> dragging_object;
    for ( auto& obj: draggable_objects )
    {
        if ( obj->is_dragged() )
        {
            dragging_object = obj;
        }
    }

    if ( dragging_object )
    {
        SDL_Point point = dragging_object->get_coords();
        dragging_object->set_coords( SDL_Point( { point.x + coords.x, point.y + coords.y } ) );
    }
}


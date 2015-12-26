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
            if ( m_highlighted_object )
            {
                m_highlighted_object->set_highlight( false );
            }
            obj->set_highlight( true );

            m_highlighted_object = obj;

            if ( obj->is_movable() )
            {
                m_dragging_object = obj;
            }
        }
    }
}

void MouseController::on_mouse_button_up(std::vector<std::shared_ptr<DraggableObject> >& draggable_objects,
                                         const SDL_Point& coords)
{
    m_dragging_object.reset();
}

void MouseController::on_mouse_move(std::vector<std::shared_ptr<DraggableObject> >& draggable_objects,
                                    const SDL_Point& coords)
{
    if ( m_dragging_object )
    {
        SDL_Point point = m_dragging_object->get_coords();
        m_dragging_object->set_coords( SDL_Point( { point.x + coords.x, point.y + coords.y } ) );
    }
}

void MouseController::reset()
{
    m_dragging_object.reset();
    m_highlighted_object.reset();
}


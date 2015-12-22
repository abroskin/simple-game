#include "mousecontroller.h"

MouseController::MouseController()
{

}

namespace
{
bool test_point_in_rect( const Point& point, const Rect& rect )
{
    return  std::get<PointX>(point) >= std::get<RectX>( rect ) &&
            std::get<PointX>(point) <= std::get<RectX>( rect ) + std::get<RectWidth>( rect ) &&
            std::get<PointY>(point) >= std::get<RectY>( rect ) &&
            std::get<PointY>(point) <= std::get<RectY>( rect ) + std::get<RectHeight>( rect );
}
}

void MouseController::on_mouse_button_down(std::vector<std::shared_ptr<DraggableObject> >& draggable_objects,
                                           const Point& coords)
{
    for ( auto& obj: draggable_objects )
    {
        if ( test_point_in_rect( coords, obj->get_bounding_box() ) )
        {
            obj->set_highlight( !obj->is_highlighted() );
        }
    }
}

void MouseController::on_mouse_button_up(std::vector<std::shared_ptr<DraggableObject> >& draggable_objects,
                                         const Point& coords)
{

}

void MouseController::on_mouse_move(std::vector<std::shared_ptr<DraggableObject> >& draggable_objects,
                                    const Point& coords)
{

}


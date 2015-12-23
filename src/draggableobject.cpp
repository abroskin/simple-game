#include "draggableobject.h"
#include "common.h"

DraggableObject::DraggableObject():
    m_is_highlighted( false )
{

}

DraggableObject::~DraggableObject()
{

}

bool DraggableObject::render(SDL_Surface* surf, const Size& surf_size)
{
    if ( m_img && is_highlighted() )
    {
        SDL_Rect dst_rect = get_bounding_box();
        SDL_FillRect( surf, &dst_rect, SDL_MapRGB( surf->format, 255, 255, 0 ) );
    }
    return VisualObject::render( surf, surf_size );
}

void DraggableObject::set_highlight(bool val)
{
    m_is_highlighted = val;
}

bool DraggableObject::is_highlighted()
{
    return m_is_highlighted;
}


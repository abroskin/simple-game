#include "draggableobject.h"

DraggableObject::DraggableObject():
    m_is_highlighted( false ),
    m_is_movable( true ),
    m_is_dragged( false )
{

}

DraggableObject::~DraggableObject()
{

}

bool DraggableObject::render( SDL_Surface* surf )
{
    if ( m_img && is_highlighted() )
    {
        SDL_Rect dst_rect = get_bounding_box();
        SDL_FillRect( surf, &dst_rect, SDL_MapRGB( surf->format, 255, 255, 0 ) );
    }
    return VisualObject::render( surf );
}

void DraggableObject::set_highlight(bool val)
{
    m_is_highlighted = val;
}

bool DraggableObject::is_highlighted() const
{
    return m_is_highlighted;
}

bool DraggableObject::is_movable() const
{
    return m_is_movable;
}

void DraggableObject::set_movable(const bool val)
{
    m_is_movable = val;
}

void DraggableObject::set_dragged( const bool val  )
{
    m_is_dragged = val;
}

bool DraggableObject::is_dragged() const
{
    return m_is_dragged;
}


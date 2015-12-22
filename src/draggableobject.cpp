#include "draggableobject.h"

DraggableObject::DraggableObject():
    m_is_highlighted( false )
{

}

DraggableObject::~DraggableObject()
{

}

bool DraggableObject::render(SDL_Surface* surf, const Size& surf_size)
{
    if ( is_highlighted() )
    {
        return true;
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


#include "visualobject.h"

#include <SDL2/SDL_image.h>

namespace
{
void free_surface( SDL_Surface* surf )
{
    SDL_FreeSurface( surf );
}
}

VisualObject::VisualObject():
    m_img( nullptr, free_surface )
{

}

VisualObject::~VisualObject()
{

}

void VisualObject::set_coords(const Point& coords)
{
    m_coords = coords;
}

Point VisualObject::get_coords() const
{
    return m_coords;
}

Rect VisualObject::get_bounding_box() const
{
    return Rect( m_coords.first, m_coords.second,
                 m_img ? m_img->w : 0,
                 m_img ? m_img->h : 0 );
}

bool VisualObject::show_image(const std::string& file)
{
    m_img.reset( IMG_Load( file.c_str() ) );
    return m_img.get() != 0;
}

bool VisualObject::render( SDL_Surface* surf, const Size& surf_size)
{
    SDL_Rect dst_rect;
    dst_rect.x = m_coords.first;
    dst_rect.y = m_coords.second;
    dst_rect.w = m_img->w;
    dst_rect.h = m_img->h;

    if ( SDL_BlitSurface( m_img.get(), nullptr, surf, &dst_rect ) != 0 )
    {
        return false;
    }

    return true;
}


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

void VisualObject::set_coords(const SDL_Point& coords)
{
    m_coords = coords;
}

SDL_Point VisualObject::get_coords() const
{
    return m_coords;
}

SDL_Rect VisualObject::get_bounding_box() const
{
    return SDL_Rect( { m_coords.x, m_coords.y,
                 m_img ? m_img->w : 0,
                 m_img ? m_img->h : 0 } );
}

bool VisualObject::show_image(const std::string& file)
{
    m_img.reset( IMG_Load( file.c_str() ) );
    return m_img.get() != 0;
}

bool VisualObject::render( SDL_Surface* surf )
{
    SDL_Rect dst_rect = get_bounding_box();

    if ( SDL_BlitSurface( m_img.get(), nullptr, surf, &dst_rect ) != 0 )
    {
        return false;
    }

    return true;
}


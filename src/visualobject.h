#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <string>
#include <memory>

#include <SDL2/SDL.h>

#include "gameobject.h"

class VisualObject : public GameObject
{
public:
    VisualObject();
    ~VisualObject() override;

    void set_coords( const std::pair< int, int >& coords );
    std::pair< int, int > get_coords() const;

    bool show_image( const std::string& file );
    bool render( SDL_Surface* surf, const std::pair< size_t, size_t >& surf_size );

private:
    std::pair< int, int > m_coords;
    std::unique_ptr< SDL_Surface, void(*)( SDL_Surface* ) > m_img;
};

#endif // VISUALOBJECT_H

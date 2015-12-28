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

    void set_coords( const SDL_Point& coords );
    SDL_Point get_coords() const;

    SDL_Rect get_bounding_box() const;

    bool show_image( const std::string& file );
    virtual bool render( SDL_Surface* surf );

protected:
    SDL_Point m_coords;
    std::unique_ptr< SDL_Surface, void(*)( SDL_Surface* ) > m_img;
};

#endif // VISUALOBJECT_H

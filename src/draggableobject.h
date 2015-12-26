#ifndef DRAGGABLEOBJECT_H
#define DRAGGABLEOBJECT_H

#include "visualobject.h"

class DraggableObject : public VisualObject
{
public:
    DraggableObject();
    ~DraggableObject() override;

    bool render( SDL_Surface* surf, const Size& surf_size ) override;

    void set_highlight( bool val );
    bool is_highlighted();

    bool is_movable() const;
    void set_movable( const bool val );

private:
    bool m_is_highlighted;
    bool m_is_movable;
};

#endif // DRAGGABLEOBJECT_H

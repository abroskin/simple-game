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

private:
    bool m_is_highlighted;
};

#endif // DRAGGABLEOBJECT_H

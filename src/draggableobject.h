#ifndef DRAGGABLEOBJECT_H
#define DRAGGABLEOBJECT_H

#include "visualobject.h"

class DraggableObject : public VisualObject
{
public:
    DraggableObject();
    ~DraggableObject() override;

    bool render( SDL_Surface* surf ) override;

    void set_highlight( bool val );
    bool is_highlighted() const;

    bool is_movable() const;
    void set_movable( const bool val );

    void set_dragged( const bool val );
    bool is_dragged() const;

private:
    bool m_is_highlighted;
    bool m_is_movable;
    bool m_is_dragged;
};

#endif // DRAGGABLEOBJECT_H

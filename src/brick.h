#ifndef BRICK_H
#define BRICK_H

#include "draggableobject.h"

class Brick : public DraggableObject
{
public:
    enum Color
    {
        Blue = 0,
        Green,
        Purple,
        Red,
        Yellow,
        ColorsCount
    };

public:
    Brick();
    ~Brick() override;

    const Color m_color;
    SDL_Point m_cell_coord;
};

#endif // BRICK_H

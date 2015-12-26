#include "brick.h"

namespace
{
const char* images[ Brick::ColorsCount ] = {"data/Blue.png",
                                             "data/Green.png",
                                             "data/Purple.png",
                                             "data/Red.png",
                                             "data/Yellow.png"};
}

Brick::Brick():
    m_color( Color( rand() % int( ColorsCount ) ) )
{
    show_image( images[ int( m_color ) ] );
}

Brick::~Brick()
{

}


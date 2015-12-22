#ifndef COMMON_H
#define COMMON_H

#include <tuple>

using Point = std::pair< int, int >;
enum PointFields
{
    PointX = 0,
    PointY
};

using Size = std::pair< int, int >;
enum SizeFields
{
    SizeWidth = 0,
    SizeHeight
};

using Rect = std::tuple< int, int, int, int >;
enum RectField
{
    RectX = 0,
    RectY,
    RectWidth,
    RectHeight
};

#endif // COMMON_H


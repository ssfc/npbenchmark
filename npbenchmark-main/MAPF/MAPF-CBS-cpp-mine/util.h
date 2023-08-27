//
// Created by take_ on 2023/8/3.
//

#ifndef DRAW_PATH_PY_UTIL_H
#define DRAW_PATH_PY_UTIL_H

struct Coordinate
{
    int x;
    int y;

    bool operator==(const Coordinate& other) const {
        return x == other.x && y == other.y;
    }
};

namespace std {
    template <>
    struct hash<Coordinate> {
        size_t operator()(const Coordinate& coord) const {
            return hash<int>()(coord.x) ^ hash<int>()(coord.y);
        }
    };
}

struct Agent
{
    Coordinate start;
    Coordinate goal;
};


#endif //DRAW_PATH_PY_UTIL_H

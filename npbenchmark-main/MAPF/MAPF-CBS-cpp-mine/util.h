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

struct Constraint
{
    int agent_id; // 人物
    std::vector<Coordinate> locations; // 可能是1个(点冲突), 也可能是2个(边冲突)
    int time_step; // 时间
};


#endif //DRAW_PATH_PY_UTIL_H

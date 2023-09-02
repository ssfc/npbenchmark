//
// Created by take_ on 2023/8/3.
//

#ifndef DRAW_PATH_PY_UTIL_H
#define DRAW_PATH_PY_UTIL_H

struct Coordinate
{
    int x;
    int y;

    bool operator==(const Coordinate& other) const
    {
        return x == other.x && y == other.y;
    }
};

namespace std
{
    template <>
    struct hash<Coordinate>
    {
        size_t operator()(const Coordinate& coord) const
        {
            return hash<int>()(coord.x) ^ hash<int>()(coord.y);
        }
    };
}

struct LocationTime
{
    Coordinate location;
    int time_step;

    bool operator==(const LocationTime& other) const
    {
        return location == other.location && time_step == other.time_step;
    }
};

namespace std
{
    template <>
    struct hash<LocationTime>
    {
        size_t operator()(const LocationTime& location_time) const
        {
            return hash<Coordinate>()(location_time.location) ^ hash<int>()(location_time.time_step);
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

struct Path
{
    std::vector<Coordinate> nodes;
};

#endif //DRAW_PATH_PY_UTIL_H

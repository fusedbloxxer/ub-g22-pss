#ifndef __CELL__
#define __CELL__

#include <stdexcept>
#include <iostream>
#include <format>

class Cell
{
public:
    enum class CellType
    {
        Player,
        Empty,
        Door,
        Wall,
    };

    template <typename Out, typename In>
    static Out convert_to(const In &value);

public:
    Cell(CellType &&cellType)
        : type(cellType) {}
    Cell()
        : Cell(CellType::Empty) {}

    inline operator bool() const
    {
        return this->type != CellType::Wall;
    }

public:
    CellType type;
};

std::ostream &operator<<(std::ostream &os, Cell &cell);
std::istream &operator>>(std::istream &is, Cell &cell);

#endif
#include <stdexcept>
#include <format>
#include <string>

#include "cell.h"

Cell::operator CellType() const
{
    return this->type;
}

Cell::operator bool() const
{
    return this->type != CellType::Wall;
}

std::ostream &operator<<(std::ostream &os, Cell &cell)
{
    return os << cell.type;
}

std::istream &operator>>(std::istream &is, Cell &cell)
{
    char c; is >> c;

    if (std::string("X01Y").find(c) == std::string::npos)
    {
        throw std::runtime_error(std::format("Invalid cell type: {}!", c));
    }

    return (cell.type = static_cast<Cell::CellType>(c)), is;
}

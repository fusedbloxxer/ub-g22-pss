#include "cell.h"

template <>
Cell::CellType Cell::convert_to(const char &value)
{
    switch (value)
    {
    case 'X':
        return CellType::Player;
    case '0':
        return CellType::Empty;
    case 'Y':
        return CellType::Door;
    case '1':
        return CellType::Wall;
    default:
        throw std::invalid_argument(std::format("Could not convert {} to CellType!", "value"));
    }
}

template <>
char Cell::convert_to(const CellType &value)
{
    switch (value)
    {
    case CellType::Player:
        return 'X';
    case CellType::Empty:
        return '0';
    case CellType::Wall:
        return '1';
    case CellType::Door:
        return 'Y';
    default:
        throw std::invalid_argument(std::format("Could not convert CellType {} to char!", static_cast<int>(value)));
    }
}

std::ostream &operator<<(std::ostream &os, Cell &cell)
{
    os << Cell::convert_to<char>(cell.type);
    return os;
}

std::istream &operator>>(std::istream &is, Cell &cell)
{
    char c;
    is >> c;
    cell.type = Cell::convert_to<Cell::CellType>(c);
    return is;
}
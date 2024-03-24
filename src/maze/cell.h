#ifndef __CELL__
#define __CELL__

#include <stdexcept>
#include <iostream>
#include <concepts>
#include <format>

template <typename T>
concept IsCell = requires(T cell)
{
    std::is_enum_v<typename T::CellType>;

    {T::CellType::Visited} -> std::convertible_to<typename T::CellType>;
    {T::CellType::Explore} -> std::convertible_to<typename T::CellType>;
    {T::CellType::Player} -> std::convertible_to<typename T::CellType>;
    {T::CellType::Empty} -> std::convertible_to<typename T::CellType>;
    {T::CellType::Door} -> std::convertible_to<typename T::CellType>;
    {T::CellType::Wall} -> std::convertible_to<typename T::CellType>;

    {cell.type} -> std::convertible_to<typename T::CellType>;
    {cell} -> std::convertible_to<typename T::CellType>;
    {cell} -> std::convertible_to<bool>;
};

struct Cell
{
    enum CellType : char
    {
        Visited = 'V',
        Explore = 'E',
        Player = 'X',
        Empty = '0',
        Door = 'Y',
        Wall = '1',
    };

    Cell(CellType &&cellType)
        : type(cellType) {}
    Cell()
        : Cell(CellType::Empty) {}

    inline operator CellType() const
    {
        return this->type;
    }

    inline operator bool() const
    {
        return this->type != CellType::Wall;
    }

    CellType type;
};

inline std::ostream &operator<<(std::ostream &os, Cell &cell)
{
    return os << cell.type;
}

inline std::istream &operator>>(std::istream &is, Cell &cell)
{
    char c; is >> c;

    if (std::string("X01VYE").find(c) == std::string::npos)
    {
        throw std::runtime_error(std::format("Invalid cell type: {}!", c));
    }

    return (cell.type = static_cast<Cell::CellType>(c)), is;
}

#endif
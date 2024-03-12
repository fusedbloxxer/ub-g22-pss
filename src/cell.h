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
        Player = 'X',
        Empty = '0',
        Door = 'Y',
        Wall = '1',
    };

    Cell(CellType &&cellType)
        : type(cellType) {}
    Cell()
        : Cell(CellType::Empty) {}

    operator CellType() const;
    operator bool() const;

    CellType type;
};

std::ostream &operator<<(std::ostream &os, Cell &cell);
std::istream &operator>>(std::istream &is, Cell &cell);

#endif
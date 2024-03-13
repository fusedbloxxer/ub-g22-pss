#ifndef __VISITOR__
#define __VISITOR__

#include <iostream>
#include <concepts>
#include <vector>

#include "graph.h"
#include "cell.h"
#include "maze.h"

template <typename T, typename P, typename N>
concept Visitor =
    requires(T vis, std::ostream& os) { os << vis; } &&
    requires(T vis, const P& problem, const N& node) { vis.visit(problem, node); };

template <IsCell T>
class ExplorationVisitor
{
public:
    template <IsCell U>
    friend std::ostream& operator<<(std::ostream& os, const ExplorationVisitor<U>& vis);

public:
    ExplorationVisitor(const std::vector<std::vector<T>>& cells)
        : _cells(cells) {}
    ExplorationVisitor(const Maze<T>& maze)
        : ExplorationVisitor(maze.cells) {}

    template<typename P, typename N>
    void visit(const P& problem, const N& node)
    {
        DetailsMap detailsMap = boost::get(vertex_details, problem.graph);

        VertexDetails details = boost::get(detailsMap, node.state.vertex);

        const auto& coords = details.coords;

        this->_cells[coords.first][coords.second] = T::CellType::Visited;

        std::cout << *this << std::endl;
    }

private:
    std::vector<std::vector<T>> _cells;
};

template <IsCell T>
std::ostream& operator<<(std::ostream& os, const ExplorationVisitor<T>& vis)
{
    for (auto it_r = vis._cells.cbegin(); it_r != vis._cells.cend(); ++it_r)
    {
        for (auto it_e = it_r->cbegin(); it_e != it_r->cend(); ++it_e)
        {
            os << static_cast<char>(*it_e) << " ";
        }

        os << std::endl;
    }

    return os;
}

#endif
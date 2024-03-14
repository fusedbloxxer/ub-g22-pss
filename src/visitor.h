#ifndef __VISITOR__
#define __VISITOR__

#include <iostream>
#include <concepts>
#include <vector>

#include "problem.h"
#include "graph.h"
#include "cell.h"
#include "maze.h"

struct Visitor
{
    virtual void visit(const Problem& problem, const Node& node) = 0;
};

template <IsCell T>
class ExplorationVisitor : public Visitor
{
    template <IsCell U>
    friend std::ostream& operator<<(std::ostream& os, const ExplorationVisitor<U>& vis);

public:
    ExplorationVisitor(const std::vector<std::vector<T>>& cells, std::ostream& os)
        : _cells(cells), os(os) {}
    ExplorationVisitor(const Maze<T>& maze, std::ostream& os)
        : ExplorationVisitor(maze.cells, os) {}

    void visit(const Problem& problem, const Node& node)
    {
        DetailsMap detailsMap = boost::get(vertex_details, problem.graph);

        VertexDetails details = boost::get(detailsMap, node.state.vertex);

        const auto& coords = details.coords;

        this->_cells[coords.first][coords.second] = T::CellType::Visited;

        os << *this << std::endl;

        ++_steps;
    }

private:
    std::vector<std::vector<T>> _cells;
    std::ostream& os;
    int _steps = 0;
};

template <IsCell T>
std::ostream& operator<<(std::ostream& os, const ExplorationVisitor<T>& vis)
{
    os << "Step: " << vis._steps << std::endl;

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
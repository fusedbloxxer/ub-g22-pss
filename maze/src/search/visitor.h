#ifndef __VISITOR__
#define __VISITOR__

#include <iostream>
#include <concepts>
#include <vector>

#include "../maze/graph.h"
#include "../maze/cell.h"
#include "../maze/maze.h"
#include "problem.h"

struct Visitor
{
    virtual void visit(const Problem &problem, const std::shared_ptr<Node> node) = 0;
};

template <IsCell T>
class ExplorationVisitor : public Visitor
{
    template <IsCell U>
    friend std::ostream &operator<<(std::ostream &os, const ExplorationVisitor<U> &vis);

public:
    ExplorationVisitor(const std::vector<std::vector<T>> &cells, std::ostream &os)
        : _cells(cells), os(os) {}
    ExplorationVisitor(const Maze<T> &maze, std::ostream &os)
        : ExplorationVisitor(maze.cells, os) {}

    void visit(const Problem &problem, const std::shared_ptr<Node> node)
    {
        solution = node;

        DetailsMap detailsMap = boost::get(vertex_details, problem.graph);

        VertexDetails details = boost::get(detailsMap, node->state.vertex);

        const auto &coords = details.coords;

        this->_cells[coords.first][coords.second] = T::CellType::Explore;

        this->_history.push_back(this->_cells);

        os << *this << std::endl;

        this->_cells[coords.first][coords.second] = T::CellType::Visited;

        ++_steps;
    }

    inline const std::vector<std::vector<std::vector<T>>>& getHistory() const
    {
        return this->_history;
    }

public:
    std::shared_ptr<Node> solution;

private:
    std::vector<std::vector<std::vector<T>>> _history;
    std::vector<std::vector<T>> _cells;
    std::ostream &os;
    int _steps = 0;
};

template <IsCell T>
std::ostream &operator<<(std::ostream &os, const ExplorationVisitor<T> &vis)
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
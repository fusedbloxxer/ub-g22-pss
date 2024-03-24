#ifndef __MAZE__
#define __MAZE__

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#include <type_traits>
#include <filesystem>
#include <stdexcept>
#include <concepts>
#include <iostream>
#include <fstream>
#include <format>
#include <vector>
#include <memory>
#include <map>

#include "cell.h"

template <IsCell T>
struct Maze
{
private:
    using CellType = T::CellType;
    std::vector<std::vector<T>> _cells;
    std::pair<int, int> _initPos;
    std::pair<int, int> _stopPos;

public:
    const decltype(_initPos) &initPos;
    const decltype(_stopPos) &stopPos;
    const decltype(_cells) &cells;

private:
    Maze(int N, int M)
        : _cells(N, std::vector<T>(M)), initPos(_initPos), stopPos(_stopPos), cells(_cells) {}

public:
    inline std::vector<T> &operator[](int index)
    {
        return this->_cells[index];
    }

    static Maze<T> from_input(std::istream& is)
    {
        std::filesystem::path filepath;
        is >> filepath;
        return Maze<T>::from_file(filepath);
    }

    static Maze<T> from_file(std::filesystem::path filepath)
    {
        std::ifstream fs(filepath, std::ios::in);

        if (!fs.is_open())
        {
            throw std::runtime_error(std::format("Cannot open {}!", filepath.string()));
        }

        int N, M;
        fs >> N >> M;
        Maze<T> maze(N, M);
        bool hasInit = false, hasStop = false;

        for (int i = 0; i != N; ++i)
        {
            for (int j = 0; j != M; ++j)
            {
                fs >> maze[i][j];

                if (maze[i][j] == CellType::Player)
                {
                    if (hasInit)
                    {
                        throw std::runtime_error(std::format("Found a second starting positions at ({}, {})!", i, j));
                    }

                    maze._initPos = std::make_pair(i, j);
                    hasInit = true;
                }

                if (maze[i][j] == CellType::Door)
                {
                    if (hasStop)
                    {
                        throw std::runtime_error(std::format("Found a second stopping positions at ({}, {})!", i, j));
                    }

                    maze._stopPos = std::make_pair(i, j);
                    hasStop = true;
                }
            }
        }

        if (!hasInit)
        {
            throw std::runtime_error("Found no starting position!");
        }

        if (!hasStop)
        {
            throw std::runtime_error("Found no stopping position!");
        }

        return maze;
    }

    template <typename Graph, typename PropMap>
    static auto to_graph(const Maze<T> &maze)
    {
        // Extract Types
        using Vertex = boost::graph_traits<Graph>::vertex_descriptor;
        using PropVal = boost::property_traits<PropMap>::value_type;

        // Allocate memory for graph & coord2vec
        std::map<std::pair<int, int>, Vertex> coord2vertex;
        auto graphPtr = std::make_unique<Graph>();
        Graph &graph = *graphPtr;

        // Fill in the graph based on the maze
        for (int i = 0; i != maze.cells.size(); ++i)
        {
            for (int j = 0; j != maze.cells[i].size(); ++j)
            {
                if (maze.cells[i][j])
                {
                    // Insert new vertex along with information regarding it
                    PropVal value = { { i, j }, maze.cells[i][j].type };
                    Vertex vertex = boost::add_vertex(value, graph);
                    coord2vertex[value.coords] = vertex;

                    // Add link to the top neighbor
                    auto top_idx = std::make_pair(i - 1, j);
                    auto top_itr = coord2vertex.find(top_idx);
                    if (top_itr != coord2vertex.end())
                    {
                        boost::add_edge(vertex, top_itr->second, graph);
                    }

                    // Add link to the left neighbor
                    auto left_idx = std::make_pair(i, j - 1);
                    auto left_itr = coord2vertex.find(left_idx);
                    if (left_itr != coord2vertex.end())
                    {
                        boost::add_edge(vertex, left_itr->second, graph);
                    }

                    // Add link to the top-left neightbor
                    auto top_left_idx = std::make_pair(i - 1, j - 1);
                    auto top_left_itr = coord2vertex.find(top_left_idx);
                    if (top_left_itr != coord2vertex.end())
                    {
                        boost::add_edge(vertex, top_left_itr->second, graph);
                    }

                    // Add link to the top-right neightbor
                    auto top_right_idx = std::make_pair(i - 1, j + 1);
                    auto top_right_itr = coord2vertex.find(top_right_idx);
                    if (top_right_itr != coord2vertex.end())
                    {
                        boost::add_edge(vertex, top_right_itr->second, graph);
                    }
                }
            }
        }

        return std::make_pair(std::move(graphPtr), std::move(coord2vertex));
    }
};

template <typename T>
std::ostream &operator<<(std::ostream &os, Maze<T> &maze)
{
    for (int i = 0; i != maze.cells.size(); ++i)
    {
        for (int j = 0; j != maze.cells[i].size(); ++j)
        {
            os << maze[i][j] << " ";
        }

        os << std::endl;
    }

    return os;
}

#endif

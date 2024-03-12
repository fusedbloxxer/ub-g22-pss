#ifndef __MAZE__
#define __MAZE__

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#include <type_traits>
#include <filesystem>
#include <stdexcept>
#include <fstream>
#include <format>
#include <vector>
#include <memory>
#include <map>

template <typename TCell>
class Maze
{
    static_assert(std::is_class<TCell>::value, "TCell is not a class!");

public:
    Maze(int N, int M)
        : cells(N, std::vector<TCell>(M)) {}
    Maze()
        : cells() {}

    inline std::vector<TCell> &operator[](int index)
    {
        return this->cells[index];
    }

    static Maze<TCell> from_file(std::filesystem::path &&filepath)
    {
        std::ifstream fs(filepath, std::ios::in);

        if (!fs.is_open())
        {
            throw std::runtime_error(std::format("Cannot open {}!", filepath.string()));
        }

        int N, M;
        fs >> N;
        fs >> M;

        char cell;
        Maze<TCell> maze(N, M);
        for (int i = 0; i != N; ++i)
        {
            for (int j = 0; j != M; ++j)
            {
                fs >> maze[i][j];
            }
        }

        return maze;
    }

    template<typename Graph, typename PropMap>
    static auto to_graph(const Maze<TCell>& maze)
    {
        // Extract Types
        using Vertex  = boost::graph_traits<Graph>::vertex_descriptor;
        using PropVal = boost::property_traits<PropMap>::value_type;

        // Allocate memory for graph & coord2vec
        std::map<std::pair<int, int>, Vertex> coord2vertex;
        auto graphPtr = std::make_unique<Graph>();
        Graph& graph = *graphPtr;

        // Fill in the graph based on the maze
        for (int i = 0; i != maze.cells.size(); ++i)
        {
            for (int j = 0; j != maze.cells[i].size(); ++j)
            {
                if (maze.cells[i][j])
                {
                    // Insert new vertex
                    PropVal value = { { i, j } };
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
                }
            }
        }

        return std::make_pair(std::move(graphPtr), std::move(coord2vertex));
    }

public:
    std::vector<std::vector<TCell>> cells;
};

template<typename TCell>
std::ostream& operator<<(std::ostream& os, Maze<TCell>& maze)
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

#include <format>
#include <vector>
#include <fstream>
#include <iostream>
#include <exception>
#include <stdexcept>
#include <filesystem>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_io.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_selectors.hpp>
#include "graph.h"
#include "zone.h"

int main()
{
    // Read the input data
    Zone<State> zone = Zone<State>::read_from("res/input.txt");

    // Transform into a graph
    Graph g;

    for (int i = 0; i != zone.data.size(); ++i)
    {
        for (int j = 0; j != zone.data[i].size(); ++j)
        {
            if (zone.data[i][j] != State::Wall)
            {
                const VertexDetails x { State::Free, std::make_tuple(i, j) };

                Vertex v = boost::add_vertex(x, g);

                auto details_map = boost::get(vertex_details, g);

                auto y = details_map[v];

                std::cout << y.s << " " << std::get<0>(y.coords) << " " << std::get<1>(y.coords) << " = ";
            }

        }

        std::cout << std::endl;
    }

    return EXIT_SUCCESS;
}
#include <map>
#include <utility>
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
    Zone<State> zone = Zone<State>::read_from("res/input.txt");

    auto&& [graph, c2v] = build_graph(zone);

    auto details_map = get(vertex_details, graph);

    for (auto&& [itp, endp] = vertices(graph); itp != endp; ++itp)
    {
        VertexDetails parent_details = get(details_map, *itp);

        std::cout << std::format("parent ({}, {}): ", parent_details.coords.first, parent_details.coords.second);

        for (auto&& [itc, endc] = adjacent_vertices(*itp, graph); itc != endc; ++itc)
        {
            VertexDetails child_details = get(details_map, *itc);

            std::cout << std::format("({}, {}) ", child_details.coords.first, child_details.coords.second);
        }

        std::cout << std::endl;
    }

    return EXIT_SUCCESS;
}
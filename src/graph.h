#ifndef __GRAPH__
#define __GRAPH__

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <utility>
#include "zone.h"

// --- Implementation Details ---
struct VertexDetails
{
    // Where is it located in the zone
    std::pair<int, int> coords;
};

// --- Property Mappings ---
enum vertex_details_t { vertex_details = 256 };

namespace boost {
    BOOST_INSTALL_PROPERTY(vertex, details);
}

using VertexProperties = boost::property<vertex_details_t, VertexDetails>;

// --- Common Types ---
using Graph = boost::adjacency_list<boost::vecS, boost::setS, boost::undirectedS, VertexProperties, boost::no_property>;
using Vertex = boost::graph_traits<Graph>::vertex_descriptor;


// --- Implementation ---
template<typename S>
auto build_graph(const Zone<S>& zone)
{
    Graph graph;

    std::map<std::pair<int, int>, Vertex> coord2vertex;

    for (int i = 0; i != zone.data.size(); ++i)
    {
        for (int j = 0; j != zone.data[i].size(); ++j)
        {
            if (zone.data[i][j] != State::Wall)
            {
                // Insert new vertex
                VertexDetails details = { { i, j } };
                Vertex vertex = boost::add_vertex(details, graph);
                coord2vertex[details.coords] = vertex;

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

    struct GraphAndCoords
    {
        decltype(graph) g;
        decltype(coord2vertex) c2v;
    };

    return GraphAndCoords { graph, coord2vertex };
}

#endif
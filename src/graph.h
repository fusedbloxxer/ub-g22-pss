#ifndef __GRAPH__
#define __GRAPH__

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <utility>

#include "cell.h"

// --- Implementation Details ---
struct VertexDetails
{
    // Where is it located in the maze
    std::pair<int, int> coords;

    // What kind of cell it represents
    Cell::CellType type;
};

// --- Property Mappings ---
enum vertex_details_t { vertex_details = 256 };

namespace boost {
    BOOST_INSTALL_PROPERTY(vertex, details);
}

// --- Useful Types ---
using VertexDetailsProperty = boost::property<vertex_details_t, VertexDetails>;

using Graph = boost::adjacency_list<boost::vecS, boost::setS, boost::undirectedS, VertexDetailsProperty, boost::no_property>;

using DetailsMap = boost::property_map<Graph, vertex_details_t>::type;

using Vertex = boost::graph_traits<Graph>::vertex_descriptor;

using Edge = boost::graph_traits<Graph>::edge_descriptor;

#endif
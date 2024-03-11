#ifndef __GRAPH__
#define __GRAPH__

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <tuple>
#include "zone.h"

// --- Implementation Details ---
struct VertexDetails
{
    // How it is represented in the zone
    State s;

    // Where is it located in the zone
    std::tuple<int, int> coords;
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

#endif
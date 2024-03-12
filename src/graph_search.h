#ifndef __GRAPH_SEARCH__
#define __GRAPH_SEARCH__

#include <stdexcept>
#include <set>
#include "graph.h"

struct State
{
    State(Graph& graph, Vertex vertex)
        : graph(graph), vertex(vertex) {}

    Vertex vertex;
    Graph& graph;
};

struct Action
{
    Edge edge;
};

std::vector<Action> ACTIONS(const State& state)
{
    std::vector<Action> actions;

    for (auto&& [it_s, it_e] = boost::out_edges(state.vertex, state.graph); it_s != it_e; ++it_s)
    {
        actions.push_back({ *it_s });
    }

    return actions;
}

State RESULT(const State& state, Action action)
{
    if (state.vertex != boost::source(action.edge, state.graph))
    {
        throw std::invalid_argument("Input Vertex should be a source!");
    }

    Vertex x = boost::target(action.edge, state.graph);

    return State(state.graph, x);
}

class GraphSearch
{

};

#endif
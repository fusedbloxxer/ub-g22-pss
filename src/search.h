#ifndef __SEARCH__
#define __SEARCH__

#include <unordered_set>
#include <functional>
#include <algorithm>
#include <stdexcept>
#include <concepts>
#include <memory>
#include <queue>
#include <stack>

#include "visitor.h"
#include "graph.h"

struct State
{
    Vertex vertex;

    State(Vertex vertex)
        : vertex(vertex) {}

    inline bool operator==(const State& state) const
    {
        return this == &state || this->vertex == state.vertex;
    }
};

class StateComparer
{
    Graph& graph;

public:
    StateComparer(Graph& graph)
        : graph(graph) {}

    bool operator()(const State& a, const State& b) const
    {
        auto detailsMap = boost::get(vertex_details, this->graph);
        auto detailsA = boost::get(detailsMap, a.vertex);
        auto detailsB = boost::get(detailsMap, b.vertex);
        return detailsA.coords < detailsB.coords;
    }
};

struct Action
{
    Edge edge;
    const int cost;
    const bool isNull;

    Action(Edge edge)
        : edge(edge), cost(1), isNull(false) {}
    Action()
        : cost(0), isNull(true) {}
};

struct Node
{
    State state;
    std::shared_ptr<Node> parent;
    Action action;
    const int pathCost;

    Node(std::shared_ptr<Node> parent, const State& state, const Action& action)
        : state(state), parent(parent), action(action), pathCost(action.cost + (!parent ? 0 : parent->pathCost)) {}
    Node(const State& state, const Action& action)
        : Node(std::shared_ptr<Node>(nullptr), state, action) {}
    Node(const State& state)
        : Node(state, Action()) {}

    inline bool isRoot() const
    {
        return !this->parent;
    }

    void chain(std::function<void(const Node&)> func) const
    {
        func(*this);

        if (this->isRoot())
        {
            return;
        }

        std::shared_ptr<Node> node;

        for (node = parent; !(node->isRoot()); node = node->parent)
        {
            func(*node);
        }

        func(*node);
    }

    void rchain(std::function<void(const Node&)> func) const
    {
        return this->rchain(func, *this);
    }

private:
    void rchain(std::function<void(const Node&)> func, const Node& node) const
    {
        if (!node.isRoot())
        {
            rchain(func, *node.parent);
        }

        func(node);
    }
};

struct Problem
{
    Graph& graph;
    State& initState;

    Problem(Graph& graph, State& state)
        : graph(graph), initState(state) {}

    std::vector<std::shared_ptr<Node>> expand(std::shared_ptr<Node> node) const
    {
        auto&& actions  = this->actions(node->state);

        auto&& children = std::vector<std::shared_ptr<Node>>(actions.size());

        std::transform(actions.cbegin(), actions.cend(), children.begin(),
            [this, &node](const Action& action)
            {
                State state = this->transition(node->state, action);

                return std::make_shared<Node>(node, state, action);
            }
        );

        return children;
    }

    std::vector<Action> actions(const State& state) const
    {
        std::vector<Action> actions;

        for (auto&& [it_s, it_e] = boost::out_edges(state.vertex, graph); it_s != it_e; ++it_s)
        {
            actions.push_back(Action(*it_s));
        }

        return actions;
    }

    State transition(const State& state, const Action& action) const
    {
        if (action.isNull)
        {
            throw std::logic_error("Must not perform transition on a null action!");
        }

        if (state.vertex != boost::source(action.edge, graph))
        {
            throw std::invalid_argument("Input Vertex should be a source!");
        }

        Vertex x = boost::target(action.edge, graph);

        return State(x);
    }

    bool isGoal(const State& state) const
    {
        DetailsMap detailsMap = boost::get(vertex_details, graph);

        VertexDetails details = boost::get(detailsMap, state.vertex);

        return details.type == Cell::CellType::Door;
    }

    bool isGoal(std::shared_ptr<Node> node) const
    {
        return this->isGoal(node->state);
    }
};

template <typename T> requires Visitor<T, Problem, Node>
std::shared_ptr<Node> breadthFirstSearch(Problem& problem, T& vis)
{
    auto node = std::make_shared<Node>(problem.initState);

    if (problem.isGoal(node->state))
    {
        vis.visit(problem, *node);
        return node;
    }

    auto&& frontier = std::queue<std::shared_ptr<Node>>();
    frontier.push(node);

    auto&& reached = std::set<State, StateComparer>(StateComparer(problem.graph));
    reached.insert(node->state);

    while(!frontier.empty())
    {
        node = frontier.front();
        vis.visit(problem, *node);
        frontier.pop();

        for (auto child : problem.expand(node))
        {
            if (problem.isGoal(child->state))
            {
                vis.visit(problem, *child);
                return child;
            }

            if (reached.find(child->state) == reached.end())
            {
                reached.insert(child->state);
                frontier.push(child);
            }
        }
    }

    return std::shared_ptr<Node>();
}

#endif
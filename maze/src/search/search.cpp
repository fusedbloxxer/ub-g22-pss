#include "search.h"

std::unique_ptr<Search> Search::createSearchAlgorithm(std::istream &is)
{
    std::string algorithm;

    is >> algorithm;

    boost::algorithm::to_lower(algorithm);

    return Search::createSearchAlgorithm(algorithm);
}

std::unique_ptr<Search> Search::createSearchAlgorithm(const std::string &algorithm)
{
    if (algorithm == "bfs")
    {
        return std::make_unique<BreadthFirstSearch>();
    }

    if (algorithm == "dfs")
    {
        return std::make_unique<DepthFirstSearch>();
    }

    if (algorithm == "ucs")
    {
        return std::make_unique<UniformCostSearch>();
    }

    if (algorithm == "a*l1")
    {
        return std::make_unique<AStarSearch>(std::make_shared<L1Heuristic>());
    }

    if (algorithm == "a*l2")
    {
        return std::make_unique<AStarSearch>(std::make_shared<L2Heuristic>());
    }

    if (algorithm == "gl1")
    {
        return std::make_unique<GreedySearch>(std::make_shared<L1Heuristic>());
    }

    if (algorithm == "gl2")
    {
        return std::make_unique<GreedySearch>(std::make_shared<L2Heuristic>());
    }

    throw std::invalid_argument(std::format("Invalid search algorithm: {}", algorithm));
}

std::shared_ptr<Node> BreadthFirstSearch::search(const Problem &problem, Visitor &vis) const
{
    auto node = std::make_shared<Node>(problem.initState);

    if (problem.isGoal(node->state))
    {
        vis.visit(problem, node);
        return node;
    }

    auto &&frontier = std::queue<std::shared_ptr<Node>>();
    frontier.push(node);

    auto &&reached = std::unordered_set<State>(0, std::hash<State>(problem.graph));
    reached.insert(node->state);

    while (!frontier.empty())
    {
        node = frontier.front();
        vis.visit(problem, node);
        frontier.pop();

        for (auto child : problem.expand(node))
        {
            if (problem.isGoal(child->state))
            {
                vis.visit(problem, child);
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

std::shared_ptr<Node> DepthFirstSearch::search(const Problem &problem, Visitor &vis) const
{
    auto node = std::make_shared<Node>(problem.initState);

    if (problem.isGoal(node->state))
    {
        vis.visit(problem, node);
        return node;
    }

    auto &&frontier = std::stack<std::shared_ptr<Node>>();
    frontier.push(node);

    auto &&reached = std::unordered_set<State>(0, std::hash<State>(problem.graph));
    reached.insert(node->state);

    while (!frontier.empty())
    {
        node = frontier.top();
        vis.visit(problem, node);
        frontier.pop();

        for (auto child : problem.expand(node))
        {
            if (problem.isGoal(child->state))
            {
                vis.visit(problem, child);
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

std::shared_ptr<Node> BestFirstSearch::search(const Problem &problem, Visitor &vis, const NodeComparer &comparer) const
{
    auto node = std::make_shared<Node>(problem.initState);

    if (problem.isGoal(node->state))
    {
        vis.visit(problem, node);
        return node;
    }

    auto frontier = std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, decltype(comparer)>(comparer);
    frontier.push(node);

    auto reached = std::unordered_map<State, std::remove_const<decltype(Node::pathCost)>::type>(0, std::hash<State>(problem.graph));
    reached.insert({node->state, node->pathCost});

    while (!frontier.empty())
    {
        node = frontier.top();
        vis.visit(problem, node);
        frontier.pop();

        for (auto child : problem.expand(node))
        {
            if (problem.isGoal(child->state))
            {
                vis.visit(problem, child);
                return child;
            }

            auto seenChild = reached.find(child->state);

            if (seenChild == reached.end() || child->pathCost < seenChild->second)
            {
                reached[child->state] = child->pathCost;
                vis.visit(problem, child);
                frontier.push(child);
            }
        }
    }

    return std::shared_ptr<Node>();
}

std::shared_ptr<Node> UniformCostSearch::search(const Problem &problem, Visitor &vis) const
{
    auto comparator = [](const std::shared_ptr<Node> &x, const std::shared_ptr<Node> &y)
    {
        return !(x->pathCost < y->pathCost);
    };

    return BestFirstSearch::search(problem, vis, comparator);
}

std::shared_ptr<Node> AStarSearch::search(const Problem &problem, Visitor &vis) const
{
    auto comparator = [&](const std::shared_ptr<Node> &x, const std::shared_ptr<Node> &y)
    {
        const int xCost = x->pathCost + (*this->heuristic)(problem, x->state);
        const int yCost = y->pathCost + (*this->heuristic)(problem, y->state);
        return xCost > yCost;
    };

    return BestFirstSearch::search(problem, vis, comparator);
}

std::shared_ptr<Node> GreedySearch::search(const Problem &problem, Visitor &vis) const
{
    auto comparator = [&](const std::shared_ptr<Node> &x, const std::shared_ptr<Node> &y)
    {
        const int xCost = (*this->heuristic)(problem, x->state);
        const int yCost = (*this->heuristic)(problem, y->state);
        return xCost > yCost;
    };

    return BestFirstSearch::search(problem, vis, comparator);
}

float Heuristic::operator()(const Problem& problem, const State &state) const
{
    DetailsMap detailsMap = boost::get(vertex_details, problem.graph);

    auto coordsCurrent = boost::get(detailsMap, state.vertex).coords;

    auto coordsGoal = boost::get(detailsMap, problem.stopState.vertex).coords;

    return this->heuristic(coordsCurrent, coordsGoal);
}

float L1Heuristic::heuristic(decltype(VertexDetails::coords) coordsCurrent, decltype(VertexDetails::coords) coordsGoal) const
{
    return std::abs(coordsCurrent.first - coordsGoal.first) + std::abs(coordsCurrent.second - coordsGoal.second);
}

float L2Heuristic::heuristic(decltype(VertexDetails::coords) coordsCurrent, decltype(VertexDetails::coords) coordsGoal) const
{
    return std::sqrt(std::pow(coordsCurrent.first - coordsGoal.first, 2) + std::pow(coordsCurrent.second - coordsGoal.second, 2));
}
#ifndef __SEARCH__
#define __SEARCH__

#include <boost/algorithm/string.hpp>
#include <unordered_set>
#include <unordered_map>
#include <type_traits>
#include <stdexcept>
#include <concepts>
#include <iostream>
#include <format>
#include <vector>
#include <memory>
#include <queue>
#include <stack>

#include "visitor.h"
#include "problem.h"

struct Search
{
    virtual std::shared_ptr<Node> search(const Problem& problem, Visitor& vis) const = 0;

    static std::unique_ptr<Search> createSearchAlgorithm(const std::string& algorithm);

    static std::unique_ptr<Search> createSearchAlgorithm(std::istream& is);
};

struct BreadthFirstSearch : Search
{
    std::shared_ptr<Node> search(const Problem& problem, Visitor& vis) const override
    {
        auto node = std::make_shared<Node>(problem.initState);

        if (problem.isGoal(node->state))
        {
            vis.visit(problem, *node);
            return node;
        }

        auto&& frontier = std::queue<std::shared_ptr<Node>>();
        frontier.push(node);

        auto&& reached = std::unordered_set<State>(0, std::hash<State>(problem.graph));
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
};

struct DepthFirstSearch : Search
{
    std::shared_ptr<Node> search(const Problem& problem, Visitor& vis) const override
    {
        auto node = std::make_shared<Node>(problem.initState);

        if (problem.isGoal(node->state))
        {
            vis.visit(problem, *node);
            return node;
        }

        auto&& frontier = std::stack<std::shared_ptr<Node>>();
        frontier.push(node);

        auto&& reached = std::unordered_set<State>(0, std::hash<State>(problem.graph));
        reached.insert(node->state);

        while(!frontier.empty())
        {
            node = frontier.top();
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
};

struct UniformCostSearch : Search
{
    std::shared_ptr<Node> search(const Problem& problem, Visitor& vis) const override
    {
        auto node = std::make_shared<Node>(problem.initState);

        if (problem.isGoal(node->state))
        {
            vis.visit(problem, *node);
            return node;
        }

        auto comparator = [](const std::shared_ptr<Node>& x, const std::shared_ptr<Node>& y)
        {
            return !(x->pathCost < y->pathCost);
        };

        auto frontier = std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, decltype(comparator)>(comparator);
        frontier.push(node);

        auto reached = std::unordered_map<State, std::remove_const<decltype(Node::pathCost)>::type>(0, std::hash<State>(problem.graph));
        reached.insert({ node->state, node->pathCost });

        while(!frontier.empty())
        {
            node = frontier.top();
            vis.visit(problem, *node);
            frontier.pop();

            for (auto child : problem.expand(node))
            {
                if (problem.isGoal(child->state))
                {
                    vis.visit(problem, *child);
                    return child;
                }

                auto seenChild = reached.find(child->state);

                if (seenChild == reached.end() || child->pathCost < seenChild->second)
                {
                    reached[child->state] = child->pathCost;
                    vis.visit(problem, *child);
                    frontier.push(child);
                }
            }
        }

        return std::shared_ptr<Node>();
    }
};

std::unique_ptr<Search> Search::createSearchAlgorithm(std::istream& is)
{
    std::string algorithm; is >> algorithm;

    boost::algorithm::to_lower(algorithm);

    return Search::createSearchAlgorithm(algorithm);
}

std::unique_ptr<Search> Search::createSearchAlgorithm(const std::string& algorithm)
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

    throw std::invalid_argument(std::format("Invalid search algorithm: {}", algorithm));
}

#endif
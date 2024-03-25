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

struct Heuristic
{
    float operator()(const Problem &problem, const State &state) const;

    virtual float heuristic(decltype(VertexDetails::coords) coordsCurrent, decltype(VertexDetails::coords) coordsGoal) const = 0;
};

struct L1Heuristic final : Heuristic
{
    virtual float heuristic(decltype(VertexDetails::coords) coordsCurrent, decltype(VertexDetails::coords) coordsGoal) const override;
};

struct L2Heuristic final : Heuristic
{
    virtual float heuristic(decltype(VertexDetails::coords) coordsCurrent, decltype(VertexDetails::coords) coordsGoal) const override;
};

struct Search
{
    virtual std::shared_ptr<Node> search(const Problem &problem, Visitor &vis) const = 0;
    static std::unique_ptr<Search> createSearchAlgorithm(const std::string &algorithm);
    static std::unique_ptr<Search> createSearchAlgorithm(std::istream &is);
};

struct BreadthFirstSearch : Search
{
    std::shared_ptr<Node> search(const Problem &problem, Visitor &vis) const override;
};

struct DepthFirstSearch : Search
{
    std::shared_ptr<Node> search(const Problem &problem, Visitor &vis) const override;
};
struct BestFirstSearch : Search
{
    using NodeComparer = std::function<bool(std::shared_ptr<Node>, std::shared_ptr<Node>)>;

    std::shared_ptr<Node> search(const Problem &problem, Visitor &vis, const NodeComparer& comparer) const;
};

struct UniformCostSearch : BestFirstSearch
{
    std::shared_ptr<Node> search(const Problem &problem, Visitor &vis) const override;
};

struct AStarSearch : BestFirstSearch
{
    AStarSearch(std::shared_ptr<Heuristic> heuristic) : heuristic(heuristic) {}

    std::shared_ptr<Node> search(const Problem &problem, Visitor &vis) const override;

private:
    std::shared_ptr<Heuristic> heuristic;
};

struct GreedySearch : BestFirstSearch
{
    GreedySearch(std::shared_ptr<Heuristic> heuristic) : heuristic(heuristic) {}

    std::shared_ptr<Node> search(const Problem &problem, Visitor &vis) const override;

private:
    std::shared_ptr<Heuristic> heuristic;
};

#endif
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

struct UniformCostSearch : Search
{
    std::shared_ptr<Node> search(const Problem &problem, Visitor &vis) const override;
};

#endif
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <fstream>
#include <format>

#include "anim/anim_explore.h"
#include "searching/visitor.h"
#include "searching/search.h"
#include "render/manager.h"
#include "base/component.h"
#include "maze/graph.h"
#include "maze/cell.h"
#include "maze/maze.h"

int main()
{
    // Manage resources & rendering
    auto manager = std::make_unique<Manager>();
    manager->setTitle("Problem Solving by Searching");

    // Retrieve maze from input file
    manager->setResourcePath("bin/resources");
    std::ofstream os = std::ofstream(manager->getOutputPath());
    std::string filename = "input_0.txt";
    std::cout << "Enter file: ";
    std::cin >> filename;
    auto &&maze = Maze<Cell>::from_file(manager->getInputPath() / filename);

    // Create a small window
    manager->setResolution(1920, 1080);

    // Convert maze to graph structure
    auto &&[graphPtr, c2v] = Maze<Cell>::to_graph<Graph, DetailsMap>(maze);
    auto &&detailsMap = boost::get(vertex_details, *graphPtr);
    Graph &graph = *graphPtr;

    // Initial State
    auto initState = State(c2v[maze.initPos]);
    auto stopState = State(c2v[maze.stopPos]);
    auto problem = Problem(graph, initState, stopState);

    // Choose algorithm
    std::string alg = "ucs";
    std::cout << "Choose alg(ucs/bfs/dfs/a*l1/a*l2/gl1/gl2): ";
    std::cin >> alg;
    auto algorithm = Search::createSearchAlgorithm(alg);

    // Perform search and track visits
    auto vis = ExplorationVisitor(maze, os);
    auto node = algorithm->search(problem, vis);

    // Check for no solutions
    if (!node)
    {
        os << "No solution was found!" << std::endl;
        return EXIT_FAILURE;
    }

    os << "A solution was found:" << std::endl;
    node->rchain([&](const Node& elem)
    {
        auto details = boost::get(detailsMap, elem.state.vertex);

        os << std::format("({}, {})", details.coords.first, details.coords.second);

        if (details.type != Cell::CellType::Door)
        {
            os << "->";
        }
    });

    os << std::endl;

    // Render the animation
    manager->init();
    manager->addSceneObject(std::make_shared<GridAnimation>(vis.getHistory(), 5));
    manager->runLoop();

    return EXIT_SUCCESS;
}
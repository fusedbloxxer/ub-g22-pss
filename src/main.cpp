#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <fstream>
#include <format>

#include "animations/grid_animation.h"
#include "components/component.h"
#include "manager/manager.h"
#include "search/visitor.h"
#include "search/search.h"
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
    std::string filename = "input_3.txt";
    std::cout << "Enter file: ";
    std::cin >> filename;
    auto &&maze = Maze<Cell>::from_file(manager->getInputPath() / filename);

    // Create a small window
    manager->setResolution(1280, 1024);

    // Convert maze to graph structure
    auto &&[graphPtr, c2v] = Maze<Cell>::to_graph<Graph, DetailsMap>(maze);
    auto &&detailsMap = boost::get(vertex_details, *graphPtr);
    Graph &graph = *graphPtr;

    // Initial State
    auto initState = State(c2v[maze.initPos]);
    auto stopState = State(c2v[maze.stopPos]);
    auto problem = Problem(graph, initState, stopState);

    // Choose algorithm
    std::string alg = "gl2";
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

    // Prepare the animation
    auto animation = std::make_shared<GridAnimation>(vis.getHistory(), detailsMap, node);
    animation->setAnimationDuration(sf::seconds(8));
    animation->setEndingDuration(sf::seconds(5));

    // Render the animation
    manager->init();
    manager->addSceneObject(animation);
    manager->runLoop();

    return EXIT_SUCCESS;
}
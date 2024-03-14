#include <iostream>
#include <fstream>
#include <format>

#include "cell.h"
#include "visitor.h"
#include "search.h"
#include "graph.h"
#include "maze.h"

int main()
{
    // Where the output should be forwarded to
    std::string output;
    std::cout << "Where do you want the output(file/console): ";
    std::cin >> output;
    std::ofstream output_file("output.txt", std::ios::trunc);
    std::ostream& os = output == "file" ? output_file : std::cout;

    // Read maze fron input file
    std::cout << "Enter path to file: ";
    auto &&maze = Maze<Cell>::from_input(std::cin);
    os << "Maze:" << std::endl << maze << std::endl;

    // Convert maze to graph structure
    auto &&[graphPtr, c2v] = Maze<Cell>::to_graph<Graph, DetailsMap>(maze);
    auto &&detailsMap = boost::get(vertex_details, *graphPtr);
    Graph &graph = *graphPtr;

    // Initial State
    auto initState = State(c2v[maze.initPos]);
    auto problem = Problem(graph, initState);

    // Choose algorithm
    std::cout << "Choose search algorithm (bfs/dfs/ucs): " << std::flush;
    auto algorithm = Search::createSearchAlgorithm(std::cin);

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

    return EXIT_SUCCESS;
}
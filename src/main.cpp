#include <format>

#include "visitor.h"
#include "search.h"
#include "graph.h"
#include "maze.h"

int main()
{
    // Read maze fron input file
    auto &&maze = Maze<Cell>::from_file("res/input_2.txt");
    std::cout << "Maze:" << std::endl << maze << std::endl;

    // Convert maze to graph structure
    auto &&[graphPtr, c2v] = Maze<Cell>::to_graph<Graph, DetailsMap>(maze);
    auto &&detailsMap = boost::get(vertex_details, *graphPtr);
    Graph &graph = *graphPtr; //

    // Initial State
    auto initState = State(c2v[maze.initPos]);
    auto problem = Problem(graph, initState);

    // BFS
    auto vis = ExplorationVisitor(maze);
    auto node = breadthFirstSearch(problem, vis);

    // Check for no solutions
    if (!node)
    {
        std::cout << "No solution was found!" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "A solution was found:" << std::endl;

    node->rchain([&](const Node& elem)
    {
        auto details = boost::get(detailsMap, elem.state.vertex);

        std::cout << std::format("({}, {})", details.coords.first, details.coords.second);

        if (details.type != Cell::CellType::Door)
        {
            std:: cout << "->";
        }
    });

    return EXIT_SUCCESS;
}
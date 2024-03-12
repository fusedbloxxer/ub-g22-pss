#include <format>
#include "cell.h"
#include "maze.h"
#include "graph.h"
#include "graph_search.h"

int main()
{
    // Read maze fron input file
    auto &&maze = Maze<Cell>::from_file("res/input.txt");
    std::cout << maze << std::endl;

    // Convert maze to graph structure
    auto&& [graphPtr, c2v] = Maze<Cell>::to_graph<Graph, DetailsMap>(maze);
    auto&& details_map = boost::get(vertex_details, *graphPtr);
    Graph& graph = *graphPtr;

    // Initial State
    auto initPos = std::make_pair(1, 1);
    auto initState = State(graph, c2v[initPos]);

    // Get some actions
    auto actions = ACTIONS(initState);

    for (auto it_a = actions.begin(); it_a != actions.end(); ++it_a)
    {
        const auto& nextState = RESULT(initState, *it_a);

        const auto& coords = boost::get(details_map, nextState.vertex).coords;

        std::cout << coords.first << " " << coords.second << ", ";
    }

    // Stop
    return EXIT_SUCCESS;
}
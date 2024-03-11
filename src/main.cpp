#include "cell.h"
#include "maze.h"
#include "graph.h"

int main()
{
    // Read maze fron input file
    auto maze = Maze<Cell>::from_file("res/input.txt");

    // Convert maze to graph structure
    auto&& [graph, c2v] = Maze<Cell>::to_graph<Graph, DetailsMap>(maze);

    return EXIT_SUCCESS;
}
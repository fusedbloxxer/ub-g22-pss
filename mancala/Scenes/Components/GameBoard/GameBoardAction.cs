using Godot;
using System.Collections.Generic;

public class GameBoardAction
{
    IEnumerable<GameBoardCell> ChangedCells { get; init; }

    GameBoardCell LastChangedCell { get; init; }
}

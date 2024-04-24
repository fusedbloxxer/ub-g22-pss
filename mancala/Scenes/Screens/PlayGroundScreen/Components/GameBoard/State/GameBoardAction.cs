using Godot;
using System.Collections.Generic;

public class GameBoardAction
{
    public IEnumerable<GameBoardCell> ChangedCells { get; init; }

    public GameBoardCell LastChangedCell { get; init; }

    public PlayerName NextPlayer { get; init; }

    public bool IsGameOver { get; init; }
}
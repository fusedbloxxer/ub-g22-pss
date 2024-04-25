using Godot;
using System;
using System.Collections.Generic;
using System.Linq;

public partial class AISearchStrategy : Node
{
    public virtual IEnumerable<int> Search(GameBoardState board, Player player)
    {
        return Array.Empty<int>();
    }

    public enum Method
    {
        Random = 0,
        HeuristicAlphaBeta = 1,
    }
}

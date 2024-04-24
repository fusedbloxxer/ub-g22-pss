using Godot;
using System;
using System.Collections.Generic;
using System.Linq;

public partial class AISearchStrategy : Node
{
    public virtual IEnumerable<uint> Search(GameBoardState board, Player player)
    {
        return Array.Empty<uint>();
    }
}

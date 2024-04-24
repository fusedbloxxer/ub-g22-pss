using Godot;
using System;
using System.Collections.Generic;
using System.Linq;

public partial class AIRandomSearchStrategy : AISearchStrategy
{
    public override IEnumerable<uint> Search(GameBoardState state, Player player)
    {
        var cups = state.Cells
            .Where(x => x is GameBoardCup && x.OwnerPlayer == player.Name)
            .Where(x => x.Pebbles > 0)
            .Select(x => x.Index)
            .ToList();

        var nonEmptyindex = new Random().Next(cups.Count);

        var cupIndex = cups.ElementAt(nonEmptyindex);

        return new List<uint>() { cupIndex };
    }
}
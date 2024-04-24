using Godot;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public partial class GameBoardCup : GameBoardCell
{
    public int Opposite { get; }

    public GameBoardCup(PlayerName playerOwner, int index, int opposite)
    : base(playerOwner, index)
    {
        Opposite = opposite;
    }
}
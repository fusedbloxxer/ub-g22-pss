using Godot;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public partial class GameBoardCup : GameBoardCell
{
    public uint Opposite { get; }

    public GameBoardCup(PlayerName playerOwner, uint index, uint opposite)
    : base(playerOwner, index)
    {
        Opposite = opposite;
    }
}
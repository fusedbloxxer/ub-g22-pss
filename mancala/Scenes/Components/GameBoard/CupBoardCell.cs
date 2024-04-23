using Godot;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public partial class CupBoardCell : GameBoardCell
{
    public uint Opposite { get; }

    public CupBoardCell(Player playerOwner, uint index, uint opposite)
    : base(playerOwner, index)
    {
        Opposite = opposite;
    }
}
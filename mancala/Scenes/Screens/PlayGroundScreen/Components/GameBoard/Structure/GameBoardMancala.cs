using Godot;
using System;
using System.Collections.Generic;
using System.Linq;

public partial class GameBoardMancala : GameBoardCell
{
    public GameBoardMancala(PlayerName playerOwner, int index)
    : base(playerOwner, index)
    {

    }
}
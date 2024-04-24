using Godot;
using System;

public partial class GameBoardCell
{
	public PlayerName OwnerPlayer { get; }

	public int Pebbles { get; set; }

	public int Index { get; }

	protected GameBoardCell(PlayerName ownerPlayer, int index)
	{
		OwnerPlayer = ownerPlayer;
		Index = index;
	}
}

using Godot;
using System;

public partial class GameBoardCell
{
	public Player OwnerPlayer { get; }

	public uint Pebbles { get; set; }

	public uint Index { get; }

	protected GameBoardCell(Player ownerPlayer, uint index)
	{
		OwnerPlayer = ownerPlayer;
		Index = index;
	}
}

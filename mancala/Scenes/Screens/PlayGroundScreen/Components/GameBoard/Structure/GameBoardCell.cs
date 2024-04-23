using Godot;
using System;

public partial class GameBoardCell
{
	public PlayerName OwnerPlayer { get; }

	public uint Pebbles { get; set; }

	public uint Index { get; }

	protected GameBoardCell(PlayerName ownerPlayer, uint index)
	{
		OwnerPlayer = ownerPlayer;
		Index = index;
	}
}

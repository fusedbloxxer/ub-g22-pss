using Godot;
using System;
using System.Collections.Generic;
using System.Linq;

public partial class GameBoardState : Node
{
	public GameBoardCell[] Cells { get; set; }

	// Called when the node enters the scene tree for the first time.
	public override void _Ready()
	{
		// Initialize the state
		ResetState();
	}

	// Called every frame. 'delta' is the elapsed time since the previous frame.
	public override void _Process(double delta)
	{

	}

	public IEnumerable<GameBoardCell> DoAction(uint index)
	{
		// Retrieve metadata
		var changes = new List<GameBoardCell>();
		var player = Cells[index].OwnerPlayer;
		var pebbles = Cells[index].Pebbles;

		// Remove pebbles from the clicked entry
		Cells[index].Pebbles = 0;
		changes.Add(Cells[index]);
		index = (uint)((index + 1) % Cells.Length);

		// Give them to the following cells
		while (pebbles != 0)
		{
			Cells[index].Pebbles += 1;

			changes.Add(Cells[index]);

			--pebbles;

			index = (uint)((index + 1) % Cells.Length);
		}

		// Check if last pebble fell alone into a cup which has an opposite with at least one pebble
		if (changes.Last() is CupBoardCell and { Pebbles: 1 } cup && cup.OwnerPlayer == player && Cells[cup.Opposite].Pebbles != 0u)
		{
			// Retrieve the lucky player's mancala
			var mancala = Cells.First(x => x is MancalaBoardCell && x.OwnerPlayer == player);

			// Add the pebbles from both cups
			mancala.Pebbles += Cells[cup.Index].Pebbles + Cells[cup.Opposite].Pebbles;
			changes.Add(mancala);

			// Remove pebbles from current
			Cells[cup.Index].Pebbles = 0u;
			changes.Add(Cells[cup.Index]);

			// Remove pebbles from opposite
			Cells[cup.Opposite].Pebbles = 0u;
			changes.Add(Cells[cup.Opposite]);
		}

		// Retreive the list of changes to notify the UI
		return changes;
	}

	public void ResetState()
	{
		Cells = new GameBoardCell[]
		{
			// First part of the board
			new CupBoardCell(Player.A, 0u, 12u) { Pebbles = 4 },
			new CupBoardCell(Player.A, 1u, 11u) { Pebbles = 4 },
			new CupBoardCell(Player.A, 2u, 10u) { Pebbles = 4 },
			new CupBoardCell(Player.A, 3u, 9u) { Pebbles = 4 },
			new CupBoardCell(Player.A, 4u, 8u) { Pebbles = 4 },
			new CupBoardCell(Player.A, 5u, 7u) { Pebbles = 4 },
			new MancalaBoardCell(Player.A, 6u),

			// Second part of the board
			new CupBoardCell(Player.B, 7u, 5u) { Pebbles = 4 },
			new CupBoardCell(Player.B, 8u, 4u) { Pebbles = 4 },
			new CupBoardCell(Player.B, 9u, 3u) { Pebbles = 4 },
			new CupBoardCell(Player.B, 10u, 2u) { Pebbles = 4 },
			new CupBoardCell(Player.B, 11u, 1u) { Pebbles = 4 },
			new CupBoardCell(Player.B, 12u, 0u) { Pebbles = 4 },
			new MancalaBoardCell(Player.B, 13u),
		};
	}
}

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
		Reset();
	}

	// Called every frame. 'delta' is the elapsed time since the previous frame.
	public override void _Process(double delta)
	{

	}

	public GameBoardAction DoAction(uint index)
	{
		// Retrieve metadata
		var changes = new List<GameBoardCell>();
		var player = Cells[index].OwnerPlayer;
		var pebbles = Cells[index].Pebbles;
		var isGameOver = false;

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
		if (changes.Last() is GameBoardCup and { Pebbles: 1 } cup && cup.OwnerPlayer == player && Cells[cup.Opposite].Pebbles != 0u)
		{
			// Retrieve the lucky player's mancala
			var mancala = Cells.First(x => x is GameBoardMancala && x.OwnerPlayer == player);

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

		// Retrieve the last cell so it's known where it lands
		var lastChangedCell = changes.Last();

		// Check if either player side is empty and distribute the points
		var playerCupsA = Cells
			.Where(x => x.OwnerPlayer == PlayerName.A && x is GameBoardCup)
			.Cast<GameBoardCup>();
		var playerCupsB = Cells
			.Where(x => x.OwnerPlayer == PlayerName.B && x is GameBoardCup)
			.Cast<GameBoardCup>();
		
		var playerCupsPebblesA = playerCupsA
			.Select(x => x.Pebbles)
			.Aggregate(0u, (acc, x) => acc + x);
		var playerCupsPebblesB = playerCupsB
			.Select(x => x.Pebbles)
			.Aggregate(0u, (acc, x) => acc + x);
		
		if (playerCupsPebblesA == 0u || playerCupsPebblesB == 0u)
		{
			// Get the non-empty side
			var (playerCups, playerPebbles, playerMancala) = playerCupsPebblesA == 0u
				? (playerCupsB, playerCupsPebblesB, Cells.First(x => x.OwnerPlayer == PlayerName.B && x is GameBoardMancala))
				: (playerCupsA, playerCupsPebblesA, Cells.First(x => x.OwnerPlayer == PlayerName.A && x is GameBoardMancala));
		
			// Zero out all pebbles on the non-empty side
			foreach (var playerCup in playerCups)
			{
				playerCup.Pebbles = 0;
				changes.Add(playerCup);
			}

			// Get all pebbles and place them in mancala
			playerMancala.Pebbles += playerPebbles;
			changes.Add(playerMancala);
			isGameOver = true;
		}

		// Retreive the list of changes to notify the UI
		return new GameBoardAction
		{
			ChangedCells = changes.DistinctBy(x => x.Index),
			LastChangedCell = lastChangedCell,
			IsGameOver = isGameOver,
		};
	}

	public void Reset()
	{
		Cells = new GameBoardCell[]
		{
			// First part of the board
			new GameBoardCup(PlayerName.A, 0u, 12u) { Pebbles = 4 },
			new GameBoardCup(PlayerName.A, 1u, 11u) { Pebbles = 4 },
			new GameBoardCup(PlayerName.A, 2u, 10u) { Pebbles = 4 },
			new GameBoardCup(PlayerName.A, 3u, 9u) { Pebbles = 4 },
			new GameBoardCup(PlayerName.A, 4u, 8u) { Pebbles = 4 },
			new GameBoardCup(PlayerName.A, 5u, 7u) { Pebbles = 4 },
			new GameBoardMancala(PlayerName.A, 6u),

			// Second part of the board
			new GameBoardCup(PlayerName.B, 7u, 5u) { Pebbles = 4 },
			new GameBoardCup(PlayerName.B, 8u, 4u) { Pebbles = 4 },
			new GameBoardCup(PlayerName.B, 9u, 3u) { Pebbles = 4 },
			new GameBoardCup(PlayerName.B, 10u, 2u) { Pebbles = 4 },
			new GameBoardCup(PlayerName.B, 11u, 1u) { Pebbles = 4 },
			new GameBoardCup(PlayerName.B, 12u, 0u) { Pebbles = 4 },
			new GameBoardMancala(PlayerName.B, 13u),
		};
	}
}

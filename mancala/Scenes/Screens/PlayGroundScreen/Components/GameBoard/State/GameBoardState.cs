using Godot;
using System;
using System.Collections.Generic;
using System.Linq;
public partial class GameBoardState : Node, ICloneable
{
	public GameBoardCell[] Cells { get; set; }

	public GameBoardState()
	{
		Reset();
	}

	public override void _Ready()
	{
		Reset();
	}

	public GameBoardAction DoAction(int index)
	{
		// Retrieve metadata
		List<GameBoardCell> changes = new List<GameBoardCell>();
		PlayerName player = Cells[index].OwnerPlayer;
		int pebbles = Cells[index].Pebbles;
		bool isGameOver = false;
		PlayerName nextPlayer;

		// Remove pebbles from the clicked entry
		Cells[index].Pebbles = 0;
		changes.Add(Cells[index]);
		index = (int)((index + 1) % Cells.Length);

		// Give them to the following cells
		while (pebbles != 0)
		{
			Cells[index].Pebbles += 1;

			changes.Add(Cells[index]);

			--pebbles;

			index = (int)((index + 1) % Cells.Length);
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
			Cells[cup.Index].Pebbles = 0;
			changes.Add(Cells[cup.Index]);

			// Remove pebbles from opposite
			Cells[cup.Opposite].Pebbles = 0;
			changes.Add(Cells[cup.Opposite]);
		}

		// Retrieve the last cell so it's known where it lands
		var lastChangedCell = changes.Last();

		// Let the player continue the turn if the last pebble falls on their mancala
		if (lastChangedCell is GameBoardMancala && lastChangedCell.OwnerPlayer == player)
		{
			nextPlayer = player;
		}
		else
		{
			nextPlayer = player == PlayerName.A ? PlayerName.B : PlayerName.A;
		}

		// Check if either player side is empty and distribute the points
		var playerCupsA = Cells
			.Where(x => x.OwnerPlayer == PlayerName.A && x is GameBoardCup)
			.Cast<GameBoardCup>();
		var playerCupsB = Cells
			.Where(x => x.OwnerPlayer == PlayerName.B && x is GameBoardCup)
			.Cast<GameBoardCup>();
		
		var playerCupsPebblesA = playerCupsA
			.Select(x => x.Pebbles)
			.Aggregate(0, (acc, x) => acc + x);
		var playerCupsPebblesB = playerCupsB
			.Select(x => x.Pebbles)
			.Aggregate(0, (acc, x) => acc + x);
		
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
			NextPlayer = nextPlayer,
		};
	}

	public void Reset()
	{
		Cells = new GameBoardCell[]
		{
			// First part of the board
			new GameBoardCup(PlayerName.A, 0, 12) { Pebbles = 4 },
			new GameBoardCup(PlayerName.A, 1, 11) { Pebbles = 4 },
			new GameBoardCup(PlayerName.A, 2, 10) { Pebbles = 4 },
			new GameBoardCup(PlayerName.A, 3, 9) { Pebbles = 4 },
			new GameBoardCup(PlayerName.A, 4, 8) { Pebbles = 4 },
			new GameBoardCup(PlayerName.A, 5, 7) { Pebbles = 4 },
			new GameBoardMancala(PlayerName.A, 6),

			// Second part of the board
			new GameBoardCup(PlayerName.B, 7, 5) { Pebbles = 4 },
			new GameBoardCup(PlayerName.B, 8, 4) { Pebbles = 4 },
			new GameBoardCup(PlayerName.B, 9, 3) { Pebbles = 4 },
			new GameBoardCup(PlayerName.B, 10, 2) { Pebbles = 4 },
			new GameBoardCup(PlayerName.B, 11, 1) { Pebbles = 4 },
			new GameBoardCup(PlayerName.B, 12, 0) { Pebbles = 4 },
			new GameBoardMancala(PlayerName.B, 13),
		};
	}

	public object Clone()
	{
		var state = new GameBoardState();

		for (int i = 0; i != Cells.Length; i++)
		{
			state.Cells[i].Pebbles = Cells[i].Pebbles;
		}

		return state;
	}
}

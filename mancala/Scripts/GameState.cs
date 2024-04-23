using Godot;
using System;
using System.Linq;

public partial class GameState : Node
{
	[Signal]
	public delegate void PlayerTurnChangedEventHandler(Player currentPlayer);

	[Signal]
	public delegate void GameBoardCellUpdateEventHandler(uint index, uint pebbles);

	public Player CurrentPlayer { get; set; } = Player.A;

	private GameBoardState _boardState;
	
	private Node _root;

	// Called when the node enters the scene tree for the first time.
	public override void _Ready()
	{
		// Retrieve local refs
		_root = GetNode<Node>("/root/Main");

		// Listen to global signals
		GetNode<SceneManager>("/root/SceneManager").GameStart += OnSceneManagerGameStart;
	}

	// Called every frame. 'delta' is the elapsed time since the previous frame.
	public override void _Process(double delta)
	{

	}

	private void OnSceneManagerGameStart(GameBoardState state)
	{
		_boardState = state;
	}

	public void SelectCupCell(uint index)
	{
		// Trigger action
		var changes = _boardState.DoAction(index);
	
		// Notify each cell about the change
		foreach (var change in changes)
		{
			EmitSignal(SignalName.GameBoardCellUpdate, change.Index, change.Pebbles);
		}

		// Let the player continue the turn if the last pebble falls on their mancala
		if (changes.Last() is not MancalaBoardCell mancala || mancala.OwnerPlayer != _boardState.Cells[index].OwnerPlayer)
		{
			NextTurn();
		}
	}

	public Player NextTurn()
	{
		// Toggle the player turn
		CurrentPlayer = CurrentPlayer switch
		{
			Player.A => Player.B,
			Player.B => Player.A,
			_ => throw new NotImplementedException()
		};

		// Inform others that it's next player's turn
		EmitSignal(SignalName.PlayerTurnChanged, Convert.ToInt32(CurrentPlayer));
		
		// Deliver the turn's current player
		return CurrentPlayer;
	}
}

using Godot;
using System;
using System.Linq;

public partial class GameManager : Node
{
	[Signal]
	public delegate void GameStartEventHandler();

	[Signal]
	public delegate void GameOverEventHandler();

	[Signal]
	public delegate void PlayerTurnChangedEventHandler(Player player);

	[Signal]
	public delegate void GameBoardCellUpdateEventHandler(uint index, uint pebbles);

	public Player CurrentPlayer { get; set; }

	private SceneManager _sceneManager;
	private GameBoard _gameBoard;
	private Node _root;

	public override void _Ready()
	{
		// Retrieve local refs
		_root = GetNode<Node>("/root/Main");
		_sceneManager = GetNode<SceneManager>("/root/SceneManager");

		// Listen to global signals
		_sceneManager.ShowPlayGroundScene += OnSceneManagerShowPlayGroundScene;
	}

	private void OnSceneManagerShowPlayGroundScene(GameBoard board, GameSettings settings)
	{
		// Retrieve the board and the current settings
		_gameBoard = board;

		// Inject the settings into the board
		_gameBoard.Settings = settings;

		// Start a new game
		StartGame();
	}

	public void SelectCup(uint index)
	{
		// Trigger action
		var action = _gameBoard.State.DoAction(index);

		// Notify each cell about the change
		foreach (var change in action.ChangedCells)
		{
			EmitSignal(SignalName.GameBoardCellUpdate, change.Index, change.Pebbles);
		}

		// Notify all that the game has ended
		if (action.IsGameOver)
		{
			EmitSignal(SignalName.GameOver);
			return;
		}

		// Let the player continue the turn if the last pebble falls on their mancala
		if (action.LastChangedCell is not GameBoardMancala mancala || mancala.OwnerPlayer != _gameBoard.State.Cells[index].OwnerPlayer)
		{
			NextTurn();
		}
	}

	public Player NextTurn()
	{
		// Toggle the player turn
		CurrentPlayer = ReferenceEquals(CurrentPlayer, _gameBoard.Settings.PlayerA)
			? _gameBoard.Settings.PlayerB
			: _gameBoard.Settings.PlayerA;

		// Inform others that it's next player's turn
		EmitSignal(SignalName.PlayerTurnChanged, CurrentPlayer);

		// Deliver the turn's current player
		return CurrentPlayer;
	}

	public void StartGame()
	{
		// Renew the game state
		_gameBoard.State.Reset();

		// Notify the UI to re-render
		foreach (var cell in _gameBoard.State.Cells)
		{
			EmitSignal(SignalName.GameBoardCellUpdate, cell.Index, cell.Pebbles);
		}

		// Player A is always first
		CurrentPlayer = _gameBoard.Settings.PlayerA;

		// Inform others who the first player is
		EmitSignal(SignalName.PlayerTurnChanged, CurrentPlayer);

		// Notify the game has started
		EmitSignal(SignalName.GameStart);
	}
}

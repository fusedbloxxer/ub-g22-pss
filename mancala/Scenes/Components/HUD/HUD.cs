using Godot;
using System;

public partial class HUD : Node
{
	private Button _nextTurnButton;

	private CanvasLayer _canvasLayer;
	private Label _labelPlayerA;
	private Label _labelPlayerB;

	private GameState _gameState;

	// Called when the node enters the scene tree for the first time.
	public override void _Ready()
	{
		// Retrieve global refs
		_gameState = GetNode<GameState>("/root/GameState");

		// Retrieve local refs
		_canvasLayer = GetNode<CanvasLayer>("CanvasLayer");
		_labelPlayerA = _canvasLayer.GetNode<Label>("LabelPlayerA");
		_labelPlayerB = _canvasLayer.GetNode<Label>("LabelPlayerB");
		_nextTurnButton = _canvasLayer.GetNode<Button>("NextTurnButton");

		// Listen to global signals
		_gameState.PlayerTurnChanged += OnGameStatePlayerTurnChanged;

		// Listen to local signals
		_nextTurnButton.Toggled += OnNextTurnButtonToggled;

		// Set initial state
		OnGameStatePlayerTurnChanged(_gameState.CurrentPlayer);
	}

	// Called every frame. 'delta' is the elapsed time since the previous frame.
	public override void _Process(double delta)
	{
	}

	private void OnGameStatePlayerTurnChanged(Player currentPlayer)
	{
		var currentPlayerLabel = currentPlayer switch
		{
			Player.A => _labelPlayerA,
			Player.B => _labelPlayerB,
			_ => throw new Exception()
		};

		var otherPlayerLabel = currentPlayer switch
		{
			Player.A => _labelPlayerB,
			Player.B => _labelPlayerA,
			_ => throw new Exception()
		};

		currentPlayerLabel.AddThemeColorOverride("font_color", Colors.Yellow);
		otherPlayerLabel.AddThemeColorOverride("font_color", Colors.White);
	}

	private void OnNextTurnButtonToggled(bool isToggled)
	{
		_gameState.NextTurn();
	}
}
